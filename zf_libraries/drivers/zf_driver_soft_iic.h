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
* �ļ�����          zf_driver_soft_iic
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
#ifndef _zf_driver_soft_iic_h_
#define _zf_driver_soft_iic_h_

#include "hal_data.h"
#include "zf_common_typedef.h"

typedef struct
{
    uint32              scl_pin;                                                // ���ڼ�¼��Ӧ�����ű��
    uint32              sda_pin;                                                // ���ڼ�¼��Ӧ�����ű��
    uint8               addr;                                                   // ������ַ ��λ��ַģʽ
    uint32              delay;                                                  // ģ�� IIC ����ʱʱ��
}soft_iic_info_struct;


void        soft_iic_write_8bit             (soft_iic_info_struct *soft_iic_obj, const uint8_t data);
void        soft_iic_write_8bit_array       (soft_iic_info_struct *soft_iic_obj, const uint8_t *data, uint32_t len);

void        soft_iic_write_16bit            (soft_iic_info_struct *soft_iic_obj, const uint16_t data);
void        soft_iic_write_16bit_array      (soft_iic_info_struct *soft_iic_obj, const uint16_t *data, uint32_t len);

void        soft_iic_write_8bit_register    (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, const uint8_t data);
void        soft_iic_write_8bit_registers   (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, const uint8_t *data, uint32_t len);

void        soft_iic_write_16bit_register   (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name, const uint16_t data);
void        soft_iic_write_16bit_registers  (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name, const uint16_t *data, uint32_t len);

uint8_t     soft_iic_read_8bit              (soft_iic_info_struct *soft_iic_obj);
void        soft_iic_read_8bit_array        (soft_iic_info_struct *soft_iic_obj, uint8_t *data, uint32_t len);

uint16_t    soft_iic_read_16bit             (soft_iic_info_struct *soft_iic_obj);
void        soft_iic_read_16bit_array       (soft_iic_info_struct *soft_iic_obj, uint16_t *data, uint32_t len);

uint8_t     soft_iic_read_8bit_register     (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name);
void        soft_iic_read_8bit_registers    (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, uint8_t *data, uint32_t len);

uint16_t    soft_iic_read_16bit_register    (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name);
void        soft_iic_read_16bit_registers   (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name, uint16_t *data, uint32_t len);

void        soft_iic_transfer_8bit_array    (soft_iic_info_struct *soft_iic_obj, const uint8_t *write_data, uint32_t write_len, uint8_t *read_data, uint32_t read_len);
void        soft_iic_transfer_16bit_array   (soft_iic_info_struct *soft_iic_obj, const uint16_t *write_data, uint32_t write_len, uint16_t *read_data, uint32_t read_len);

void        soft_iic_sccb_write_register    (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, uint8_t data);
uint8_t     soft_iic_sccb_read_register     (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name);

void        soft_iic_init                   (soft_iic_info_struct *soft_iic_obj, uint8_t addr, uint32_t delay, bsp_io_port_pin_t scl_pin, bsp_io_port_pin_t sda_pin);
void    soft_iic_start      (soft_iic_info_struct *soft_iic_obj);
void    soft_iic_stop       (soft_iic_info_struct *soft_iic_obj);
uint8_t soft_iic_send_data  (soft_iic_info_struct *soft_iic_obj, const uint8_t data);
uint8_t mt9v03x_force_write_16bit(soft_iic_info_struct *soft_iic_obj, uint8_t reg, uint16_t data);
uint16_t mt9v03x_force_read_16bit(soft_iic_info_struct *soft_iic_obj, uint8_t reg);
#endif

