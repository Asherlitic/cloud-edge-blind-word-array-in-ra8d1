/*********************************************************************************************************************
* RA8D1 Opensourec Library                   (RA8D1 开源库)
* Copyright (c) 2025 SEEKFREE 逐飞科技
*
* 文件名称          hal_entry
* 公司名称          成都逐飞科技有限公司
* 修改记录
* 日期              作�?               备注
* 2025-03-25        ZSY            first version
* 2025-06-12        Updated         Dual-mode state machine (M1 OCR/gesture + M2 voice dialogue)
* 2025-06-12        Refactor        UART3 �?wifi_uart (UART4) for STM32 communication
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "zf_device_wifi_spi.h"
#include "zf_device_wifi_uart.h"
#include <string.h>
#include "wait_audio.h"

// wifi_uart.c 中定义的 UART4 实例（const），用于直接硬件发送（绕过 WiFi 协议栈）
extern const uart_instance_t wifi_uart4;
// wifi_uart.c 中的发送完成标志（已在 wifi_uart.c 中改为非 static�?
extern volatile bool uart_send_complete_flag;
// 简�?UART4 硬件写入的宏（去�?const 以兼�?p_api->write 签名�?
#define U4 ((uart_instance_t *)&wifi_uart4)
// �?修复：等待发送完成，避免连续 write 互相覆盖
#define UART4_WRITE(buf, len)                                     \
    do {                                                          \
        uart_send_complete_flag = false;                          \
        U4->p_api->write(U4->p_ctrl, (uint8_t const *)(buf), (uint32_t)(len)); \
        while(!uart_send_complete_flag);                          \
    } while(0)

// ==================== WiFi 配置（请填写你自己的网络参数） ====================
// RA8D1 与树莓派须处于同一局域网；树莓派上运行 TCP Server 监听 TARGET_PORT
#define WIFI_SSID_TEST          "YOUR_WIFI_SSID"
#define WIFI_PASSWORD_TEST      "YOUR_WIFI_PASSWORD"
#define TARGET_IP               "192.168.1.100"   // 树莓派（边缘网关）的局域网 IP
#define TARGET_PORT             "8888"            // 树莓派 TCP Server 端口
#define LOCAL_PORT              "8080"            // RA8D1 本地端口

// ==================== 帧协议标签（与树莓派约定�?====================
#define TAG_IMG_OCR     "IMOC"   // 4字节
#define TAG_IMG_GESTURE "IMG_"   // 4字节
#define TAG_IMG_MULTI   "IMMU"   // 4字节
#define TAG_AUDIO_VOICE "AUDI"   // 4字节
#define TAG_TTS_DATA    "TTSD"   // 4字节
#define TAG_TEXT_MSG    "TEXT"   // 4字节
#define TAG_CMD_PIC     "PICR"   // 4字节
#define TAG_RES_GESTURE "RGES"   // 4字节

// ==================== 大模�?�?状态机 ====================
typedef enum {
    MODE_PROCESS = 0,    // 端侧处理（OCR / 手势�?
    MODE_CLOUD           // 云端交互（语音对话）
} big_mode_t;

typedef enum {
    /* 通用空闲 */
    IDLE,

    /* 模式一：端侧处�?*/
    M1_OCR_SNAP,          // 等拍�?�?�?IMG_OCR �?等结�?
    M1_OCR_ACT,           // 收到 TEXT_MSG �?�?STM32 �?等完�?
    M1_GESTURE_SNAP,      // 等拍�?�?�?IMG_GEST �?等结�?
    M1_GESTURE_ACT,       // 收到 RES_GESTURE �?�?STM32 �?等完�?

    /* 模式二：云端交互 */
    M2_RECORDING,         // 正在录音（再�?KEY-B 停止�?
    M2_WAIT_CLOUD,        // 已发送音�?�?等回�?
    M2_SNAP_PIC,          // 收到 CMD_PIC �?拍照 �?�?IMG_MULTI
    M2_WAIT_FINAL,        // 等待最�?TTS �?回空�?

    ERROR_STATE
} system_state_t;

// ==================== 全局变量 ====================
uint8 camera_data_head[] = {0xAA, 0x02, 0x40, 0x08,
                            (MT9V03X_W & 0xFF), (MT9V03X_W >> 8),
                            (MT9V03X_H & 0xFF), (MT9V03X_H >> 8)};
uint8 mt9v03x_copy[MT9V03X_W * MT9V03X_H];

static system_state_t g_state = IDLE;
static big_mode_t     g_big_mode = MODE_PROCESS;
static uint8_t g_pic_ready  = 0;
static uint8_t g_frame_sent = 0;

#define RECV_BUF_SIZE    65536
static uint8_t g_recv_buf[RECV_BUF_SIZE];

/* 软件定时器（每约 50 ms + 循环耗时 �?1 tick�?*/
static volatile uint32_t g_sys_tick = 0;

// ==================== 麦克风录音缓冲区 ====================
#define MIC_MAX_SAMPLES  48000
static uint16_t g_mic_buf[MIC_MAX_SAMPLES];
static uint32_t g_mic_count = 0;

// ==================== 辅助函数 ====================

// UART4 打印：直接操�?UART4 硬件发送（绕过 WiFi 协议栈的 AT+CIPSEND�?
// 这样数据会直接从 UART4 TX 引脚输出，可�?USB转TTL 接到电脑查看
static void uart4_print(const char *s)
{
    if (!s) return;
    uint32_t l = 0;
    while (s[l]) l++;
    // 直接调用 UART4 硬件发送，不经�?WiFi 模块�?AT 指令封装
    UART4_WRITE(s, l);
}

// 调试串口打印（原 debug UART�?
static void uart_print(const char *s)
{
    if (!s) return;
    uint32_t l = 0;
    while (s[l]) l++;
    debug_write_buffer((const uint8 *)s, l);
}
// ==================== 本地音频播放 ====================


static void play_wait_audio(void)
{
    uart_print("\r\n[AUDIO] Playing wait prompt...\r\n");
    for (uint32_t i = 0; i < WAIT_AUDIO_NUM_SAMPLES; i++)
    {
        dac_out(g_wait_audio[i]);
        system_delay_us(125);
    }
    uart_print("\r\n[AUDIO] Wait prompt done\r\n");
}
// ==================== 帧协议收�?====================

static void send_tag_frame(const char *tag, const uint8 *payload, uint32 payload_len)
{
    uint8 header[8];
    memcpy(header, tag, 4);
    header[4] = (uint8)((payload_len >> 24) & 0xFF);
    header[5] = (uint8)((payload_len >> 16) & 0xFF);
    header[6] = (uint8)((payload_len >> 8) & 0xFF);
    header[7] = (uint8)(payload_len & 0xFF);
    wifi_spi_send_buffer(header, 8);
    if (payload_len > 0 && payload)
        wifi_spi_send_buffer(payload, payload_len);
}

static void send_pic_with_tag(const char *tag)
{
    uint32_t total = 8 + MT9V03X_IMAGE_SIZE;
    uint8 header[8];
    memcpy(header, tag, 4);
    header[4] = (uint8)((total >> 24) & 0xFF);
    header[5] = (uint8)((total >> 16) & 0xFF);
    header[6] = (uint8)((total >> 8) & 0xFF);
    header[7] = (uint8)(total & 0xFF);
    wifi_spi_send_buffer(header, 8);
    wifi_spi_send_buffer(camera_data_head, 8);
    wifi_spi_send_buffer((const uint8 *)mt9v03x_copy, MT9V03X_IMAGE_SIZE);
}

static int try_recv_frame(uint8 *out_tag, uint8 *out_payload,
                          uint32 *out_len, uint32 max_len)
{
    uint8 hdr[8];
    uint32_t r = wifi_spi_read_buffer(hdr, 4);
    if (r < 4) return -1;
    memcpy(out_tag, hdr, 4);

    r = wifi_spi_read_buffer(hdr, 4);
    if (r < 4) return -1;

    uint32_t plen = ((uint32_t)hdr[0] << 24) | ((uint32_t)hdr[1] << 16) |
                    ((uint32_t)hdr[2] << 8)  | (uint32_t)hdr[3];
    if (plen > max_len) plen = max_len;

    uint32_t read_done = 0;
    while (read_done < plen)
    {
        uint32_t to_read = plen - read_done;
        if (to_read > WIFI_SPI_RECVIVE_SIZE) to_read = WIFI_SPI_RECVIVE_SIZE;
        uint32_t rr = wifi_spi_read_buffer(out_payload + read_done, to_read);
        if (!rr) break;
        read_done += rr;
    }
    *out_len = read_done;
    return 0;
}

// ==================== STM32 通信（UART4 via wifi_uart�?====================

static void trigger_stm32(const char *command)
{
    uint32_t len = strlen(command);

    // 只通过调试串口打印（避免调试信息也通过 UART4 发给 STM32�?
    uart_print("\r\n[STM32-TX] Sending: ");
    uart_print(command);
    uart_print("\r\n");

    // �?修复：一次性发送整条命令，避免多次 write 的间隙问�?
    // 先发固定前缀（根�?STM32 的协议格式调整）
    //UART4_WRITE("1", 1);
    //UART4_WRITE("AS", 2);
    // 改为直接发送命令本身（去掉调试用的 "1"+"AS"�?
    UART4_WRITE(command, len);
    UART4_WRITE("\r\n", 2);
}

static int wait_stm32_done(uint32 timeout_ms)
{
    (void)timeout_ms;
    // 此函数不再使用，STM32 不回�?DONE
    // 保留定义以兼容旧代码
    return 0;
}

static void tactile_shutter(void)
{
    trigger_stm32("SNAP");
    uart_print("\r\n[TACTILE] Shutter sent\r\n");
}

// ==================== 按键检测（例程 button_down_flag 风格�?====================
//    KEY_A = KEY4——切换大模式
//    KEY_B = KEY3——模式一：OCR / 模式二：录音
//    KEY_C = KEY2——模式一：手�?
#define KEY_A   KEY1
#define KEY_B   KEY2
#define KEY_C   KEY3

static uint8_t btn_a_down = 0;
static uint8_t btn_b_down = 0;
static uint8_t btn_c_down = 0;
// 全局变量区加
static uint32_t m2_wait = 0;
static uint32_t m2f = 0;

// ==================== 主函�?====================
void hal_entry(void)
{
    init_sdram();
    system_delay_ms(100);
    debug_init();
    system_delay_ms(100);

    uart_print("\r\n===== RA8D1 Dual-Mode (UART4 Direct HW) =====\r\n");

    // 初始�?UART4（使�?wifi_uart_init，它内部�?open UART4 硬件�?
    uart_print("\r\n[UART4] Initializing via wifi_uart_init...\r\n");
    int uart4_ret = wifi_uart_init((char *)WIFI_SSID_TEST, (char *)WIFI_PASSWORD_TEST, WIFI_UART_STATION);
    if (uart4_ret == 0)
        uart_print("\r\n[UART4] wifi_uart_init SUCCESS\r\n");
    else
        uart_print("\r\n[UART4] wifi_uart_init FAIL (continue anyway)\r\n");

    // UART4 已初始化完成，后续所有通信直接通过 UART4_WRITE 宏走硬件
    //uart4_print("1\r\n");
    //uart4_print("[UART4] READY\r\n");
    //uart4_print("你好\r\n");

    // WiFi SPI（与树莓派通信�?
    uart_print("\r\nWiFi connecting...\r\n");
    {
        int wifi_retry = 20;
        while (wifi_spi_init((char *)WIFI_SSID_TEST, (char *)WIFI_PASSWORD_TEST) && wifi_retry-- > 0)
        { uart_print("."); system_delay_ms(500); }
        if (wifi_retry <= 0)
            uart_print("\r\n[WARN] wifi_spi_init TIMEOUT, continuing...\r\n");
    }
    {
        int tcp_retry = 20;
        while (wifi_spi_socket_connect("TCP", (char *)TARGET_IP,
                                       (char *)TARGET_PORT, (char *)LOCAL_PORT) && tcp_retry-- > 0)
        { uart_print("\r\nTCP fail, retry..."); system_delay_ms(500); }
        if (tcp_retry <= 0)
            uart_print("\r\n[WARN] TCP connect TIMEOUT, continuing...\r\n");
    }
    uart_print("\r\nWiFi & TCP OK!\r\n");

    // 摄像�?
    mt9v03x_init();
    uart_print("\r\nCamera initialized\r\n");

    // 麦克�?ADC
    adc_init();
    dac_init();
    uart_print("\r\nMIC ADC initialized\r\n");



    uint8_t  tag_buf[4];
    uint32_t recv_len   = 0;
    uint8_t  result_buf[512] = {0};
    uint32_t result_len = 0;

    g_sys_tick = 0;

    system_delay_ms(1000);
    //uart4_print("我是人天天上学看书我是人天天上学看书我是人天天上学看书我是人天天上学看书我是人天天上学看书\r\n");

    // ====== 主循�?======
    while (1)
    {
        g_sys_tick++;

        // 摄像头帧就绪
        if (mt9v03x_finish_flag)
        {
            mt9v03x_finish_flag = 0;
            memcpy(mt9v03x_copy, mt9v03x_image, MT9V03X_IMAGE_SIZE);
            g_pic_ready = 1;
        }

        // 接收树莓派帧
        if (try_recv_frame(tag_buf, g_recv_buf, &recv_len, RECV_BUF_SIZE) == 0)
        {
            if (memcmp(tag_buf, TAG_TEXT_MSG, 4) == 0)
            {
                if (recv_len > 0 && recv_len < sizeof(result_buf))
                {
                    memcpy(result_buf, g_recv_buf, recv_len);
                    result_buf[recv_len] = '\0';
                    result_len = recv_len;
                    uart_print("\r\n[TEXT_MSG] ");
                    uart_print((char *)result_buf);
                    uart_print("\r\n");
                }
                if (g_state == M1_OCR_SNAP)
                {
                    uart_print("\r\n-> M1_OCR_ACT\r\n");
                    g_state = M1_OCR_ACT;
                }
            }
            else if (memcmp(tag_buf, TAG_RES_GESTURE, 4) == 0)
            {
                if (recv_len > 0 && recv_len < sizeof(result_buf))
                {
                    memcpy(result_buf, g_recv_buf, recv_len);
                    result_buf[recv_len] = '\0';
                    result_len = recv_len;
                    uart_print("\r\n[GESTURE] ");
                    uart_print((char *)result_buf);
                    uart_print("\r\n");
                }
                if (g_state == M1_GESTURE_SNAP)
                {
                    uart_print("\r\n-> M1_GESTURE_ACT\r\n");
                    g_state = M1_GESTURE_ACT;
                }
            }
            else if (memcmp(tag_buf, TAG_TTS_DATA, 4) == 0)
            {
                uart_print("\r\n[TTS_AUDIO] received, playing...\r\n");
                if (g_state == M2_WAIT_CLOUD || g_state == M2_WAIT_FINAL)
                {
                    uint32_t samples = recv_len;
                    for (uint32_t i = 0; i < samples; i++)
                    {
                        uint16_t dac_val = (uint16_t)g_recv_buf[i] * 16;
                        dac_out(dac_val);
                        system_delay_us(125);
                    }
                    uart_print("\r\n[M2] TTS play done, back to IDLE\r\n");
                    g_state = IDLE;
                }
            }
            else if (memcmp(tag_buf, TAG_CMD_PIC, 4) == 0)
            {
                uart_print("\r\n[CMD_PIC] Pi wants photo\r\n");
                if (g_state == M2_WAIT_CLOUD)
                    g_state = M2_SNAP_PIC;
            }
        }

        // 按键检�?
        uint8_t key_a = 0, key_b = 0, key_c = 0;

        if (!gpio_get_level(KEY_A) && !btn_a_down)
        { btn_a_down = 1; key_a = 1; system_delay_ms(200); }
        if (gpio_get_level(KEY_A)) btn_a_down = 0;

        if (!gpio_get_level(KEY_B) && !btn_b_down)
        { btn_b_down = 1; key_b = 1; system_delay_ms(200); }
        if (gpio_get_level(KEY_B)) btn_b_down = 0;

        if (!gpio_get_level(KEY_C) && !btn_c_down)
        { btn_c_down = 1; key_c = 1; system_delay_ms(200); }
        if (gpio_get_level(KEY_C)) btn_c_down = 0;

        // KEY-A：切换大模式
        if (key_a)
        {
            g_big_mode = (g_big_mode + 1) % 2;
            g_state = IDLE;
            if (g_big_mode == MODE_PROCESS){
                uart_print("\r\n[SWITCH] MODE: PROCESS (OCR/Gesture)\r\n");
                trigger_stm32("ocr");}
            else
            { uart_print("\r\n[SWITCH] MODE: CLOUD (Voice)\r\n");
                trigger_stm32("audio");}
        }

        // 状态机
        switch (g_state)
        {
            case IDLE:
                if (g_big_mode == MODE_PROCESS)
                {
                    if (key_b)
                    {
                        uart_print("\r\n[M1] OCR mode\r\n");
                        system_delay_ms(500);
                        g_frame_sent = 0;
                        trigger_stm32("ocr");
                        g_state = M1_OCR_SNAP;
                    }
                    else if (key_c)
                    {
                        uart_print("\r\n[M1] Gesture mode\r\n");
                        system_delay_ms(500);
                        g_pic_ready  = 0;
                        g_frame_sent = 0;
                        trigger_stm32("gesture");
                        g_state = M1_GESTURE_SNAP;
                    }
                }
                else
                {
                    if (key_b)
                    {
                        uart_print("\r\n[M2] Record start (KEY-B again to stop)\r\n");
                        g_mic_count = 0;
                        trigger_stm32("audio");
                        g_state = M2_RECORDING;
                    }
                }
                break;

            case M1_OCR_SNAP:
                if (g_pic_ready && !g_frame_sent)
                {
                    g_frame_sent = 1;
                    uart_print("\r\n[M1] Sending IMG_OCR...\r\n");
                    send_pic_with_tag(TAG_IMG_OCR);
                    uart_print("\r\n[M1] Wait result...\r\n");
                }
                break;

            case M1_OCR_ACT:
                uart_print("\r\n[M1] OCR -> STM32\r\n");
                if (result_len > 0 && result_buf[0] != '\0')
                {
                    char *text_to_send = (char *)result_buf;
                    char ocr_text[256] = {0};
                    char *text_start = strstr((char *)result_buf, "\"texts\":");
                    if (text_start)
                    {
                        char *arr_start = strchr(text_start + 8, '[');
                        if (arr_start)
                        {
                            char *quote1 = strchr(arr_start + 1, '"');
                            if (quote1)
                            {
                                char *quote2 = strchr(quote1 + 1, '"');
                                if (quote2)
                                {
                                    uint32_t txt_len = (uint32_t)(quote2 - quote1 - 1);
                                    if (txt_len > 0 && txt_len < sizeof(ocr_text) - 1)
                                    {
                                        memcpy(ocr_text, quote1 + 1, txt_len);

                                        ocr_text[txt_len] = '\0';
                                        text_to_send = ocr_text;
                                    }
                                }
                            }
                        }
                    }
                    trigger_stm32(text_to_send);
                    uart_print("\r\n[M1] No DONE check (STM32 prints text)\r\n");
                }
                else
                {
                    uart_print("\r\n[M1] Empty result, skip STM32\r\n");
                }
                g_state = IDLE;
                break;

            case M1_GESTURE_SNAP:
                if (g_pic_ready && !g_frame_sent)
                {
                    g_frame_sent = 1;
                    uart_print("\r\n[M1] Sending IMG_GEST...\r\n");
                    send_pic_with_tag(TAG_IMG_GESTURE);
                    uart_print("\r\n[M1] Wait result...\r\n");
                }
                break;

            case M1_GESTURE_ACT:
                uart_print("\r\n[M1] Gesture -> STM32\r\n");
                if (result_len >= 2 && result_buf[0] != '\0')
                {
                    char *label_key = strstr((char *)result_buf, "\"gesture_label\":");
                    if (label_key)
                    {
                        char *quote1 = strchr(label_key + 16, '"');
                        if (quote1)
                        {
                            char *quote2 = strchr(quote1 + 1, '"');
                            if (quote2)
                            {
                                uint32_t label_len = (uint32_t)(quote2 - quote1 - 1);
                                if (label_len > 0 && label_len < 32)
                                {
                                    char gesture_label[32];
                                    memcpy(gesture_label, quote1 + 1, label_len);
                                    gesture_label[label_len] = '\0';
                                    const char *cmd = NULL;
                                    if (strcmp(gesture_label, "rock") == 0) cmd = "1";
                                    else if (strcmp(gesture_label, "paper") == 0) cmd = "2";
                                    else if (strcmp(gesture_label, "scissors") == 0) cmd = "3";
                                    else if (strcmp(gesture_label, "_unknown_") == 0) cmd = "0";
                                    else cmd = gesture_label;
                                    if (cmd) trigger_stm32(cmd);
                                }
                            }
                        }
                    }
                    else
                    {
                        trigger_stm32((char *)result_buf);
                    }
                    uart_print("\r\n[M1] Gesture sent to STM32\r\n");
                }
                else
                {
                    uart_print("\r\n[M1] Empty result, skip STM32\r\n");
                }
                g_state = IDLE;
                break;

            case M2_RECORDING:
                uart_print("\r\n[M2] Recording in progress... (Press KEY-B to stop)\r\n");
                while (g_mic_count < MIC_MAX_SAMPLES)
                {
                    g_mic_buf[g_mic_count++] = adc_get_mic();
                    system_delay_us(125);
                    if ((g_mic_count % 64) == 0)
                    {
                        if (!gpio_get_level(KEY_B))
                        {
                            system_delay_ms(20);
                            if (!gpio_get_level(KEY_B))
                            {
                                while(!gpio_get_level(KEY_B));
                                btn_b_down = 0;
                                break;
                            }
                        }
                    }
                }
                if (g_mic_count > 4000)
                {
                    uart_print("\r\n[M2] Record stop, sending...\r\n");
                    send_tag_frame(TAG_AUDIO_VOICE, (uint8 *)g_mic_buf,
                                   g_mic_count * sizeof(uint16_t));
                    uart_print("\r\n[M2] Wait reply...\r\n");
                    // �?播放"正在等待云端答复"提示�?
                    play_wait_audio();
                    g_state = M2_WAIT_CLOUD;
                }
                else
                {
                    uart_print("\r\n[M2] Too short, canceled\r\n");
                    g_state = IDLE;
                }
                break;

            case M2_WAIT_CLOUD:
                {
                    m2_wait = 0;
                    if (m2_wait == 0) m2_wait = g_sys_tick;
                    if ((g_sys_tick - m2_wait) > 600)
                    {
                        uart_print("\r\n[M2] Cloud timeout\r\n");
                        m2_wait = 0;
                        g_state = IDLE;
                    }
                }
                break;

            case M2_SNAP_PIC:
                uart_print("\r\n[M2] Taking photo...\r\n");
                system_delay_ms(1000);
                g_pic_ready = 0;
                {
                    uint32_t wp = 0;
                    while (!g_pic_ready && wp < 5000)
                    {
                        if (mt9v03x_finish_flag)
                        {
                            mt9v03x_finish_flag = 0;
                            memcpy(mt9v03x_copy, mt9v03x_image, MT9V03X_IMAGE_SIZE);
                            g_pic_ready = 1;
                        }
                        system_delay_ms(10);
                        wp += 10;
                    }
                }
                if (g_pic_ready)
                {
                    uart_print("\r\n[M2] Sending IMG_MULTI...\r\n");
                    send_pic_with_tag(TAG_IMG_MULTI);
                    uart_print("\r\n[M2] Wait final...\r\n");
                    g_state = M2_WAIT_FINAL;
                }
                else
                {
                    uart_print("\r\n[M2] Camera timeout\r\n");
                    g_state = IDLE;
                }
                break;

            case M2_WAIT_FINAL:
                {
                    m2f = 0;
                    if (m2f == 0) m2f = g_sys_tick;
                    if ((g_sys_tick - m2f) > 600)
                    {
                        uart_print("\r\n[M2] Final timeout\r\n");
                        m2f = 0;
                        g_state = IDLE;
                    }
                }
                break;

            default:
                g_state = IDLE;
                break;
        }

        system_delay_ms(50);
    }
}
