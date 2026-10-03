/*********************************************************************************************************************
* RA8D1 Opensourec Library ����RA8D1 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2025 SEEKFREE ��ɿƼ�
* 
* ���ļ��� RA8D1 ��Դ���һ����
* 
* RA8D1 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
* 
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
* 
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
* 
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          zf_device_mt9v03x
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MDK 5.38
* ����ƽ̨          RA8D1
* ��������          https://seekfree.taobao.com/
* 
* �޸ļ�¼
* ����              ����                ��ע
* 2025-03-25        ZSY            first version
********************************************************************************************************************/
#include "zf_device_mt9v03x.h"
#include "zf_device_config.h"

vuint8  mt9v03x_finish_flag;
volatile uint8_t ceu_error_flag = 0;       // 错误求救标志
uint8 mt9v03x_image[MT9V03X_IMAGE_SIZE] BSP_ALIGN_VARIABLE(64); //BSP_PLACE_IN_SECTION(".bss.uncached");
#define MT9V03X_VSYNC_PIN (BSP_IO_PORT_07_PIN_10)
#define MT9V03X_HSYNC_PIN (BSP_IO_PORT_07_PIN_09)
// ��Ҫ���õ�����ͷ������ �����������޸Ĳ���
static int16 mt9v03x_set_confing_buffer[MT9V03X_CONFIG_FINISH][2]=
{
    {MT9V03X_INIT,              0},                                             // ����ͷ��ʼ��ʼ��

    {MT9V03X_AUTO_EXP,          MT9V03X_AUTO_EXP_DEF},                          // �Զ��ع�����
    {MT9V03X_EXP_TIME,          MT9V03X_EXP_TIME_DEF},                          // �ع�ʱ��
    {MT9V03X_FPS,               MT9V03X_FPS_DEF},                               // ͼ��֡��
    {MT9V03X_SET_COL,           MT9V03X_W},                                     // ͼ��������
    {MT9V03X_SET_ROW,           MT9V03X_H},                                     // ͼ��������
    {MT9V03X_LR_OFFSET,         MT9V03X_LR_OFFSET_DEF},                         // ͼ������ƫ����
    {MT9V03X_UD_OFFSET,         MT9V03X_UD_OFFSET_DEF},                         // ͼ������ƫ����
    {MT9V03X_GAIN,              MT9V03X_GAIN_DEF},                              // ͼ������
    {MT9V03X_PCLK_MODE,         MT9V03X_PCLK_MODE_DEF},                         // ����ʱ��ģʽ
};

// ������ͷ�ڲ���ȡ������������ �����������޸Ĳ���
static int16 mt9v03x_get_confing_buffer[MT9V03X_CONFIG_FINISH - 1][2]=
{
    {MT9V03X_AUTO_EXP,          0},                                             // �Զ��ع�����
    {MT9V03X_EXP_TIME,          0},                                             // �ع�ʱ��
    {MT9V03X_FPS,               0},                                             // ͼ��֡��
    {MT9V03X_SET_COL,           0},                                             // ͼ��������
    {MT9V03X_SET_ROW,           0},                                             // ͼ��������
    {MT9V03X_LR_OFFSET,         0},                                             // ͼ������ƫ����
    {MT9V03X_UD_OFFSET,         0},                                             // ͼ������ƫ����
    {MT9V03X_GAIN,              0},                                             // ͼ������
    {MT9V03X_PCLK_MODE,         0},                                             // ����ʱ��ģʽ���� PCLKģʽ < ������� MT9V034 V1.5 �Լ����ϰ汾֧�ָ����� >
};



void g_ceu_mt9v03x_callback(capture_callback_args_t* p_args)
{
    // 1. 完美抓到一整帧
    int8 debug_str_buff[128];
        // 必须打开这行打印！这是决定生死的事件码！
        //uint32 len = zf_sprintf(debug_str_buff, "\r\n>>> CALLBACK FIRED! EVT: %d <<<\r\n", p_args->event);
       // debug_write_buffer((uint8 *)debug_str_buff, len);
    if ((p_args->event & CEU_EVENT_FRAME_END) == CEU_EVENT_FRAME_END)
    {
        mt9v03x_finish_flag = 1;
    }
    // 2. 抓到首帧垃圾或发生 16777216 同步错误
    else
    {
        ceu_error_flag = 1; // 仅仅举起求救标志，立刻逃离中断！
    }
}
// 模块 1：底层引脚配置（解决 RA8D1 只发不收的冲突）
// ============================================================================
void mt9v03x_pin_config(void) {
    // 核心配置：开启输入(DIRECTION_INPUT) + 开启开漏(NMOS) + 开启上拉(PULLUP)
    // 这是让 gpio_get_level 能读到真实电平的关键
    uint32_t pin_cfg = IOPORT_CFG_PORT_DIRECTION_OUTPUT |
                       IOPORT_CFG_PORT_DIRECTION_INPUT  |
                       IOPORT_CFG_NMOS_ENABLE           |
                       IOPORT_CFG_PULLUP_ENABLE;

    R_IOPORT_PinCfg(&g_ioport_ctrl, MT9V03X_COF_IIC_SCL, pin_cfg);
    R_IOPORT_PinCfg(&g_ioport_ctrl, MT9V03X_COF_IIC_SDA, pin_cfg);
}

// ============================================================================
// 模块 2：总线自救复位（解决 SDA 挂死在 0 的问题）
// ============================================================================
void i2c_bus_release(void) {
    gpio_high(MT9V03X_COF_IIC_SDA); // 释放 SDA
    // 连敲 9 个时钟，让可能卡死的从机释放总线
    for (int i = 0; i < 9; i++) {
        gpio_low(MT9V03X_COF_IIC_SCL);
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
        gpio_high(MT9V03X_COF_IIC_SCL);
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    }
    // 发送一个 Stop 信号清理战场
    gpio_low(MT9V03X_COF_IIC_SDA);
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    gpio_high(MT9V03X_COF_IIC_SCL);
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    gpio_high(MT9V03X_COF_IIC_SDA);
}

// ============================================================================
// 模块 3：逻辑自检（验证 SDA/SCL 是否真的通了）
// ============================================================================
void i2c_logic_check(void) {
    uint8_t buf[64];
    gpio_high(MT9V03X_COF_IIC_SDA);
    uint8_t sda_h = gpio_get_level(MT9V03X_COF_IIC_SDA);
    gpio_low(MT9V03X_COF_IIC_SDA);
    uint8_t sda_l = gpio_get_level(MT9V03X_COF_IIC_SDA);

    zf_sprintf((char *)buf, "[Check] SDA Logic: H=%d, L=%d (Must be 1,0)\r\n", sda_h, sda_l);
    debug_write_buffer(buf, strlen((char *)buf));
}
/***uint8 mt9v03x_init (void)
{
    system_delay_ms(100);
    soft_iic_info_struct mt9v03x_iic_struct;
    soft_iic_init(&mt9v03x_iic_struct, 0, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);
    if(mt9v03x_set_config_sccb(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer))
    {
        return 1;
    }
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);
    g_ceu_mt9v03x.p_api->captureStart(g_ceu_mt9v03x.p_ctrl, (uint8*)mt9v03x_image);
	R_CEU->CAPCR = R_CEU_CAPCR_CTNCP_Msk;
    return 0;
}
***/

/***uint8 mt9v03x_init(void)
{
    uint8_t  id_data = 0;
    uint8_t  debug_buf[128];
    uint8_t  write_buf[2];
    uint8_t  read_back[2];
    soft_iic_info_struct mt9v03x_iic_struct;

    // 1. 【核心：找回你的 IIC 握手】
    soft_iic_init(&mt9v03x_iic_struct, 0x5c, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);

    // 必须有这两步，你的排线才能从高阻态拉回来
    i2c_bus_release();
    i2c_logic_check();

    // 使用你之前调通的 16 位转 8 位 ID 读取逻辑
    uint16_t chip_id = soft_iic_read_16bit_register(&mt9v03x_iic_struct, 0x00);
    if(chip_id == 0x0000 || chip_id == 0xFFFF) {
        id_data = soft_iic_read_8bit_register(&mt9v03x_iic_struct, 0xEF);
    } else {
        id_data = (uint8_t)(chip_id & 0xFF);
    }

    if(id_data == 0x00 || id_data == 0xFF) {
        zf_sprintf((char *)debug_buf, "[MT9V03X] ERROR: Module Not Found! (ID:0x%X)\r\n", id_data);
        debug_write_buffer(debug_buf, strlen((char *)debug_buf));
        return 1; // 这里如果报错，说明 SDA/SCL 还是没通
    }
    zf_sprintf((char *)debug_buf, "[MT9V03X] Found ID: 0x%X. Initializing...\r\n", id_data);
    debug_write_buffer(debug_buf, strlen((char *)debug_buf));

    // 2. 写入配置序列
    mt9v03x_set_config_sccb(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer);

    // 【唤醒三部曲】：强行修正宽度、高度并叫醒摄像头
    write_buf[0] = 0x00; write_buf[1] = 0xB8; // 宽 184
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x04, write_buf, 2);
    write_buf[0] = 0x00; write_buf[1] = 0x78; // 高 120
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x03, write_buf, 2);

    write_buf[0] = 0x01; write_buf[1] = 0x88; // 写 0x07 唤醒
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x07, write_buf, 2);
    system_delay_ms(10);

    // 3. 【RA8D1 寄存器接管】
    // 先 Open 打开时钟
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);

    // 强行 CPKIL 复位，防止 FSP 锁定寄存器
    R_CEU->CAPSR = (1U << 16);
    while(R_CEU->CAPSR & (1U << 16));
    R_CEU->CAPSR = 0;

    // --- 修正宽度和高度的位布局 (184x120) ---
    // CAPWR: [27:16]是宽(B8), [11:0]是高(78) -> 0x00B80078
    R_CEU->CAPWR = 0x00B80078;

    // CFSZR: [27:16]是高(78), [11:0]是宽的一半(5C) -> 0x0078005C
    R_CEU->CFSZR = 0x0078005C;

    R_CEU->CDWDR = 184U;      // 行跨度 184 字节
    R_CEU->CAMCR = 0x00000010; // Data Fetch 模式
    R_CEU->CDOCR = 0x00000010; // 输出使能
    R_CEU->CDAYR = (uint32_t)mt9v03x_image; // 内存地址
    R_CEU->CMCYR = 0;          // 关掉校验
    // 0x00030010: VPO=1, HPO=1 (双低有效)
        // 0x00020010: VPO=1, HPO=0 (VSYNC低有效)
        // 0x00010010: VPO=0, HPO=1 (HSYNC低有效)
        // 0x00000010: VPO=0, HPO=0 (全高有效)
        R_CEU->CAMCR = 0x00000010;
    // 清除一切历史报错标志
    *(uint32_t *)((uint32_t)R_CEU + 0x18) = 0;

    // 4. 【发车】直接写寄存器启动
    // 千万不要在后面再调用 p_api->captureStart 了，否则寄存器会被洗掉！
    R_CEU->CAPSR = 1;

    // 打印验证：此时回读 Reg 07 应该是 0x18B
    soft_iic_read_8bit_registers(&mt9v03x_iic_struct, 0x07, read_back, 2);
    zf_sprintf((int8 *)debug_buf, "\r\n[TRUE_DATA] Reg 07: 0x%X, CAPWR: 0x%X\r\n",
                (read_back[0]<<8|read_back[1]), R_CEU->CAPWR);
    debug_write_buffer(debug_buf, strlen((char *)debug_buf));

    return 0;
}
***/
/***
uint8 mt9v03x_init(void)
{
    uint8_t  id_data = 0;
    uint8_t  debug_buf[128];
    uint8_t  write_buf[2];
    uint8_t  read_back[2];
    soft_iic_info_struct mt9v03x_iic_struct;

    // 1. 【IIC 握手】保留你最稳的逻辑
    soft_iic_init(&mt9v03x_iic_struct, 0x5c, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);
    i2c_bus_release();
    i2c_logic_check();

    uint16_t chip_id = soft_iic_read_16bit_register(&mt9v03x_iic_struct, 0x00);
    if(chip_id == 0x0000 || chip_id == 0xFFFF) {
        id_data = soft_iic_read_8bit_register(&mt9v03x_iic_struct, 0xEF);
    } else {
        id_data = (uint8_t)(chip_id & 0xFF);
    }

    if(id_data == 0x00 || id_data == 0xFF) {
        zf_sprintf((char *)debug_buf, "[MT9V03X] ERROR: Module Not Found!\r\n");
        debug_write_buffer(debug_buf, strlen((char *)debug_buf));
        return 1;
    }

    // 2. 【摄像头唤醒】依据手册 Page 24，确保芯片不待机、不暂停
    mt9v03x_set_config_sccb(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer);



    // 重要：写入 0x0188 唤醒 (之前你的 0x18B 会让芯片进入 Standby)
    write_buf[0] = 0x01; write_buf[1] = 0x88;
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x07, write_buf, 2);
    system_delay_ms(10);

    // 3. 【RA8D1 寄存器接管】彻底解决 Bytes:0 问题
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);

    // 强行 CPKIL 复位 CEU 引擎
    R_CEU->CAPSR = (1U << 16);
    while(R_CEU->CAPSR & (1U << 16));
    R_CEU->CAPSR = 0;



    // --- 核心 B：将 CAMCR 从 0x20 强行改回 0x10 (开启 Data Fetch) ---
    // 如果全高有效不行，请尝试 0x00000014 (反转 VSYNC 极性)
    R_CEU->CAMCR = 0x00000010;

    // --- 核心 C：寄存器参数精确对齐 ---
    R_CEU->CAPWR = 0x00BC0078; // 宽 188, 高 120
    R_CEU->CFSZR = 0x0078002E; // 高 120, 宽/2 = 92 (Data Fetch 模式必须减半)
    R_CEU->CDWDR = 184U;       // 行步长
    R_CEU->CDOCR = 0x00000010; // 开启搬运
    R_CEU->CDAYR = (uint32_t)mt9v03x_image;
    R_CEU->CMCYR = 0;

    // --- 核心 D：处理 D-Cache (防止 Bytes 到了却看不见) ---
    // 告诉内核：这块内存即将被 DMA 强刷，请丢弃当前 Cache 里的旧货
    SCB_InvalidateDCache_by_Addr((uint32_t *)mt9v03x_image, 188 * 120);

    // 4. 【正式点火】
    *(uint32_t *)((uint32_t)R_CEU + 0x18) = 0; // 清除历史报错


    // 打印验证：重点看 CAMCR 是否变回了 0x10
    zf_sprintf((int8 *)debug_buf, "\r\n[FIXED] CAMCR: 0x%X, CAPWR: 0x%X\r\n", R_CEU->CAMCR, R_CEU->CAPWR);
    debug_write_buffer(debug_buf, strlen((char *)debug_buf));
    system_delay_us(20);
    R_CEU->CAPSR = 1;
    return 0;
}
***/
/***
uint8 mt9v03x_init (void)
{
    uint8_t  id_data = 0;
        uint8_t  debug_buf[128];
        uint8_t  write_buf[2];
        uint8_t  read_back[2];
    system_delay_ms(100);
    soft_iic_info_struct mt9v03x_iic_struct;
    soft_iic_init(&mt9v03x_iic_struct, 0, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);
    i2c_bus_release();
    i2c_logic_check();
    if(mt9v03x_set_config_sccb(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer))
    {
        return 1;
    }
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);
    g_ceu_mt9v03x.p_api->captureStart(g_ceu_mt9v03x.p_ctrl, (uint8*)mt9v03x_image);

    R_CEU->CAPSR = (1U << 16);
        while(R_CEU->CAPSR & (1U << 16)); // 等待暂停

        // 改回 Data Fetch 模式 (如果不行，后面可以试 0x12 或 0x14 换极性)
        R_CEU->CAMCR = 0x00000016;

        R_CEU->CAPWR = 0x00B00072;

            // --- Data Fetch 的抓取尺寸 ---
            // VFL = 114 (0x72)
            // HFL = (180字节 / 4字节对齐) - 1 = 44 (0x2C)
            R_CEU->CFSZR = 0x0072002C;
        // 清除刚才可能产生的误报，重新点火
        *(uint32_t *)((uint32_t)R_CEU + 0x18) = 0;
        R_CEU->CAPCR = R_CEU_CAPCR_CTNCP_Msk;
    zf_sprintf((int8 *)debug_buf, "\r\n[FIXED] CAMCR: 0x%X, CAPWR: 0x%X\r\n", R_CEU->CAMCR, R_CEU->CAPWR);
    debug_write_buffer(debug_buf, strlen((char *)debug_buf));
    return 0;
}
***/
/***
uint8 mt9v03x_init(void)
{
    uint8_t  write_buf[2];
    soft_iic_info_struct mt9v03x_iic_struct;

    // 1. IIC 握手与摄像头唤醒
    soft_iic_init(&mt9v03x_iic_struct, 0x5c, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);
    i2c_bus_release();
    i2c_logic_check();

    mt9v03x_set_config_sccb(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer);

    // 写入 0x0188 唤醒
    write_buf[0] = 0x01; write_buf[1] = 0x88;
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x07, write_buf, 2);
    system_delay_ms(10);

    // 2. 借用 FSP 打开电源和时钟
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);

    // 3. 【核弹操作 A】彻底禁用 CEU 的中断，让 FSP 闭嘴
    R_CEU->CEIER = 0x00000000;

    // 软复位引擎
    R_CEU->CAPSR = (1U << 16);
    while(R_CEU->CAPSR & (1U << 16));

    // 强制捅开物理引脚
    *(volatile uint32_t *)&R_PFS->PORT[7].PIN[8]  |= (1 << 14); // PCLK
    *(volatile uint32_t *)&R_PFS->PORT[7].PIN[9]  |= (1 << 14); // HD
    *(volatile uint32_t *)&R_PFS->PORT[7].PIN[10] |= (1 << 14); // VD

    // 4. 【核弹操作 B】进入 Data Fetch 模式，开启黑洞窗口！
    R_CEU->CAMCR = 0x00000014;

    // 写入最大支持尺寸 4096 x 4096
    R_CEU->CAPWR = 0x0FFF0FFF;
    R_CEU->CFSZR = 0x0FFF0FFF;

    R_CEU->CDWDR = 188U; // 步长暂定 188
    R_CEU->CDOCR = 0x00000010;
    R_CEU->CDAYR = (uint32_t)mt9v03x_image;

    // 清除一切历史报错标志
    *(volatile uint32_t *)((uint32_t)R_CEU + 0x18) = 0;

    // 5. 强行点火
    R_CEU->CAPSR = 1;

    return 0;
}
***/
/***
uint8 mt9v03x_init(void)
{
    uint8_t  write_buf[2];
    soft_iic_info_struct mt9v03x_iic_struct;

    // 1. IIC 握手与摄像头配置
    soft_iic_init(&mt9v03x_iic_struct, 0x5c, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);
    i2c_bus_release();
    i2c_logic_check();

    mt9v03x_set_config_sccb(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer);

    // 设置摄像头输出 188x120 (虽然单片机不关心尺寸，但摄像头得发这么多)
    write_buf[0] = 0x00; write_buf[1] = 0xBC; // 188
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x04, write_buf, 2);
    write_buf[0] = 0x00; write_buf[1] = 0x78; // 120
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x03, write_buf, 2);

    // 唤醒摄像头并开启测试图案
    write_buf[0] = 0x01; write_buf[1] = 0x88;
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x07, write_buf, 2);
    write_buf[0] = 0x00; write_buf[1] = 0x01;
    soft_iic_write_8bit_registers(&mt9v03x_iic_struct, 0x70, write_buf, 2);
    system_delay_ms(10);

    // 2. 让 FSP 接管一切
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);

    // ⚠️ 极其重要的一步：在 Data Fetch 模式下，我们要告诉 FSP 需要搬运的总字节数
    // 你的数组是 uint8_t [188*120]，总共 22560 字节。
    // RASC 里配置的尺寸可能是 0，但在调用 captureStart 时，我们需要传入目标地址。
    g_ceu_mt9v03x.p_api->captureStart(g_ceu_mt9v03x.p_ctrl, (uint8_t *)mt9v03x_image);

    // 3. 唯一的微调：只确认模式和极性，其他什么都不动！
    R_CEU->CAPSR = 0;
    while(R_CEU->CAPSR & 1);

    // 钉死 Data Fetch 模式 (0x10)，保持高电平有效
    R_CEU->CAMCR = 0x00000014;

    // 清除一切历史报错标志
    *(volatile uint32_t *)((uint32_t)R_CEU + 0x18) = 0;

    // 重新点火！
    R_CEU->CAPSR = 1;

    return 0;
}
***/
/***
uint8 mt9v03x_init(void)
{
    soft_iic_info_struct mt9v03x_iic_struct;
    uint8_t  id_data = 0;
    uint8_t  debug_buf[128];

    // 1. IIC 握手
    soft_iic_init(&mt9v03x_iic_struct, 0x5c, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);
    i2c_bus_release();
    i2c_logic_check();
    uint16_t chip_id = soft_iic_read_16bit_register(&mt9v03x_iic_struct, 0x00);
        if(chip_id == 0x0000 || chip_id == 0xFFFF) {
            id_data = soft_iic_read_8bit_register(&mt9v03x_iic_struct, 0xEF);
        } else {
            id_data = (uint8_t)(chip_id & 0xFF);
        }

        if(id_data == 0x00 || id_data == 0xFF) {
            zf_sprintf((char *)debug_buf, "[MT9V03X] ERROR: Module Not Found!\r\n");
            debug_write_buffer(debug_buf, strlen((char *)debug_buf));
            return 1;
        }
        soft_iic_write_16bit_register(&mt9v03x_iic_struct, 0x04, 188);

        // 立刻读回来验证！
        uint16_t check_width = soft_iic_read_16bit_register(&mt9v03x_iic_struct, 0x04);

        sprintf((char *)debug_buf, "Current Camera Width is: %x\r\n", check_width);


            // 调用你的串口发送函数
       debug_write_buffer(debug_buf, strlen((char *)debug_buf));
    // 2. 🌟 绝对信任你的原版驱动，不做任何多余的 I2C 写入！
    mt9v03x_set_config_sccb(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer);

    // 稍微延时，等摄像头时钟稳定输出
    system_delay_ms(50);

    // 3. 让 FSP 接管 DMA
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);
    g_ceu_mt9v03x.p_api->captureStart(g_ceu_mt9v03x.p_ctrl, (uint8_t *)mt9v03x_image);

    // 4. 仅修正一次模式和极性 (确保是 0x10)
    R_CEU->CAPSR = 0;
    while(R_CEU->CAPSR & 1);

    R_CEU->CAMCR = 0x00000010; // Data Fetch 模式，VD/HD高有效
    *(volatile uint32_t *)((uint32_t)R_CEU + 0x18) = 0; // 清理历史报错

    R_CEU->CAPSR =  R_CEU_CAPCR_CTNCP_Msk; // 点火！

    return 0;
}
***/
// 提前声明 16位 配置函数，防止编译器不认识
unsigned char mt9v03x_set_config_16bit(void *soft_iic_obj, short int buff[][2]);
uint8 mt9v03x_init(void)
{
    soft_iic_info_struct mt9v03x_iic_struct;
    uint8_t  id_data = 0;
    uint8_t  debug_buf[128];

    // 1. IIC 握手与释放总线
    soft_iic_init(&mt9v03x_iic_struct, 0x5c, MT9V03X_COF_IIC_DELAY, MT9V03X_COF_IIC_SCL, MT9V03X_COF_IIC_SDA);
   // i2c_bus_release();
   // i2c_logic_check();

    // 2. 检查芯片是否存活
   // uint16_t chip_id = mt9v03x_force_read_16bit(&mt9v03x_iic_struct, 0x00);
    //if(chip_id != 0x1324) {
     //   sprintf((char *)debug_buf, "[MT9V03X] ERROR: Module Not Found! Read ID: 0x%X\r\n", chip_id);
      //  debug_write_buffer(debug_buf, strlen((char *)debug_buf));
     //   return 1;
    //}

    // 3. 🌟 写入配置（调用我们刚刚修正的 16 位配置函数！）
    mt9v03x_set_config_16bit(&mt9v03x_iic_struct, mt9v03x_set_confing_buffer);

    // 稍微延时，等待摄像头时钟按 188x120 稳定输出
    //system_delay_ms(50);
    /***
    uint16_t check_hight = mt9v03x_force_read_16bit(&mt9v03x_iic_struct, 0x03);
        sprintf((char *)debug_buf, ">>> Camera Hight is Now: %d (0x%X) <<<\r\n", check_hight, check_hight);
        debug_write_buffer(debug_buf, strlen((char *)debug_buf));
    // 4. 读回宽度，进行终极审判！
    uint16_t check_width = mt9v03x_force_read_16bit(&mt9v03x_iic_struct, 0x04);
    sprintf((char *)debug_buf, ">>> Camera Width is Now: %d (0x%X) <<<\r\n", check_width, check_width);
    debug_write_buffer(debug_buf, strlen((char *)debug_buf));

    uint16_t regth = mt9v03x_force_read_16bit(&mt9v03x_iic_struct, 0x07);
        sprintf((char *)debug_buf, ">>> regth is Now: %d (0x%X) <<<\r\n", regth, regth);
        debug_write_buffer(debug_buf, strlen((char *)debug_buf));
    ***/
    // ==========================================
    // 5. 让 FSP 接管并强行修正 CEU 门框 (尺寸必须对齐 188x120)
    // ==========================================
    g_ceu_mt9v03x.p_api->open(g_ceu_mt9v03x.p_ctrl, g_ceu_mt9v03x.p_cfg);
    g_ceu_mt9v03x.p_api->captureStart(g_ceu_mt9v03x.p_ctrl, (uint8_t *)mt9v03x_image);
    // ---------------- CEU 核心寄存器深度扫描 ----------------
   /*** uint32_t reg_camcr = R_CEU->CAMCR;
    uint32_t reg_capwr = R_CEU->CAPWR;
    uint32_t reg_cfszr = R_CEU->CFSZR;
    uint32_t reg_cdwdr = R_CEU->CDWDR;
    uint32_t reg_cetsr = R_CEU->CETCR; // 事件状态寄存器，最关键！
    uint32_t log_len = 0;

    log_len = (uint32_t)sprintf((char *)debug_buf, "\r\n--- CEU Hardware Snapshot ---\r\n");
    debug_write_buffer((const uint8_t *)debug_buf, log_len);

    // 1. CAMCR: 模式、极性、边沿
    log_len = (uint32_t)sprintf((char *)debug_buf, "CAMCR: 0x%08X (Mode/Pol/Edge)\r\n", reg_camcr);
    debug_write_buffer((const uint8_t *)debug_buf, log_len);

    // 2. CAPWR: 捕获窗口 (官方 Data Fetch 模式下可能为 0)
    log_len = (uint32_t)sprintf((char *)debug_buf, "CAPWR: 0x%08X (Window Size)\r\n", reg_capwr);
    debug_write_buffer((const uint8_t *)debug_buf, log_len);

    // 3. CFSZR: 过滤尺寸
    log_len = (uint32_t)sprintf((char *)debug_buf, "CFSZR: 0x%08X (Fetch Size)\r\n", reg_cfszr);
    debug_write_buffer((const uint8_t *)debug_buf, log_len);

    // 4. CDWDR: 内存宽度 (Stride)
    log_len = (uint32_t)sprintf((char *)debug_buf, "CDWDR: 0x%08X (Memory Width)\r\n", reg_cdwdr);
    debug_write_buffer((const uint8_t *)debug_buf, log_len);

    // 5. CETCR: 错误状态记录器
    log_len = (uint32_t)sprintf((char *)debug_buf, "CETCR: 0x%08X (Error Cause)\r\n", reg_cetsr);
    debug_write_buffer((const uint8_t *)debug_buf, log_len);

    log_len = (uint32_t)sprintf((char *)debug_buf, "------------------------------\r\n");
    debug_write_buffer((const uint8_t *)debug_buf, log_len);
    ***/
    /***
    // 4. 仅修正一次模式和极性 (确保是 0x10)
       R_CEU->CAPSR = 0;
       while(R_CEU->CAPSR & 1);

       R_CEU->CAMCR = 0x00000010; // Data Fetch 模式，VD/HD高有效
       *(volatile uint32_t *)((uint32_t)R_CEU + 0x18) = 0; // 清理历史报错
    ***/
       R_CEU->CAPCR =  R_CEU_CAPCR_CTNCP_Msk; // 点火！

    return 0;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     通过 SCCB 协议连续写入 MT9V03X 摄像头的配置寄存器
// 参数说明     *soft_iic_obj   软件 IIC 句柄
// 参数说明     buff[][2]       配置数组，格式为 {寄存器地址, 寄存器数据}
// 返回参数     unsigned char   返回 0 表示成功
//-------------------------------------------------------------------------------------------------------------------
unsigned char mt9v03x_set_config_sccb(void *soft_iic_obj, short int buff[][2])
{
    uint32_t i = 0;
    
    // 逐飞科技的摄像头配置数组，通常以特定标志（如 -1 或 0xFFFF，或根据数组长度）作为结尾
    // 这里我们遍历数组，只要第一位（寄存器地址）不是 -1 就继续写入
    while (buff[i][0] != -1) 
    {
        // 调用底层 zf_driver_soft_iic.c 中的单字节写入函数
        soft_iic_sccb_write_register((soft_iic_info_struct *)soft_iic_obj, (uint8_t)buff[i][0], (uint8_t)buff[i][1]);
        
        // 适当延时，等待摄像头内部将寄存器配置落盘（很重要，否则容易丢配置）
        // 延时函数如果是别的名字，请替换为你工程里的毫秒延时，比如 R_BSP_SoftwareDelay
        system_delay_ms(1); 
        
        i++;
    }
    
    return 0;
}
// 专为 MT9V034 打造的 16 位配置写入函数！
unsigned char mt9v03x_set_config_16bit(void *soft_iic_obj, short int buff[][2])
{
    uint8_t  reg_addr;
    uint16_t reg_val;

    for (uint32_t i = 0; i < MT9V03X_CONFIG_FINISH; i++)
    {
        uint8_t  cmd = (uint8_t)buff[i][0];
        uint16_t val = (uint16_t)buff[i][1];

        switch(cmd)
        {
            case MT9V03X_INIT:
                // 🌟 修正：复位必须有始有终！写1复位，再写0释放
                mt9v03x_force_write_16bit(soft_iic_obj, 0x0C, 0x0001);
                system_delay_ms(10);
                mt9v03x_force_write_16bit(soft_iic_obj, 0x0C, 0x0000);
                system_delay_ms(10);
                continue; // 已经处理完了，跳过本次循环

            case MT9V03X_AUTO_EXP:  reg_addr = 0xAF; reg_val = (val > 0) ? 0x0003 : 0x0000; break;
            case MT9V03X_EXP_TIME:  reg_addr = 0x0B; reg_val = val; break;
            case MT9V03X_FPS:       reg_addr = 0x05; reg_val = 50;  break; // 默认水平消隐
            case MT9V03X_SET_COL:   reg_addr = 0x04; reg_val = val; break;
            case MT9V03X_SET_ROW:   reg_addr = 0x03; reg_val = val; break; // 120 进 0x03
            case MT9V03X_LR_OFFSET: reg_addr = 0x01; reg_val = 1;   break;
            case MT9V03X_UD_OFFSET: reg_addr = 0x02; reg_val = 4;   break;
            case MT9V03X_GAIN:      reg_addr = 0x35; reg_val = val; break;

            case MT9V03X_PCLK_MODE:
                // 🌟 核心：结合你的注释和 RA8D1 的需求
                // 我们使用 0x018E 作为基础(唤醒+输出使能)，根据 val 决定是否加 Bit 10
                reg_addr = 0x07;
                //reg_val = (val == 1) ? (0x018E | 0x0400) : 0x018E;
                reg_val =0x0188;
                break;

            default: continue;
        }

        mt9v03x_force_write_16bit(soft_iic_obj, reg_addr, reg_val);
        system_delay_ms(2);
    }
    return 0;
}
void ceu_start_data_fetch(void)
{
    // 1. 让 FSP 建好 DMA 通道（此时它会按配置把硬件尺寸设为 0）
    g_ceu_mt9v03x.p_api->captureStart(g_ceu_mt9v03x.p_ctrl, (uint8_t *)mt9v03x_image);

    // 2. 趁数据还没来，立刻软暂停，准备修改硬件寄存器
    R_CEU->CAPSR = 0;
    while(R_CEU->CAPSR & 1);

    // 3. 钉死你的 Data Fetch 模式 (绝对正确)
    R_CEU->CAMCR = 0x00000010;

    // 4. 🌟 欺骗硬件：填入 184x120 的绝对物理换算值
    // CAPWR (Capture Window): HWD = 184-4 = 180 (0xB4), VWD = 120-2 = 118 (0x76)
    R_CEU->CAPWR = 0x00BC0078;

    // CFSZR (Fetch Size): HFL = (184字节/4)-1 = 45 (0x2D), VFL = 120-2 = 118 (0x76)
    R_CEU->CFSZR = 0x0077002E;

    // 5. 清除那个恶心的 16777216 历史错误
    *(volatile uint32_t *)((uint32_t)R_CEU + 0x18) = 0;

    // 6. 正式点火，大门敞开！
    R_CEU->CAPSR = 1;
}
