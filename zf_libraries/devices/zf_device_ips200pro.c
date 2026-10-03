// /*********************************************************************************************************************
// * RA8D1 Opensourec Library ����RA8D1 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
// * Copyright (c) 2025 SEEKFREE ��ɿƼ�
// * 
// * ���ļ��� RA8D1 ��Դ���һ����
// * 
// * RA8D1 ��Դ�� ���������
// * �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
// * �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
// * 
// * ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
// * ����û�������������Ի��ʺ��ض���;�ı�֤
// * ����ϸ����μ� GPL
// * 
// * ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
// * ���û�У������<https://www.gnu.org/licenses/>
// * 
// * ����ע����
// * ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
// * ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
// * ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
// * ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
// * 
// * �ļ�����          zf_device_ips200pro
// * ��˾����          �ɶ���ɿƼ����޹�˾
// * �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
// * ��������          MDK 5.38
// * ����ƽ̨          RA8D1
// * ��������          https://seekfree.taobao.com/
// * 
// * �޸ļ�¼
// * ����              ����                ��ע
// * 2025-03-25        ZSY            first version
// ********************************************************************************************************************/

// #include "zf_device_ips200pro.h"

// #define IPS200PRO_SPI_LENGTH    ( 4096 )    // ÿ��SPIͨѶ��󳤶� �����޸�

// // �����޸�
// #define MAX_ID_PAGE             ( 10 )
// #define MAX_ID_LABEL            ( 50 )
// #define MAX_ID_TABLE            ( 20 )
// #define MAX_ID_METER            ( 2  )
// #define MAX_ID_CLOCK            ( 1  )
// #define MAX_ID_PROGRESS_BAR     ( 10 )
// #define MAX_ID_CALENDAR         ( 1  )
// #define MAX_ID_WAVEFORM         ( 5  )
// #define MAX_ID_IMAGE            ( 5  )
// #define MAX_ID_IMAGE_LINE       ( 10 )
// #define MAX_ID_IMAGE_RECTANGLE  ( 5  )

// ips200pro_information_struct    ips200pro_information;
// ips200pro_time_struct           ips200pro_time;
// static uint8                    ips200pro_page_num = 0;
// static char                     ips200pro_printf_buffer[51];
// typedef enum
// {
//     IPS200PRO_PARAMETER_SET    		= 0x01,	// ����ϵͳ��������
//     IPS200PRO_PARAMETER_GET,                // ��ȡϵͳ��������
//     IPS200PRO_WIDGETS_PAGE     		= 0x10,	// ҳ�����
//     IPS200PRO_WIDGETS_LABEL,                // �ı���ǩ���
//     IPS200PRO_WIDGETS_TABLE,                // �������
//     IPS200PRO_WIDGETS_METER,                // �Ǳ����
//     IPS200PRO_WIDGETS_CLOCK,                // ʱ�����
//     IPS200PRO_WIDGETS_BAR,                  // ���������
//     IPS200PRO_WIDGETS_CALENDAR,             // �������
//     IPS200PRO_WIDGETS_WAVEFORM,             // �������
//     IPS200PRO_WIDGETS_IMAGE,                // ͼ�����
//     IPS200PRO_WIDGETS_CONTAINER,            // �������
//     IPS200PRO_WIDGETS_MAX,                  // ռλʹ��
// }ips200pro_command1_enum;

// // ϵͳ����������������Ļ�����������
// typedef enum
// {
//     // �ɶ���д�Ĳ���
//     IPS200PRO_SYSTEM_DATE            = 0x01,// ϵͳ����
//     IPS200PRO_SYSTEM_TIME,         	        // ϵͳʱ��
//     IPS200PRO_SYSTEM_PARENT,               	// ������
//     IPS200PRO_SYSTEM_CODED_FORMAT,         	// �����ʽ
//     IPS200PRO_SYSTEM_BACKLIGHT,            	// ��������
//     IPS200PRO_SYSTEM_DIRECTION,            	// ��Ļ��ʾ����
//     IPS200PRO_SYSTEM_CRC_STATE,            	// CRCʹ��״̬
//     IPS200PRO_SYSTEM_FONT_SIZE,            	// ȫ������
//     SCREEN_SYSTEM_OPTIMIZE,                 // �Ż� Ŀǰ�����ͼ������Ż�(Ĭ���Ż�����)
//     IPS200PRO_SYSTEM_THEME,                	// ϵͳ����
//     IPS200PRO_SYSTEM_SET_MAX,              	// ռλʹ��

//     // ���ɶ��Ĳ���
//     IPS200PRO_SYSTEM_INFORMATION   	= 0x10, // ��ĻID��š��ֱ��ʡ��̼��汾
//     IPS200PRO_SYSTEM_FREE_STACK,           	// ϵͳ����ջ��С
//     IPS200PRO_SYSTEM_GET_MAX,              	// ռλʹ��

//     // ͨ�ò�������
//     IPS200PRO_COMMON_CREATE        	= 0x01, // �������
//     IPS200PRO_COMMON_DELETE,               	// ���ɾ��
//     IPS200PRO_COMMON_FONT_SIZE,            	// ��������С
//     IPS200PRO_COMMON_COLOR,                	// �����ɫ
//     IPS200PRO_COMMON_VALUE,                	// �����ֵ ��ͬ����������Ͳ�ͬ
//     IPS200PRO_COMMON_POSITION,             	// ���λ��
//     IPS200PRO_COMMON_HIDDEN,               	// �������
//     IPS200PRO_COMMON_MAX,                  	// ռλ

//     // PAGE���ר������
//     IPS200PRO_PAGE_SWITCH          	= 0x10, // �л�ҳ��
//     IPS200PRO_PAGE_TITLE,                  	// ����ҳ�������ʾλ�������
//     IPS200PRO_PAGE_MAX,

//     // LABEL���ר������
//     IPS200PRO_LABEL_LONG_MODE      	= 0x10, // ���ı�ģʽ
//     IPS200PRO_LABEL_MAX,

//     // TABLE���ר������
//     IPS200PRO_TABLE_COL_WIDTH      	= 0x10, // �����п�������  �и�����Ļ�Զ��������޷�����
//     IPS200PRO_TABLE_SELECT,                	// �����е�Ԫ��ѡ��
//     IPS200PRO_TABLE_MAX,

//     // WAVEFORM���ר������
//     IPS200PRO_WAVEFORM_LINE_STATE  	= 0x10, // ����ָ������
//     IPS200PRO_WAVEFORM_LINE_TYPE,          	// �������������������
//     IPS200PRO_WAVEFORM_CLEAR,              	// �������������
//     IPS200PRO_WAVEFORM_MAX,

//     // IMAGE���ר������
//     IPS200PRO_IMAGE_DRAW_LINE      	= 0x10, // ͼ����
//     IPS200PRO_IMAGE_DRAW_RECTANGLE,        	// ͼ�񻭿�
//     IPS200PRO_IMAGE_MAX,
// }ips200pro_command2_enum;


// // ����ʹ�ú궨��ķ�ʽ��Ŀ����Ϊ�˱���ʹ�������ṹ�壬���²���IDE�޷����ߵ��Ե�ʱ��鿴�����ṹ�������
// #define IPS200PRO_HEADER   \
//     uint8   command1;                   	/* ����1     */ \
//     uint8   command2;                   	/* ����2     */ \
//     uint8   check_crc8;                 	/* CRCУ�顢Ĭ��δ����   */ \
//     uint8   widgets_id;                 	/* ID���    */ \
//     uint32  length                      	/* ���ݰ����� */ \

// typedef struct
// {
//     IPS200PRO_HEADER;
// }ips200pro_header_struct;

// // ���ݲ��������
// typedef union
// {
//     int8    int8_data[2];               	// �з����ֽ�����
//     uint8   uint8_data[2];              	// �޷����ֽ�����
//     int16   int16_data;                 	// �з��Ű�������
//     uint16  uint16_data;                	// �޷��Ű�������
// }data_split_union;


// // ͨ�ýṹ��
// #define IPS200PRO_COMMON_STRUCT(name, num) \
//     struct common_packet\
//     {\
//         IPS200PRO_HEADER;  \
//         data_split_union  data[num]; \
//     }name;

// #define ips200pro_write_8bit_data_spi_array(data, len)                 (spi_write_8bit_array(IPS200PRO_SPI_INDEX, (data), (len)))
// #define ips200pro_transfer_8bit_data_spi_array(tx_data, rx_data, len)  (spi_transfer_8bit(IPS200PRO_SPI_INDEX, (tx_data), (rx_data), (len)))

    
// volatile int wifi_spi_flag = false;
// void spi2_callback (spi_callback_args_t *p_args)
// {
//     if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
//     {
//         wifi_spi_flag = true;
//     }
// }    

// static void spi2_time_out (void)
// {
//     while(!wifi_spi_flag)
//     {
//     }
// }

// //-------------------------------------------------------------------------------------------------------------------
// // �������     ��Ļ�ȴ�����
// // ����˵��     wait_time       �ȴ�ʱ��
// // ���ز���     uint8           1����ʱ�˳� 0��δ��ʱ
// // ʹ��ʾ��
// // ��ע��Ϣ     �ڲ�ʹ�ã��û��������
// //-------------------------------------------------------------------------------------------------------------------
// static uint8 ips200pro_wait_idle (uint32 wait_time)
// {
//     wait_time = wait_time * 100;

//     while(0 == gpio_get_level(IPS200PRO_INT_PIN) && (0 != wait_time))
//     {
//         func_soft_delay(1000);
//         wait_time--;
//     }
//     return (!wait_time);
// }

// #if(1 == IPS200PRO_CRC_ENABLE)
// static uint8 ips200pro_calculate_crc8(uint8 *data, uint32 length)
// {
//     uint8 i, crc = 0;
//     while(length--)
//     {
//         crc ^= *data++;
//         for (i = 0; i < 8; i++)
//         {
//             crc = (crc & 0x80) ? ((crc << 1) ^ 0x07) : (crc <<= 1);
//         }
//     }
//     return crc;
// }
// #endif

// uint8 ips200pro_send_buffer(const void *buffer, uint32 length, uint32 time_out, uint8 end_flag)
// {
//     uint8 return_state = 1;

//     if(     ((0 == time_out) || (0 == ips200pro_wait_idle(time_out)))  	// �ȴ�δ��ʱ
//         &&  (IPS200PRO_SPI_LENGTH >= length)                           	// ������δ��������
//         &&  (NULL != buffer))                                       	// ָ�벻Ϊ��
//     {
//         gpio_low(IPS200PRO_CS_PIN);
//         wifi_spi_flag = false;
//         g_sci_spi2.p_api->write(g_sci_spi2.p_ctrl, (const uint8_t *)buffer, length, SPI_BIT_WIDTH_8_BITS);
//         spi2_time_out();
        
//         if(1 == end_flag)
//         {
//             gpio_high(IPS200PRO_CS_PIN);
//         }
//         return_state = 0;
//     }
//     return return_state;
// }


// uint8 ips200pro_receive_buffer(void *buffer, uint32 length, uint32 time_out)
// {
//     uint8 return_state = 1;

//     if(     ((0 == time_out) || (0 == ips200pro_wait_idle(time_out)))  	// �ȴ�δ��ʱ
//         &&  (IPS200PRO_SPI_LENGTH >= length)                           	// ������δ��������
//         &&  (NULL != buffer))                                       	// ָ�벻Ϊ��
//     {
//         gpio_low(IPS200PRO_CS_PIN);
//         wifi_spi_flag = false;
//         g_sci_spi2.p_api->writeRead(g_sci_spi2.p_ctrl, (const uint8_t *)buffer, (uint8_t *)buffer, length, SPI_BIT_WIDTH_8_BITS);
//         spi2_time_out();
//         gpio_high(IPS200PRO_CS_PIN);
//         return_state = 0;
//     }

//     return return_state;
// }

// uint8 ips200pro_write_packet(ips200pro_command1_enum command1, ips200pro_command2_enum command2, uint8 widgets_id, ips200pro_header_struct *temp, uint32 length, const void *buffer, uint32 buffer_length)
// {
//     uint8 return_state  = 1;

//     temp->command1      = command1;
//     temp->command2      = command2;
//     temp->widgets_id    = widgets_id;
//     temp->length        = length + buffer_length;
// #if(1 == IPS200PRO_CRC_ENABLE)
//     temp->check_crc8    = 0;
//     temp->check_crc8    = ips200pro_calculate_crc8((uint8 *)temp, length);
// #endif

//     return_state = ips200pro_send_buffer(temp, length, IPS200PRO_WAIT_TIME, NULL == buffer);
//     if((0 == return_state) && (NULL != buffer) && (0 != buffer_length))
//     {
//         return_state = ips200pro_send_buffer(buffer, buffer_length, 0, 1);
//     }
//     else
//     {
//         gpio_high(IPS200PRO_CS_PIN);
//     }
//     return return_state;
// }

// uint8 ips200pro_read_parameter(ips200pro_command2_enum command2, ips200pro_header_struct *temp, uint8 length)
// {
//     uint8 return_state;

//     temp->command1   = IPS200PRO_PARAMETER_GET;
//     temp->command2   = command2;
//     temp->length     = length;
//     // ��������
//     return_state    = ips200pro_send_buffer(temp, length, IPS200PRO_WAIT_TIME, 1);
//     // ��ȡ����
//     temp->command1  = 0x00;
//     return_state    = ips200pro_receive_buffer(temp, length, IPS200PRO_WAIT_TIME);
//     //*parameter = temp.data[0].uint8_data[0];
//     return return_state;
// }

// uint8 ips200pro_set_date(uint16 year, uint8 month, uint8 day)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = year;
//     temp.data[1].uint8_data[0]  = month;
//     temp.data[1].uint8_data[1]  = day;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_DATE, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_time(uint8 hour, uint8 minute, uint8 second)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint8_data[0]  = hour;
//     temp.data[0].uint8_data[1]  = minute;
//     temp.data[1].uint8_data[0]  = second;
//     temp.data[1].uint8_data[1]  = 0;

//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_TIME, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_parent(uint16 child_id, uint16 parent_id)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint8_data[0]  = (uint8)child_id;
//     temp.data[0].uint8_data[1]  = child_id >> 8;
//     temp.data[1].uint8_data[0]  = (uint8)parent_id;
//     temp.data[1].uint8_data[1]  = parent_id >> 8;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_PARENT, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_format(ips200pro_format_enum format)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint8_data[0]  = format;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_CODED_FORMAT, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_backlight(uint8 backlight)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint8_data[0]  = backlight;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_BACKLIGHT, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_direction(ips200pro_display_direction_enum dir)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint8_data[0]  = dir;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_DIRECTION, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_crc_state(uint8 crc_state)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint8_data[0]  = crc_state;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_CRC_STATE, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_default_font(ips200pro_font_size_enum font)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint8_data[0]  = font;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, IPS200PRO_SYSTEM_FONT_SIZE, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_optimize(uint8 state)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint8_data[0]  = state;
//     return_state = ips200pro_write_packet(IPS200PRO_PARAMETER_SET, SCREEN_SYSTEM_OPTIMIZE, 0, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_get_date(ips200pro_time_struct *time)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     return_state = ips200pro_read_parameter(IPS200PRO_SYSTEM_DATE, (ips200pro_header_struct *)&temp, sizeof(temp));
//     if(0 == return_state)
//     {
//         memcpy(&time->year, &(temp.data[0]), 4);
//     }
//     return return_state;
// }

// uint8 ips200pro_get_time(ips200pro_time_struct *time)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     return_state = ips200pro_read_parameter(IPS200PRO_SYSTEM_TIME, (ips200pro_header_struct *)&temp, sizeof(temp));
//     if(0 == return_state)
//     {
//         memcpy(&time->hour, &(temp.data[0]), 4);
//     }
//     return return_state;
// }

// uint8 ips200pro_get_information(ips200pro_information_struct *information)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 4);

//     return_state = ips200pro_read_parameter(IPS200PRO_SYSTEM_INFORMATION, (ips200pro_header_struct *)&temp, sizeof(temp));
//     if(0 == return_state)
//     {
//         information->version_major  = temp.widgets_id;              // ���汾
//         memcpy(&information->id, &(temp.data[0]), 8);
//     }
//     return return_state;
// }

// uint8 ips200pro_get_free_stack_size(uint32 *stack_size)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     return_state = ips200pro_read_parameter(IPS200PRO_SYSTEM_FREE_STACK, (ips200pro_header_struct *)&temp, sizeof(temp));
//     if(0 == return_state)
//     {
//         *stack_size = *((uint32 *)&temp.data[0]);
//     }
//     return return_state;
// }

// uint8 ips200pro_create_widgets(uint16 widgets_id, int16 x, int16 y, uint16 width, uint16 height)
// {
//     uint8 return_state = 1;
//     IPS200PRO_COMMON_STRUCT(temp, 4);

//     temp.data[0].int16_data     = x;
//     temp.data[1].int16_data     = y;
//     temp.data[2].uint16_data    = width;
//     temp.data[3].uint16_data    = height;

//     if(ips200pro_page_num || (IPS200PRO_WIDGETS_PAGE == (widgets_id >> 8)))
//     {
//         // ֻ����ҳ���Ѿ������󣬲����������������
//         return_state = ips200pro_write_packet(widgets_id >> 8, IPS200PRO_COMMON_CREATE, (uint8)widgets_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     }

//     return return_state;
// }

// uint8 ips200pro_delete_widgets(uint16 widgets_id)
// {
//     uint8 return_state;
//     ips200pro_header_struct temp;

//     return_state = ips200pro_write_packet(widgets_id >> 8, IPS200PRO_COMMON_DELETE, (uint8)widgets_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_font(uint16 widgets_id, ips200pro_font_size_enum font_size)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint16_data    = font_size;
//     return_state = ips200pro_write_packet(widgets_id >> 8, IPS200PRO_COMMON_FONT_SIZE, (uint8)widgets_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_color(uint16 widgets_id, ips200pro_widgets_color_type_enum color_type, uint16 color)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = color_type;
//     temp.data[1].uint16_data    = color;
//     return_state = ips200pro_write_packet(widgets_id >> 8, IPS200PRO_COMMON_COLOR, (uint8)widgets_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_position(uint16 widgets_id, int16 x, int16 y)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].int16_data    = x;
//     temp.data[1].int16_data    = y;
//     return_state = ips200pro_write_packet(widgets_id >> 8, IPS200PRO_COMMON_POSITION, (uint8)widgets_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_set_hidden(uint16 widgets_id, uint8 state)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint16_data    = state;
//     return_state = ips200pro_write_packet(widgets_id >> 8, IPS200PRO_COMMON_HIDDEN, (uint8)widgets_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }


// uint16 ips200pro_page_create(char *str)
// {
//     uint8 return_state = 1;

//     if(MAX_ID_PAGE > ips200pro_page_num)
//     {
//         return_state = ips200pro_create_widgets(++ips200pro_page_num | (IPS200PRO_WIDGETS_PAGE << 8), 0, 0, 0, 0);
//         if(1 == return_state)
//         {
//             ips200pro_page_num--;
//         }
//         else if(NULL != str)
//         {
//            return_state = ips200pro_page_set_title_name(ips200pro_page_num, str);
//            ips200pro_page_switch(ips200pro_page_num, PAGE_ANIM_ON);
//         }
//     }
//     return return_state == 1 ? 0 : (ips200pro_page_num | (IPS200PRO_WIDGETS_PAGE << 8));
// }

// uint8 ips200pro_page_switch(uint16 page_id, ips200pro_page_animations_enum anim_en)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint16_data = anim_en;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_PAGE, IPS200PRO_PAGE_SWITCH, (uint8)page_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_page_hidden(uint16 page_id, uint8 state)
// {
//     uint8 return_state;
//     if(0 == page_id)
//     {
//         page_id = 0 | (IPS200PRO_WIDGETS_PAGE << 8);
//     }
//     return_state = ips200pro_set_hidden(page_id, state);
//     return return_state;
// }

// uint8 ips200pro_page_set_title_name(uint16 page_id, char *str)
// {
//     uint8 return_state;
//     ips200pro_header_struct temp;

//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_PAGE, IPS200PRO_COMMON_VALUE, (uint8)page_id, (ips200pro_header_struct *)&temp, sizeof(temp), str, strlen(str));
//     return return_state;
// }

// uint8 ips200pro_page_set_title_position_width(ips200pro_title_position_enum title_position, uint8 title_width)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = title_position;
//     temp.data[1].uint16_data    = title_width;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_PAGE, IPS200PRO_PAGE_TITLE, 1, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint16 ips200pro_label_create(int16 x, int16 y, uint16 width, uint16 height)
// {
//     uint8 return_state = 1;
//     static uint8 lebel_num = 0;

//     if(MAX_ID_LABEL > lebel_num)
//     {
//         return_state = ips200pro_create_widgets(++lebel_num | (IPS200PRO_WIDGETS_LABEL << 8), x, y, width, height);
//         if(1 == return_state)
//         {
//             lebel_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (lebel_num | (IPS200PRO_WIDGETS_LABEL << 8));
// }

// uint8 ips200pro_label_printf(uint16 label_id, const char *format, ...)
// {
// 	int32 str_length; 
//     va_list arg;
//     va_start(arg, format);
//     uint8 return_state = 1;
//     ips200pro_header_struct temp;
	
// 	str_length = vsnprintf(ips200pro_printf_buffer, sizeof(ips200pro_printf_buffer) - 1, format, arg);
// 	if(0 <= str_length)
// 	{
// 		temp.length = (uint32)str_length;
// 		return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_LABEL, IPS200PRO_COMMON_VALUE, (uint8)label_id, (ips200pro_header_struct *)&temp, sizeof(temp), ips200pro_printf_buffer, temp.length);
// 	}
// 	va_end(arg);

//     return return_state;
// }

// uint8 ips200pro_label_show_string(uint16 label_id, const char *str)
// {
//     uint8 return_state;
//     ips200pro_header_struct temp;

//     temp.length  = strlen(str);
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_LABEL, IPS200PRO_COMMON_VALUE, (uint8)label_id, (ips200pro_header_struct *)&temp, sizeof(temp), str, temp.length);

//     return return_state;
// }

// uint8 ips200pro_label_mode(uint16 label_id, ips200pro_label_mode_enum mode)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint16_data    = mode;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_LABEL, IPS200PRO_LABEL_LONG_MODE, (uint8)label_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }


// uint16 ips200pro_table_create(int16 x, int16 y, uint16 row_num, uint16 col_num)
// {
//     uint8 return_state = 1;
//     static uint8 table_num = 0;
//     if(MAX_ID_TABLE > table_num)
//     {
//         return_state = ips200pro_create_widgets(++table_num | (IPS200PRO_WIDGETS_TABLE << 8), x, y, row_num, col_num);
//         if(1 == return_state)
//         {
//             table_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (table_num | (IPS200PRO_WIDGETS_TABLE << 8));
// }

// uint8 ips200pro_table_cell_printf(uint16 table_id, uint8 row, uint8 col, char *format, ...)
// {
// 	int32 str_length; 
//     va_list arg;
//     va_start(arg, format);
//     uint8 return_state = 1;
//     IPS200PRO_COMMON_STRUCT(temp, 2);
	
// 	str_length = vsnprintf(ips200pro_printf_buffer, sizeof(ips200pro_printf_buffer) - 1, format, arg);
// 	if(0 <= str_length)
// 	{
// 		temp.length = (uint32)str_length;
// 		temp.data[0].uint16_data	= row;
// 		temp.data[1].uint16_data	= col;
// 		return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_TABLE, IPS200PRO_COMMON_VALUE, (uint8)table_id, (ips200pro_header_struct *)&temp, sizeof(temp), ips200pro_printf_buffer, temp.length);
// 	}
// 	va_end(arg);
	
//     return return_state;
// }


// uint8 ips200pro_table_set_col_width(uint16 table_id, uint8 col, uint16 width)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = width;
//     temp.data[1].uint16_data    = col;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_TABLE, IPS200PRO_TABLE_COL_WIDTH, (uint8)table_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_table_select(uint16 table_id, uint8 row, uint8 col)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = row;
//     temp.data[1].uint16_data    = col;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_TABLE, IPS200PRO_TABLE_SELECT, (uint8)table_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint16 ips200pro_meter_create(int16 x, int16 y, uint16 size, ips200pro_meter_style_enum style)
// {
//     uint8 return_state = 1;
//     static uint8 meter_num = 0;
//     if(MAX_ID_METER > meter_num)
//     {
//         return_state = ips200pro_create_widgets(++meter_num | (IPS200PRO_WIDGETS_METER << 8), x, y, size, style);
//         if(1 == return_state)
//         {
//             meter_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (meter_num | (IPS200PRO_WIDGETS_METER << 8));
// }

// uint8 ips200pro_meter_set_value(uint16 meter_id, int16 value)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].int16_data     = value;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_METER, IPS200PRO_COMMON_VALUE, (uint8)meter_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint16 ips200pro_clock_create(int16 x, int16 y, uint16 clock_size, ips200pro_clock_style_enum clock_type)
// {
//     uint8 return_state = 1;
//     static uint8 clock_num = 0;
//     if(MAX_ID_CLOCK > clock_num)
//     {
//         return_state = ips200pro_create_widgets(++clock_num | (IPS200PRO_WIDGETS_CLOCK << 8), x, y, clock_size, clock_type);
//         if(1 == return_state)
//         {
//             clock_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (clock_num | (IPS200PRO_WIDGETS_CLOCK << 8));
// }


// uint16 ips200pro_progress_bar_create(int16 x, int16 y, uint16 width, uint16 height)
// {
//     uint8 return_state = 1;
//     static uint8 progress_bar_num = 0;
//     if(MAX_ID_PROGRESS_BAR > progress_bar_num)
//     {
//         return_state = ips200pro_create_widgets(++progress_bar_num | (IPS200PRO_WIDGETS_BAR << 8), x, y, width, height);
//         if(1 == return_state)
//         {
//             progress_bar_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (progress_bar_num | (IPS200PRO_WIDGETS_BAR << 8));
// }

// uint8 ips200pro_progress_bar_set_value(uint16 progress_bar_id, uint8 start_value, uint8 end_value)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = start_value;
//     temp.data[1].uint16_data    = end_value;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_BAR, IPS200PRO_COMMON_VALUE, (uint8)progress_bar_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint16 ips200pro_calendar_create(int16 x, int16 y, uint16 width, uint16 height)
// {
//     uint8 return_state;
//     // ֻ�ܴ���һ�������ؼ�
//     return_state = ips200pro_create_widgets(1 | (IPS200PRO_WIDGETS_CALENDAR << 8), x, y, width, height);
//     return return_state == 1 ? 0 : (1 | (IPS200PRO_WIDGETS_CALENDAR << 8));
// }

// uint8 ips200pro_calendar_display(uint16 year, uint8 month, ips200pro_calendar_mode_enum mode)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = year;
//     temp.data[1].uint8_data[0]  = month;
//     temp.data[1].uint8_data[1]  = mode;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_CALENDAR, IPS200PRO_COMMON_VALUE, 1, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint16 ips200pro_waveform_create(int16 x, int16 y, uint16 width, uint16 height)
// {
//     uint8 return_state = 1;
//     static uint8 waveform_num = 0;
//     if(MAX_ID_WAVEFORM > waveform_num)
//     {
//         return_state = ips200pro_create_widgets(++waveform_num | (IPS200PRO_WIDGETS_WAVEFORM << 8), x, y, width, height);
//         if(1 == return_state)
//         {
//             waveform_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (waveform_num | (IPS200PRO_WIDGETS_WAVEFORM << 8));
// }

// uint8 ips200pro_waveform_add_value(uint16 waveform_id, uint8 line_id, const uint16 *data, uint16 length, uint16 color)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = line_id;
//     temp.data[1].uint16_data    = color;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_WAVEFORM, IPS200PRO_COMMON_VALUE, (uint8)waveform_id, (ips200pro_header_struct *)&temp, sizeof(temp), data, length * 2);
//     return return_state;
// }

// uint8 ips200pro_waveform_line_state(uint16 waveform_id, uint16 line_id, uint16 line_state)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = line_id;
//     temp.data[1].uint16_data    = line_state;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_WAVEFORM, IPS200PRO_WAVEFORM_LINE_STATE, (uint8)waveform_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_waveform_line_type(uint16 waveform_id, uint8 line_type)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 1);

//     temp.data[0].uint16_data    = line_type;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_WAVEFORM, IPS200PRO_WAVEFORM_LINE_TYPE, (uint8)waveform_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint8 ips200pro_waveform_clear(uint16 waveform_id)
// {
//     uint8 return_state;
//     ips200pro_header_struct temp;

//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_WAVEFORM, IPS200PRO_WAVEFORM_CLEAR, (uint8)waveform_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint16 ips200pro_image_create(int16 x, int16 y, uint16 width, uint16 height)
// {
//     uint8 return_state = 1;
//     static uint8 image_num = 0;
//     if(MAX_ID_IMAGE > image_num)
//     {
//         return_state = ips200pro_create_widgets(++image_num | (IPS200PRO_WIDGETS_IMAGE << 8), x, y, width, height);
//         if(1 == return_state)
//         {
//             image_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (image_num | (IPS200PRO_WIDGETS_IMAGE << 8));
// }

// uint8 ips200pro_image_display(uint16 image_id, const void *image, uint16 width, uint16 height, ips200pro_image_type_enum image_type, uint8 threshold)
// {
//     uint8 return_state = 0;
//     uint16 send_length;
//     uint32 image_size;
//     uint8 const *image_data;
//     IPS200PRO_COMMON_STRUCT(temp, 4);

//     image_data = (uint8 *)image;
//     image_size = width * height * (IMAGE_RGB565 == image_type ? 2 : 1);
//     // �����������㣬���ʾ����Ҫ����ͼ�����ݣ���֪ͨ��Ļ���±��߻����
//     if((NULL == image) || (!width) || (!height) || (IMAGE_NULL == image_type))
//     {
//         temp.data[0].uint16_data    = 0;
//         temp.data[1].uint16_data    = 0;
//         temp.data[2].uint8_data[1]  = IMAGE_NULL;
//     }
//     else
//     {
//         temp.data[0].uint16_data    = width;
//         temp.data[1].uint16_data    = height;
//         temp.data[2].uint8_data[1]  = image_type;
//     }
//     temp.data[2].uint8_data[0]  = 1;            // ͼ��ʼ�����־λ
//     temp.data[3].uint16_data    = threshold;

//     do
//     {
//         // ���㱾�δ����ֽ���
//         send_length = image_size > (IPS200PRO_SPI_LENGTH - sizeof(temp)) ? (IPS200PRO_SPI_LENGTH - sizeof(temp)) : (uint16)image_size;
//         return_state += ips200pro_write_packet(IPS200PRO_WIDGETS_IMAGE, IPS200PRO_COMMON_VALUE, (uint8)image_id, (ips200pro_header_struct *)&temp, sizeof(temp), image_data, send_length);
//         image_data += send_length;
//         image_size -= send_length;
//         temp.data[2].uint8_data[0] = 0;
//     }while(image_size);

//     return return_state;
// }

// uint8 ips200pro_image_draw_line(uint16 image_id, uint8 line_id, void *line_data, uint16 line_length, ips200pro_image_line_type_enum data_type, uint16 color)
// {
//     uint8 return_state = 1;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     if(MAX_ID_IMAGE_LINE >= line_id)
//     {
//         temp.length                 = data_type * line_length * 2 + sizeof(temp);
//         temp.data[0].uint8_data[0]  = line_id;
//         temp.data[0].uint8_data[1]  = data_type;
//         temp.data[1].uint16_data    = color;
//         return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_IMAGE, IPS200PRO_IMAGE_DRAW_LINE, (uint8)image_id, (ips200pro_header_struct *)&temp, sizeof(temp), line_data, data_type * line_length * 2);
//     }
//     return return_state;
// }

// uint8 ips200pro_image_draw_rectangle(uint16 image_id, uint8 rectangle_id, int16 x, int16 y, uint16 rectangle_width, uint16 rectangle_height, uint16 color)
// {
//     uint8 return_state = 1;
//     IPS200PRO_COMMON_STRUCT(temp, 6);
//     if(MAX_ID_IMAGE_RECTANGLE >= rectangle_id)
//     {
//         temp.data[0].uint16_data    = rectangle_id;
//         temp.data[1].int16_data     = x;
//         temp.data[2].int16_data     = y;
//         temp.data[3].uint16_data    = rectangle_width;
//         temp.data[4].uint16_data    = rectangle_height;
//         temp.data[5].uint16_data    = color;
//         return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_IMAGE, IPS200PRO_IMAGE_DRAW_RECTANGLE, (uint8)image_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     }
//     return return_state;
// }

// uint16 ips200pro_container_create(int16 x, int16 y, uint16 width, uint16 height)
// {
//     uint8 return_state = 1;
//     static uint8 container_num = 0;
//     if(MAX_ID_IMAGE_RECTANGLE > container_num)
//     {
//         return_state = ips200pro_create_widgets(++container_num | (IPS200PRO_WIDGETS_CONTAINER << 8), x, y, width, height);
//         if(1 == return_state)
//         {
//             container_num--;
//         }
//     }
//     return return_state == 1 ? 0 : (container_num | (IPS200PRO_WIDGETS_CONTAINER << 8));
// }

// uint8 ips200pro_container_radius(uint16 container_id, uint16 border_width, uint16 radius)
// {
//     uint8 return_state;
//     IPS200PRO_COMMON_STRUCT(temp, 2);

//     temp.data[0].uint16_data    = border_width;
//     temp.data[1].uint16_data    = radius;
//     return_state = ips200pro_write_packet(IPS200PRO_WIDGETS_CONTAINER, IPS200PRO_COMMON_VALUE, (uint8)container_id, (ips200pro_header_struct *)&temp, sizeof(temp), NULL, 0);
//     return return_state;
// }

// uint16 ips200pro_init(char *str, ips200pro_title_position_enum title_position, uint8 title_size)
// {
//     uint16 page_id = 0;
//     g_sci_spi2.p_api->open(g_sci_spi2.p_ctrl, g_sci_spi2.p_cfg);
    
//     gpio_set_level(IPS200PRO_RST_PIN, 0);
//     system_delay_ms(5);
//     gpio_set_level(IPS200PRO_RST_PIN, 1);
//     system_delay_ms(20);

//     // ĳһЩ������Ļ�ӿ�����û��MISO���ţ������޷���ȡ��Ļ��Ϣ
//     // ips200pro_get_information(&ips200pro_information);
//     // ips200pro_get_time(&ips200pro_time);

//     // ����ʱ���������Ч����˲��������ڳ�ʼ���е���ʱ�����ú���������ڳ�ʼ���е��þͻᵼ��ÿ���ϵ�֮��ʱ�䶼�ỹԭ
//     // ips200pro_set_time(15, 54, 30);

//     // �ر�ע�⡢�ر�ע�⡢�ر�ע��
//     // ����ҳ�������ʾλ���Լ�������ȣ���Ҫע����ڴ���ҳ��֮ǰ���ò�����Ч
//     // �ر�ע�⡢�ر�ע�⡢�ر�ע��
//     ips200pro_page_set_title_position_width(title_position, title_size);
//     ips200pro_set_format(IPS200PRO_DEFAULT_FORMAT);
//     ips200pro_set_default_font(IPS200PRO_DEFAULT_FONT_SIZE);
//     ips200pro_set_optimize(IPS200PRO_DEFAULT_OPTIMIZE);
//     // ����Ĭ�ϲ���������һ��ҳ��
//     if(NULL != str)
//     {
//         page_id = ips200pro_page_create(str); // ����һ��ҳ��
//     }

// #if(1 == IPS200PRO_CRC_ENABLE)
//     ips200pro_set_crc_state(1);  // ʹ��CRCģʽ��ǿ������������������Ҫ��������һ����������CRC����
// #endif

//     return page_id;
// }
