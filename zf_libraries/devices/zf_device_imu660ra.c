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
// * �ļ�����          zf_device_imu660ra
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
// #include "zf_device_imu660ra.h"
// #include "zf_device_config.h"

// int16 imu660ra_gyro_x = 0, imu660ra_gyro_y = 0, imu660ra_gyro_z = 0;            // ��������������   gyro (������)
// int16 imu660ra_acc_x = 0, imu660ra_acc_y = 0, imu660ra_acc_z = 0;  

// volatile bool write_flag = false;

// void spi1_callback (spi_callback_args_t *p_args)
// {
//     if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
//     {
//         write_flag = true;
//     }
// }

// //-------------------------------------------------------------------------------------------------------------------
// // �������     IMU660RA д�Ĵ���
// // ����˵��     reg             �Ĵ�����ַ
// // ����˵��     data            ����
// // ���ز���     void
// // ʹ��ʾ��     imu660ra_write_acc_gyro_register(IMU660RA_SLV0_CONFIG, 0x00);
// // ��ע��Ϣ     �ڲ�����
// //-------------------------------------------------------------------------------------------------------------------
// static void imu660ra_write_register (uint8 reg, uint8 idata)
// {
//     IMU660RA_CS(0);
//     uint8 command[2] = {reg | IMU660RA_SPI_W, idata};
//     write_flag = false;
//     g_spi1.p_api->write(g_spi1.p_ctrl, command, 2, SPI_BIT_WIDTH_8_BITS);
//     while(!write_flag);

//     IMU660RA_CS(1);
// }

// static volatile uint8 static_dat = 0;


// static uint8 imu660ra_read_register (uint8 reg)
// {
//     uint8 p_reg[3] = {reg | IMU660RA_SPI_R, 0};
//     uint8 idata[3] = {0};

//     IMU660RA_CS(0);
    
//     write_flag = false;
//     g_spi1.p_api->writeRead(g_spi1.p_ctrl, p_reg, idata, 3, SPI_BIT_WIDTH_8_BITS);
//     while(!write_flag);
    
//     IMU660RA_CS(1);
//     static_dat = idata[2];

//     return idata[2];
// }

// //-------------------------------------------------------------------------------------------------------------------
// // �������     IMU963RA ������ �ڲ�����
// // ����˵��     reg             �Ĵ�����ַ
// // ����˵��     data            ���ݻ�����
// // ����˵��     len             ���ݳ���
// // ���ز���     void
// // ʹ��ʾ��     imu963ra_read_acc_gyro_registers(IMU963RA_OUTX_L_A, dat, 6);
// // ��ע��Ϣ     �ڲ�����
// //-------------------------------------------------------------------------------------------------------------------
// static void imu660ra_read_registers (uint8_t reg, uint8_t *idata, uint32_t len)
// {
//     uint8_t p_reg[8] = {reg | IMU660RA_SPI_R, 0, 0, 0, 0, 0, 0};

//     IMU660RA_CS(0);
    
//     write_flag = false;
//     g_spi1.p_api->writeRead(g_spi1.p_ctrl, &p_reg, idata, len, SPI_BIT_WIDTH_8_BITS);
//     while(!write_flag);

//     IMU660RA_CS(1);
// }


// //-------------------------------------------------------------------------------------------------------------------
// // �������     IMU660RA ������ �ڲ�����
// // ����˵��     reg             �Ĵ�����ַ
// // ����˵��     data            ���ݻ�����
// // ����˵��     len             ���ݳ���
// // ���ز���     void
// // ʹ��ʾ��     imu660ra_read_acc_gyro_registers(IMU660RA_OUTX_L_A, dat, 6);
// // ��ע��Ϣ     �ڲ�����
// //-------------------------------------------------------------------------------------------------------------------
// static void imu660ra_write_registers (uint8 reg, uint8 *idata, uint32_t len)
// {
//     IMU660RA_CS(0);
    
//     uint8 command[2] = {reg | IMU660RA_SPI_W};
//     write_flag = false;
//     g_spi1.p_api->write(g_spi1.p_ctrl, command, 1, SPI_BIT_WIDTH_8_BITS);
//     while(!write_flag);

//     write_flag = false;
//     g_spi1.p_api->write(g_spi1.p_ctrl, idata, len, SPI_BIT_WIDTH_8_BITS);
//     while(!write_flag);
    
//     IMU660RA_CS(1);
// }

// //-------------------------------------------------------------------------------------------------------------------
// // �������     IMU660RA �����Լ� �ڲ�����
// // ����˵��     void
// // ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// // ʹ��ʾ��     imu660ra_acc_gyro_self_check();
// // ��ע��Ϣ     �ڲ�����
// //-------------------------------------------------------------------------------------------------------------------
// static uint8 imu660ra_self_check (void)
// {
//     uint8 dat = 0, return_state = 0;
//     uint16 timeout_count = 0;
//     do
//     {
//         if(IMU660RA_TIMEOUT_COUNT < timeout_count ++)
//         {
//             return_state =  1;
//             break;
//         }
//         dat = imu660ra_read_register(IMU660RA_CHIP_ID);
//         system_delay_ms(1);
//     }while(0x24 != dat);                                                        // ��ȡ�豸ID�Ƿ����0X24���������0X24����Ϊû��⵽�豸
//     return return_state;
// }

// //-------------------------------------------------------------------------------------------------------------------
// // �������     ��ȡ IMU660RA ���ٶȼ�����
// // ����˵��     void
// // ���ز���     void
// // ʹ��ʾ��     imu660ra_get_acc();
// // ��ע��Ϣ     ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// //-------------------------------------------------------------------------------------------------------------------
// void imu660ra_get_acc (void)
// {
//     uint8 dat[7];

//     imu660ra_read_registers(IMU660RA_ACC_ADDRESS, dat, 8);
//     imu660ra_acc_x = (int16_t)(((uint16_t)dat[3] << 8 | dat[2]));
//     imu660ra_acc_y = (int16_t)(((uint16_t)dat[5] << 8 | dat[4]));
//     imu660ra_acc_z = (int16_t)(((uint16_t)dat[7] << 8 | dat[6]));
// }


// //-------------------------------------------------------------------------------------------------------------------
// // �������     ��ȡIMU660RA����������
// // ����˵��     void
// // ���ز���     void
// // ʹ��ʾ��     imu660ra_get_gyro();
// // ��ע��Ϣ     ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// //-------------------------------------------------------------------------------------------------------------------
// void imu660ra_get_gyro (void)
// {
//     uint8 dat[7];

//     imu660ra_read_registers(IMU660RA_GYRO_ADDRESS, dat, 8);
//     imu660ra_gyro_x = (int16_t)(((uint16_t)dat[3] << 8 | dat[2]));
//     imu660ra_gyro_y = (int16_t)(((uint16_t)dat[5] << 8 | dat[4]));
//     imu660ra_gyro_z = (int16_t)(((uint16_t)dat[7] << 8 | dat[6]));
// }

// //-------------------------------------------------------------------------------------------------------------------
// // �������     ��ʼ�� IMU660RA
// // ����˵��     void
// // ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// // ʹ��ʾ��     imu660ra_init();
// // ��ע��Ϣ     
// //-------------------------------------------------------------------------------------------------------------------
// uint8 imu660ra_init (void)
// {
//     uint8 return_state = 0;
                                      
//     system_delay_ms (10);
//     g_spi1.p_api->open(g_spi1.p_ctrl, g_spi1.p_cfg);
//     imu660ra_write_register(0x00, 0x00); 
//     do
//     {
//         if(imu660ra_self_check())                                               // IMU660RA �Լ�
//         {
//             // �������������˶�����Ϣ ������ʾ����λ��������
//             // ��ô���� IMU660RA �Լ��������ʱ�˳���
//             // ���һ�½�����û������ ���û������ܾ��ǻ���
//             return_state = 1;
//             break;
//         }
//         imu660ra_write_register(IMU660RA_PWR_CONF, 0x00);                       // �رո߼�ʡ��ģʽ
//         system_delay_ms(1);
//         imu660ra_write_register(IMU660RA_INIT_CTRL, 0x00);                      // ��ʼ��ģ����г�ʼ������
//         imu660ra_write_registers(IMU660RA_INIT_DATA, (uint8*)imu660ra_config_file, sizeof(imu660ra_config_file));   // ��������ļ�
//         imu660ra_write_register(IMU660RA_INIT_CTRL, 0x01);                      // ��ʼ�����ý���
//         system_delay_ms(20);
//         if(1 != imu660ra_read_register(IMU660RA_INT_STA))                       // ����Ƿ��������
//         {
//             // �������������˶�����Ϣ ������ʾ����λ��������
//             // ��ô���� IMU660RA ���ó�ʼ���ļ�������
//             // ���һ�½�����û������ ���û������ܾ��ǻ���
//             return_state = 1;
//             break;
//         }
//         imu660ra_write_register(IMU660RA_PWR_CTRL, 0x0E);                       // ��������ģʽ  ʹ�������ǡ����ٶȡ��¶ȴ�����
//         imu660ra_write_register(IMU660RA_ACC_CONF, 0xA7);                       // ���ٶȲɼ����� ����ģʽ �����ɼ� 50Hz  ����Ƶ��
//         imu660ra_write_register(IMU660RA_GYR_CONF, 0xA9);                       // �����ǲɼ����� ����ģʽ �����ɼ� 200Hz ����Ƶ��

//         // IMU660RA_ACC_SAMPLE �Ĵ���
//         // ����Ϊ 0x00 ���ٶȼ�����Ϊ ��2  g   ��ȡ���ļ��ٶȼ����ݳ��� 16384  ����ת��Ϊ��������λ������ (g �����������ٶ� ����ѧ���� һ������� g ȡ 9.8 m/s^2 Ϊ��׼ֵ)
//         // ����Ϊ 0x01 ���ٶȼ�����Ϊ ��4  g   ��ȡ���ļ��ٶȼ����ݳ��� 8192   ����ת��Ϊ��������λ������ (g �����������ٶ� ����ѧ���� һ������� g ȡ 9.8 m/s^2 Ϊ��׼ֵ)
//         // ����Ϊ 0x02 ���ٶȼ�����Ϊ ��8  g   ��ȡ���ļ��ٶȼ����ݳ��� 4096   ����ת��Ϊ��������λ������ (g �����������ٶ� ����ѧ���� һ������� g ȡ 9.8 m/s^2 Ϊ��׼ֵ)
//         // ����Ϊ 0x03 ���ٶȼ�����Ϊ ��16 g   ��ȡ���ļ��ٶȼ����ݳ��� 2048   ����ת��Ϊ��������λ������ (g �����������ٶ� ����ѧ���� һ������� g ȡ 9.8 m/s^2 Ϊ��׼ֵ)
//         switch(IMU660RA_ACC_SAMPLE_DEFAULT)
//         {
//             default:
//             {
//                 return_state = 1;
//             }break;
//             case IMU660RA_ACC_SAMPLE_SGN_2G:
//             {
//                 imu660ra_write_register(IMU660RA_ACC_RANGE, 0x00);
//             }break;
//             case IMU660RA_ACC_SAMPLE_SGN_4G:
//             {
//                 imu660ra_write_register(IMU660RA_ACC_RANGE, 0x01);
//             }break;
//             case IMU660RA_ACC_SAMPLE_SGN_8G:
//             {
//                 imu660ra_write_register(IMU660RA_ACC_RANGE, 0x02);
//             }break;
//             case IMU660RA_ACC_SAMPLE_SGN_16G:
//             {
//                 imu660ra_write_register(IMU660RA_ACC_RANGE, 0x03);
//             }break;
//         }
//         if(1 == return_state)
//         {
//             break;
//         }

//         // IMU660RA_GYR_RANGE �Ĵ���
//         // ����Ϊ 0x04 ����������Ϊ ��125  dps    ��ȡ�������������ݳ��� 262.4   ����ת��Ϊ��������λ������ ��λΪ ��/s
//         // ����Ϊ 0x03 ����������Ϊ ��250  dps    ��ȡ�������������ݳ��� 131.2   ����ת��Ϊ��������λ������ ��λΪ ��/s
//         // ����Ϊ 0x02 ����������Ϊ ��500  dps    ��ȡ�������������ݳ��� 65.6    ����ת��Ϊ��������λ������ ��λΪ ��/s
//         // ����Ϊ 0x01 ����������Ϊ ��1000 dps    ��ȡ�������������ݳ��� 32.8    ����ת��Ϊ��������λ������ ��λΪ ��/s
//         // ����Ϊ 0x00 ����������Ϊ ��2000 dps    ��ȡ�������������ݳ��� 16.4    ����ת��Ϊ��������λ������ ��λΪ ��/s
//         switch(IMU660RA_GYRO_SAMPLE_DEFAULT)
//         {
//             default:
//             {
//                 return_state = 1;
//             }break;
//             case IMU660RA_GYRO_SAMPLE_SGN_125DPS:
//             {
//                 imu660ra_write_register(IMU660RA_GYR_RANGE, 0x04);
//             }break;
//             case IMU660RA_GYRO_SAMPLE_SGN_250DPS:
//             {
//                 imu660ra_write_register(IMU660RA_GYR_RANGE, 0x03);
//             }break;
//             case IMU660RA_GYRO_SAMPLE_SGN_500DPS:
//             {
//                 imu660ra_write_register(IMU660RA_GYR_RANGE, 0x02);
//             }break;
//             case IMU660RA_GYRO_SAMPLE_SGN_1000DPS:
//             {
//                 imu660ra_write_register(IMU660RA_GYR_RANGE, 0x01);
//             }break;
//             case IMU660RA_GYRO_SAMPLE_SGN_2000DPS:
//             {
//                 imu660ra_write_register(IMU660RA_GYR_RANGE, 0x00);
//             }break;
//         }
//         if(1 == return_state)
//         {
//             break;
//         }
//     }while(0);


//     return return_state;

// }
