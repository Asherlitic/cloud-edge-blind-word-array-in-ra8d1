/*********************************************************************************************************************
* RA8D1 Opensourec Library ����RA8D1 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2025 SEEKFREE ��ɿƼ�
* 
* ���ļ��� RA8D1 ��Դ���һ����
* 
* RA8D1 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
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
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          zf_device_wifi_uart
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
#ifndef _zf_device_wifi_uart_h_
#define _zf_device_wifi_uart_h_

#include "zf_common_headfile.h"
#include "hal_data.h"
#define WIFI_UART_INDEX         &wifi_uart4
#define WIFI_UART_BAUD          (1500000)                                        // ģ�鹤��������
  
#define WIFI_UART_RTS_PIN       (BSP_IO_PORT_02_PIN_06)                                                // ����ʹ��Ӳ����λ���� �������׳��ֵ�Ƭ����λ���޷�������ʼ��ģ��
#define WIFI_UART_RST_PIN       (BSP_IO_PORT_02_PIN_07)

#define WIFI_UART_BUFFER_SIZE   (2048)                                           // ������ջ�������С

#define WIFI_UART_AUTO_CONNECT  (0)                                             // �����Ƿ��ʼ��ʱ����TCP����UDP����    0-������  1-�Զ�����TCP������  2-�Զ�����UDP������  3���Զ�����TCP������

#define MAX_ROUND_WIFI_COUNT 16

#if     (WIFI_UART_AUTO_CONNECT > 3)
#error "WIFI_UART_AUTO_CONNECT ��ֵֻ��Ϊ [0,1,2,3]"
#else
#define WIFI_UART_TARGET_IP     "192.168.2.140"                                  // ����Ŀ��� IP
#define WIFI_UART_TARGET_PORT   "8086"                                          // ����Ŀ��Ķ˿�
#define WIFI_UART_LOCAL_PORT    "8086"                                          // �����˿�
#endif

typedef enum
{
    WIFI_UART_STATION,                                                          // �豸ģʽ
    WIFI_UART_SOFTAP,                                                           // APģʽ
}wifi_uart_mode_enum;

typedef enum
{
    WIFI_UART_COMMAND,                                                          // ʹ������ķ�ʽ��������
    WIFI_UART_SERIANET,                                                         // ʹ��͸���ķ�ʽ��������
}wifi_uart_transfer_mode_enum;

typedef enum
{
    WIFI_UART_TCP_CLIENT,                                                       // ģ������TCP������
    WIFI_UART_TCP_SERVER,                                                       // ģ����ΪTCP������
    WIFI_UART_UDP_CLIENT,                                                       // ģ������UDP����
}wifi_uart_connect_mode_enum;

typedef enum
{
    WIFI_UART_SERVER_OFF,                                                       // ģ��δ���ӷ�����
    WIFI_UART_SERVER_ON,                                                        // ģ���Ѿ����ӷ�����
}wifi_uart_connect_state_enum;

typedef enum
{
    WIFI_UART_LINK_0,                                                           // ģ�鵱ǰ���� 0
    WIFI_UART_LINK_1,                                                           // ģ�鵱ǰ���� 1
    WIFI_UART_LINK_2,                                                           // ģ�鵱ǰ���� 2
    WIFI_UART_LINK_3,                                                           // ģ�鵱ǰ���� 3
    WIFI_UART_LINK_4,                                                           // ģ�鵱ǰ���� 4
}wifi_uart_link_id_enum;

typedef struct
{
    uint8_t                           wifi_uart_version[12];                      // �̼��汾         �ַ�����ʽ
    uint8_t                           wifi_uart_mac[20];                          // ���� MAC ��ַ    �ַ�����ʽ
    uint8_t                           wifi_uart_local_ip[17];                     // ���� IP ��ַ     �ַ�����ʽ
    uint8_t                           wifi_uart_local_port[10];                   // �����˿ں�       �ַ�����ʽ
    uint8_t                           wifi_uart_remote_ip[5][17];                 // Զ�� IP ��ַ     �ַ�����ʽ
    wifi_uart_mode_enum             wifi_uart_mode;                             // WIFI ģʽ
    wifi_uart_transfer_mode_enum    wifi_uart_transfer_mode;                    // ��ǰ����ģʽ
    wifi_uart_connect_mode_enum     wifi_uart_connect_mode;                     // ��������ģʽ
    wifi_uart_connect_state_enum    wifi_uart_connect_state;                    // �������������
}wifi_uart_information_struct;

extern wifi_uart_information_struct wifi_uart_information;

uint8_t   wifi_uart_disconnected_wifi         (void);                                                                         // �Ͽ� WIFI ����
uint8_t   wifi_uart_entry_serianet            (void);                                                                         // ��͸��ģʽ
uint8_t   wifi_uart_exit_serianet             (void);                                                                         // �ر�͸��ģʽ

uint8_t   wifi_uart_connect_tcp_servers       (char *ip, char *port, wifi_uart_transfer_mode_enum mode);                      // ���� TCP ����
uint8_t   wifi_uart_connect_udp_client        (char *ip, char *port, char *local_port, wifi_uart_transfer_mode_enum mode);    // ���� UDP ����
uint8_t   wifi_uart_disconnect_link           (void);                                                                         // �Ͽ����� TCP Server ʹ�ñ��ӿڽ���Ͽ���������
uint8_t   wifi_uart_disconnect_link_with_id   (wifi_uart_link_id_enum link_id);                                               // TCP Server �Ͽ�ָ������ TCP/UDP Client �������з�Ӧ

uint8_t   wifi_uart_entry_tcp_servers         (char *port);                                                                   // ���� TCP ������
uint8_t   wifi_uart_exit_tcp_servers          (void);                                                                         // �ر� TCP ������
uint8_t   wifi_uart_tcp_servers_check_link    (void);                                                                         // TCP Server ģʽ�¼�鵱ǰ�������� ����ȡ IP

uint32_t  wifi_uart_send_buffer               (const uint8_t *buff, uint32_t len);                                                // WIFI ģ�����ݷ��ͺ���
uint32_t  wifi_uart_tcp_servers_send_buffer   (const uint8_t *buff, uint32_t len, wifi_uart_link_id_enum id);                     // WIFI ģ����Ϊ TCP Server ָ��Ŀ���豸���ͺ���
uint32_t  wifi_uart_read_buffer               (uint8_t *buff, uint32_t len);                                                      // WIFI ģ�����ݽ��պ���

void      wifi_uart_callback                  (void);                                                                         // WIFI ģ�鴮�ڻص�����
uint8_t   wifi_uart_init                      (char *wifi_ssid, char *pass_word, wifi_uart_mode_enum wifi_mode);              // WIFI ģ���ʼ������

#endif
