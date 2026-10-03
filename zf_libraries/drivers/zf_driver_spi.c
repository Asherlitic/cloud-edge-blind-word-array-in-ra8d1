/*********************************************************************************************************************
* 瑞萨 RA8D1 适配版 zf_driver_spi.c
* 移除了 NXP 的 LPSPI 和 IOMUX 依赖，底层全面接管为瑞萨 FSP (R_SPI / R_SCI_SPI)
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "zf_driver_spi.h"

// 建立全局标志位，支持多个 SPI 模块的非阻塞传输等待 (最高支持10个)
static volatile bool g_spi_tx_done[10] = {false};

// =========================================================================
// 中断回调函数接收区 
// (注意：如果你在 hal_entry.c 中已经写过同名回调，请把这删掉或移过去)
// =========================================================================
void spi1_callback(spi_callback_args_t *p_args) {
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event) g_spi_tx_done[SPI_1] = true;
}

void spi2_callback(spi_callback_args_t *p_args) {
    // 假设你在 FSP 中给 SCI2 起名为 g_sci_spi2，且通道号对应 SPI_2
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event) g_spi_tx_done[SPI_2] = true;
}

// =========================================================================
// 核心映射函数：将逐飞的模块枚举映射到瑞萨的 FSP 实例
// =========================================================================
static const spi_instance_t * get_spi_instance(spi_index_enum spi_n) {
    switch(spi_n) {
        // 根据你的 configuration.xml，目前配置了 g_spi1 和 g_sci_spi2
        case SPI_1: return &g_spi1;
        case SPI_2: return &g_sci_spi2; 
        // 如果后续加了 SPI_3 等，直接在这里新增 case 即可
        default: return NULL;
    }
}

// =========================================================================
// 统一的底层数据传输核心 (自动适配读写和位宽)
// =========================================================================
/***static void spi_transfer_generic(spi_index_enum spi_n, const void *tx_buf, void *rx_buf, uint32 length, spi_bit_width_t bit_width) {
    const spi_instance_t * spi = get_spi_instance(spi_n);
    if(NULL == spi || 0 == length) return;

    g_spi_tx_done[spi_n] = false;

    if (tx_buf != NULL && rx_buf != NULL) {
        // 同时读写
        spi->p_api->writeRead(spi->p_ctrl, tx_buf, rx_buf, length, bit_width);
    } else if (tx_buf != NULL) {
        // 只写
        spi->p_api->write(spi->p_ctrl, tx_buf, length, bit_width);
    } else if (rx_buf != NULL) {
        // 只读
        spi->p_api->read(spi->p_ctrl, rx_buf, length, bit_width);
    }

    // 死等传输完成 (模拟原 NXP 的阻塞机制)
    while(!g_spi_tx_done[spi_n]){;}
}
***/
// 统一的底层数据传输核心 (自动适配读写和位宽)
// =========================================================================
static void spi_transfer_generic(spi_index_enum spi_n, const void *tx_buf, void *rx_buf, uint32 length, spi_bit_width_t bit_width) {
    const spi_instance_t * spi = get_spi_instance(spi_n);
    if(NULL == spi || 0 == length) return;
    

    g_spi_tx_done[spi_n] = false;
    
    if (tx_buf != NULL && rx_buf != NULL) {
        // 同时读写
        spi->p_api->writeRead(spi->p_ctrl, tx_buf, rx_buf, length, bit_width);
    } else if (tx_buf != NULL) {
        // 只写
        spi->p_api->write(spi->p_ctrl, tx_buf, length, bit_width);
    } else if (rx_buf != NULL) {
        // 只读
        spi->p_api->read(spi->p_ctrl, rx_buf, length, bit_width);
    }
    
    // 死等传输完成 (模拟原 NXP 的阻塞机制)
    while(!g_spi_tx_done[spi_n]){;} 


}


//-------------------------------------------------------------------------------------------------------------------
// 瑞萨的引脚复用在 FSP 图形界面中完成，此处函数保留以防上层报错，但设为空函数即可
//-------------------------------------------------------------------------------------------------------------------
void spi_iomuxc(spi_index_enum spi_n, spi_sck_pin_enum sck_pin, spi_mosi_pin_enum mosi_pin, spi_miso_pin_enum miso_pin, spi_cs_pin_enum cs_pin)
{
    // Do nothing for Renesas RA
    (void)spi_n; (void)sck_pin; (void)mosi_pin; (void)miso_pin; (void)cs_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// SPI 接口初始化 (仅调用开启)
//-------------------------------------------------------------------------------------------------------------------
void spi_init (spi_index_enum spi_n, spi_mode_enum mode, uint32 baud, spi_sck_pin_enum sck_pin, spi_mosi_pin_enum mosi_pin, spi_miso_pin_enum miso_pin, spi_cs_pin_enum cs_pin)
{
    (void)mode; (void)baud; (void)sck_pin; (void)mosi_pin; (void)miso_pin; (void)cs_pin;
    
    const spi_instance_t * spi = get_spi_instance(spi_n);
    if(NULL == spi) return;
    
    // 打开 FSP SPI 硬件层 (工作模式、波特率已经在 FSP 配置中固化)
    spi->p_api->open(spi->p_ctrl, spi->p_cfg);
}

// =========================================================================
// 以下为对外接口 API (直接封装调用 spi_transfer_generic)
// =========================================================================

void spi_write_8bit_array (spi_index_enum spi_n, const uint8 *data, uint32 length) {
    spi_transfer_generic(spi_n, data, NULL, length, SPI_BIT_WIDTH_8_BITS);
}

void spi_write_8bit (spi_index_enum spi_n, const uint8 data) {
    spi_transfer_generic(spi_n, &data, NULL, 1, SPI_BIT_WIDTH_8_BITS);
}

void spi_write_16bit (spi_index_enum spi_n, const uint16 data) {
    spi_transfer_generic(spi_n, &data, NULL, 1, SPI_BIT_WIDTH_16_BITS);
}

void spi_write_16bit_array (spi_index_enum spi_n, const uint16 *data, uint32 length) {
    spi_transfer_generic(spi_n, data, NULL, length, SPI_BIT_WIDTH_16_BITS);
}

void spi_write_8bit_register (spi_index_enum spi_n, const uint8 register_name, const uint8 data) {
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_8_BITS);
    spi_transfer_generic(spi_n, &data, NULL, 1, SPI_BIT_WIDTH_8_BITS);
}

void spi_write_8bit_registers (spi_index_enum spi_n, const uint8 register_name, const uint8 *data, uint32 length) {
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_8_BITS);
    spi_transfer_generic(spi_n, data, NULL, length, SPI_BIT_WIDTH_8_BITS);
}

void spi_write_16bit_register (spi_index_enum spi_n, const uint16 register_name, const uint16 data) {
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_16_BITS);
    spi_transfer_generic(spi_n, &data, NULL, 1, SPI_BIT_WIDTH_16_BITS);
}

void spi_write_16bit_registers (spi_index_enum spi_n, const uint16 register_name, const uint16 *data, uint32 length) {
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_16_BITS);
    spi_transfer_generic(spi_n, data, NULL, length, SPI_BIT_WIDTH_16_BITS);
}

uint8 spi_read_8bit (spi_index_enum spi_n) {
    uint8 data = 0;
    spi_transfer_generic(spi_n, NULL, &data, 1, SPI_BIT_WIDTH_8_BITS);
    return data;
}

void spi_read_8bit_array (spi_index_enum spi_n, uint8 *data, uint32 length) {
    spi_transfer_generic(spi_n, NULL, data, length, SPI_BIT_WIDTH_8_BITS);
}

uint16 spi_read_16bit (spi_index_enum spi_n) {
    uint16 data = 0;
    spi_transfer_generic(spi_n, NULL, &data, 1, SPI_BIT_WIDTH_16_BITS);
    return data;
}

void spi_read_16bit_array (spi_index_enum spi_n, uint16 *data, uint32 length) {
    spi_transfer_generic(spi_n, NULL, data, length, SPI_BIT_WIDTH_16_BITS);
}

uint8 spi_read_8bit_register (spi_index_enum spi_n, const uint8 register_name) {
    uint8 data = 0;
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_8_BITS);
    spi_transfer_generic(spi_n, NULL, &data, 1, SPI_BIT_WIDTH_8_BITS);
    return data;
}

void spi_read_8bit_registers (spi_index_enum spi_n, const uint8 register_name, uint8 *data, uint32 length) {
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_8_BITS);
    spi_transfer_generic(spi_n, NULL, data, length, SPI_BIT_WIDTH_8_BITS);
}

uint16 spi_read_16bit_register (spi_index_enum spi_n, const uint16 register_name) {
    uint16 data = 0;
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_16_BITS);
    spi_transfer_generic(spi_n, NULL, &data, 1, SPI_BIT_WIDTH_16_BITS);
    return data;
}

void spi_read_16bit_registers (spi_index_enum spi_n, const uint16 register_name, uint16 *data, uint32 length) {
    spi_transfer_generic(spi_n, &register_name, NULL, 1, SPI_BIT_WIDTH_16_BITS);
    spi_transfer_generic(spi_n, NULL, data, length, SPI_BIT_WIDTH_16_BITS);
}

void spi_transfer_8bit (spi_index_enum spi_n, const uint8 *write_buffer, uint8 *read_buffer, uint32 length) {
    spi_transfer_generic(spi_n, write_buffer, read_buffer, length, SPI_BIT_WIDTH_8_BITS);
}

void spi_transfer_16bit (spi_index_enum spi_n, const uint16 *write_buffer, uint16 *read_buffer, uint32 length) {
    spi_transfer_generic(spi_n, write_buffer, read_buffer, length, SPI_BIT_WIDTH_16_BITS);
}
