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
// #ifndef _zf_device_imu963ra_h
// #define _zf_device_imu963ra_h

// #include "zf_common_headfile.h"

// typedef enum
// {
//     IMU660RA_ACC_SAMPLE_SGN_2G ,                                                // ���ٶȼ����� ��2G  (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
//     IMU660RA_ACC_SAMPLE_SGN_4G ,                                                // ���ٶȼ����� ��4G  (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
//     IMU660RA_ACC_SAMPLE_SGN_8G ,                                                // ���ٶȼ����� ��8G  (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
//     IMU660RA_ACC_SAMPLE_SGN_16G,                                                // ���ٶȼ����� ��16G (ACC = Accelerometer ���ٶȼ�) (SGN = signum �������� ��ʾ������Χ) (G = g �������ٶ� g��9.80 m/s^2)
// }imu660ra_acc_sample_config;

// typedef enum
// {
//     IMU660RA_GYRO_SAMPLE_SGN_125DPS ,                                           // ���������� ��125DPS  (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
//     IMU660RA_GYRO_SAMPLE_SGN_250DPS ,                                           // ���������� ��250DPS  (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
//     IMU660RA_GYRO_SAMPLE_SGN_500DPS ,                                           // ���������� ��500DPS  (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
//     IMU660RA_GYRO_SAMPLE_SGN_1000DPS,                                           // ���������� ��1000DPS (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
//     IMU660RA_GYRO_SAMPLE_SGN_2000DPS,                                           // ���������� ��2000DPS (GYRO = Gyroscope ������) (SGN = signum �������� ��ʾ������Χ) (DPS = Degree Per Second ���ٶȵ�λ ��/S)
// }imu660ra_gyro_sample_config;

// #define IMU660RA_CS_PIN                 (BSP_IO_PORT_04_PIN_13)                                       // CS Ƭѡ����
// #define IMU660RA_CS(x)                  ((x) ? (gpio_set_level(IMU660RA_CS_PIN, 1)) : (gpio_set_level(IMU660RA_CS_PIN, 0)))

// #define IMU660RA_ACC_SAMPLE_DEFAULT     ( IMU660RA_ACC_SAMPLE_SGN_8G )          // ��������Ĭ�ϵ� ���ٶȼ� ��ʼ������
// #define IMU660RA_GYRO_SAMPLE_DEFAULT    ( IMU660RA_GYRO_SAMPLE_SGN_2000DPS )    // ��������Ĭ�ϵ� ������   ��ʼ������

// #define IMU660RA_TIMEOUT_COUNT          ( 0x00FF )                                  // IMU660RA ��ʱ����

// //================================================���� IMU660RA �ڲ���ַ================================================
// #define IMU660RA_DEV_ADDR               ( 0x69 )                                    // SA0�ӵأ�0x68 SA0������0x69 ģ��Ĭ������
// #define IMU660RA_SPI_W                  ( 0x00 )
// #define IMU660RA_SPI_R                  ( 0x80 )

// #define IMU660RA_CHIP_ID                ( 0x00 )
// #define IMU660RA_PWR_CONF               ( 0x7C )
// #define IMU660RA_PWR_CTRL               ( 0x7D )
// #define IMU660RA_INIT_CTRL              ( 0x59 )
// #define IMU660RA_INIT_DATA              ( 0x5E )
// #define IMU660RA_INT_STA                ( 0x21 )
// #define IMU660RA_ACC_ADDRESS            ( 0x0C )
// #define IMU660RA_GYRO_ADDRESS           ( 0x12 )
// #define IMU660RA_ACC_CONF               ( 0x40 )
// #define IMU660RA_ACC_RANGE              ( 0x41 )
// #define IMU660RA_GYR_CONF               ( 0x42 )
// #define IMU660RA_GYR_RANGE              ( 0x43 )
// //================================================���� IMU660RA �ڲ���ַ================================================

// extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;                 // ��������������      gyro (������)
// extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;                    // ������ٶȼ�����     acc (accelerometer ���ٶȼ�)

// void  imu660ra_get_acc                  (void);                                     // ��ȡ IMU660RA ���ٶȼ�����
// void  imu660ra_get_gyro                 (void);                                     // ��ȡ IMU660RA ����������

// uint8   imu660ra_init                   (void);

// #endif
