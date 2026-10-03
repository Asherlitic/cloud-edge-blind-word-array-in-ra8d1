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
#include "zf_driver_soft_iic.h"
#include "zf_driver_gpio.h"
#include "zf_driver_delay.h"

#define SOFT_IIC_SDA_IO_SWITCH          (0)                                     // �Ƿ���Ҫ SDA ���� I/O �л� 0-����Ҫ 1-��Ҫ

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC ��ʱ
// ����˵��     delay           ��ʱ����
// ���ز���     void
// ʹ��ʾ��     soft_iic_delay(1);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
#define soft_iic_delay(x)  for(volatile uint32_t i = x; i --; )


//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC START �ź�
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ���ز���     void
// ʹ��ʾ��     soft_iic_start(soft_iic_obj);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_start (soft_iic_info_struct *soft_iic_obj)
{
    gpio_high(soft_iic_obj->scl_pin);                                           // SCL �ߵ�ƽ
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA �ߵ�ƽ

    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->sda_pin);                                            // SDA ������
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL ������
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC STOP �ź�
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ���ز���     void
// ʹ��ʾ��     soft_iic_stop(soft_iic_obj);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_stop (soft_iic_info_struct *soft_iic_obj)
{
    gpio_low(soft_iic_obj->sda_pin);                                            // SDA �͵�ƽ
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ

    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->scl_pin);                                           // SCL ������
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA ������
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC ���� ACK/NAKC �ź� �ڲ�����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     ack             ACK ��ƽ
// ���ز���     void
// ʹ��ʾ��     soft_iic_send_ack(soft_iic_obj, 1);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_send_ack (soft_iic_info_struct *soft_iic_obj, uint8_t ack)
{
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ

    if(ack)
    {
        gpio_high(soft_iic_obj->sda_pin);                                       // SDA ����
    }
    else
    {
        gpio_low(soft_iic_obj->sda_pin);                                        // SDA ����
    }

    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->scl_pin);                                           // SCL ����
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL ����
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA ����
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC ��ȡ ACK/NAKC �ź�
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ���ز���     uint8           ACK ״̬
// ʹ��ʾ��     soft_iic_wait_ack(soft_iic_obj);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_wait_ack (soft_iic_info_struct *soft_iic_obj)
{
    uint8_t temp = 0;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA �ߵ�ƽ �ͷ� SDA
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir((gpio_pin_enum)soft_iic_obj->sda_pin, GPI, GPI_FLOATING_IN);
#endif
    soft_iic_delay(soft_iic_obj->delay);

    gpio_high(soft_iic_obj->scl_pin);                                           // SCL �ߵ�ƽ
    soft_iic_delay(soft_iic_obj->delay);

    if(gpio_get_level(soft_iic_obj->sda_pin))
    {
        temp = 1;
    }
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir((gpio_pin_enum)soft_iic_obj->sda_pin, GPO, GPO_OPEN_DTAIN);
#endif
    soft_iic_delay(soft_iic_obj->delay);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC ���� 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     data            ����
// ���ز���     uint8           ACK ״̬
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_send_data (soft_iic_info_struct *soft_iic_obj, const uint8_t data)
{
    uint8_t temp = 0x80;
    while(temp)
    {
//        gpio_set_level(soft_iic_obj->sda_pin, data & temp);
        ((data & temp) ? (gpio_high(soft_iic_obj->sda_pin)) : (gpio_low(soft_iic_obj->sda_pin)));
        temp >>= 1;

        soft_iic_delay(soft_iic_obj->delay / 2);
        gpio_high(soft_iic_obj->scl_pin);                                       // SCL ����
        soft_iic_delay(soft_iic_obj->delay);
        gpio_low(soft_iic_obj->scl_pin);                                        // SCL ����
        soft_iic_delay(soft_iic_obj->delay / 2);
    }
    return ((soft_iic_wait_ack(soft_iic_obj) == 1) ? 0 : 1 );
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC ��ȡ 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     ack             ACK �� NACK
// ���ز���     uint8           ����
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_read_data (soft_iic_info_struct *soft_iic_obj, uint8_t ack)
{
    uint8_t data = 0x00;
    uint8_t temp = 8;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA �ߵ�ƽ �ͷ� SDA
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir((gpio_pin_enum)soft_iic_obj->sda_pin, GPI, GPI_FLOATING_IN);
#endif

    while(temp --)
    {
        gpio_low(soft_iic_obj->scl_pin);                                        // SCL ����
        soft_iic_delay(soft_iic_obj->delay);
        gpio_high(soft_iic_obj->scl_pin);                                       // SCL ����
        soft_iic_delay(soft_iic_obj->delay);
        data = (const uint8)(((data << 1) | gpio_get_level(soft_iic_obj->sda_pin)));
    }
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir((gpio_pin_enum)soft_iic_obj->sda_pin, GPO, GPO_OPEN_DTAIN);
#endif
    soft_iic_delay(soft_iic_obj->delay);
    soft_iic_send_ack(soft_iic_obj, ack);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ�д 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     data            Ҫд�������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_8bit_register(soft_iic_obj, 0x01);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit (soft_iic_info_struct *soft_iic_obj, const uint8_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ�д 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     *data           ���ݴ�Ż�����
// ����˵��     len             ����������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_8bit_array(soft_iic_obj, data, 6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_array (soft_iic_info_struct *soft_iic_obj, const uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    while(len --)
    {
        soft_iic_send_data(soft_iic_obj, *data ++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ���д 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     data            Ҫд�������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_16bit(soft_iic_obj, 0x0101);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit (soft_iic_info_struct *soft_iic_obj, const uint16_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((data & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ�д 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     *data           ���ݴ�Ż�����
// ����˵��     len             ����������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_16bit_array(soft_iic_obj, data, 6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_array (soft_iic_info_struct *soft_iic_obj, const uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    while(len --)
    {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*data & 0xFF00) >> 8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*data ++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ��򴫸����Ĵ���д 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     data            Ҫд�������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_8bit_register(soft_iic_obj, 0x01, 0x01);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_register (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, const uint8_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ��򴫸����Ĵ���д 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     *data           ���ݴ�Ż�����
// ����˵��     len             ����������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_8bit_registers(soft_iic_obj, 0x01, data, 6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_registers (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, const uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, register_name);
    while(len --)
    {
        soft_iic_send_data(soft_iic_obj, *data ++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ��򴫸����Ĵ���д 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     data            Ҫд�������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_16bit_register(soft_iic_obj, 0x0101, 0x0101);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_register (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name, const uint16_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((data & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ��򴫸����Ĵ���д 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     *data           ���ݴ�Ż�����
// ����˵��     len             ����������
// ���ز���     void
// ʹ��ʾ��     soft_iic_write_16bit_registers(soft_iic_obj, 0x0101, data, 6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_registers (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name, const uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    while(len --)
    {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*data & 0xFF00) >> 8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*data ++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڶ�ȡ 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ���ز���     uint8           ���ض�ȡ�� 8bit ����
// ʹ��ʾ��     soft_iic_read_8bit(soft_iic_obj);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_read_8bit (soft_iic_info_struct *soft_iic_obj)
{
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     *data           Ҫ��ȡ�����ݵĻ�����ָ��
// ����˵��     len             Ҫ��ȡ�����ݳ���
// ���ز���     void
// ʹ��ʾ��     soft_iic_read_8bit_array(soft_iic_obj, data, 8);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_array (soft_iic_info_struct *soft_iic_obj, uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    while(len --)
    {
        *data ++ = soft_iic_read_data(soft_iic_obj, len == 0);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڶ�ȡ 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ���ز���     uint16          ���ض�ȡ�� 16bit ����
// ʹ��ʾ��     soft_iic_read_16bit(soft_iic_obj);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16_t soft_iic_read_16bit (soft_iic_info_struct *soft_iic_obj)
{
    uint16_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = (const uint8)(((temp << 8)| soft_iic_read_data(soft_iic_obj, 1)));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڶ�ȡ 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     *data           Ҫ��ȡ�����ݵĻ�����ָ��
// ����˵��     len             Ҫ��ȡ�����ݳ���
// ���ز���     void
// ʹ��ʾ��     soft_iic_read_16bit_array(soft_iic_obj, data, 8);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_array (soft_iic_info_struct *soft_iic_obj, uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    while(len --)
    {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = (const uint8)(((*data << 8)| soft_iic_read_data(soft_iic_obj, 0 == len)));
        data ++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ���ز���     uint8           ���ض�ȡ�� 8bit ����
// ʹ��ʾ��     soft_iic_read_8bit_register(soft_iic_obj, 0x01);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_read_8bit_register (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name)
{
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     *data           Ҫ��ȡ�����ݵĻ�����ָ��
// ����˵��     len             Ҫ��ȡ�����ݳ���
// ���ز���     void
// ʹ��ʾ��     soft_iic_read_8bit_registers(soft_iic_obj, 0x01, data, 8);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_registers (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    while(len --)
    {
        *data ++ = soft_iic_read_data(soft_iic_obj, len == 0);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴӴ������Ĵ�����ȡ 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ���ز���     uint16          ���ض�ȡ�� 16bit ����
// ʹ��ʾ��     soft_iic_read_16bit_register(soft_iic_obj, 0x0101);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16_t soft_iic_read_16bit_register (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name)
{
    uint16_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = (const uint8)(((temp << 8)| soft_iic_read_data(soft_iic_obj, 1)));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴӴ������Ĵ�����ȡ 16bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     *data           Ҫ��ȡ�����ݵĻ�����ָ��
// ����˵��     len             Ҫ��ȡ�����ݳ���
// ���ز���     void
// ʹ��ʾ��     soft_iic_read_16bit_registers(soft_iic_obj, 0x0101, data, 8);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_registers (soft_iic_info_struct *soft_iic_obj, const uint16_t register_name, uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    while(len --)
    {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = (const uint8)(((*data << 8)| soft_iic_read_data(soft_iic_obj, 0 == len)));
        data ++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴ��� 8bit ���� ��д���ȡ
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     *write_data     �������ݴ�Ż�����
// ����˵��     write_len       ���ͻ���������
// ����˵��     *read_data      ��ȡ���ݴ�Ż�����
// ����˵��     read_len        ��ȡ����������
// ���ز���     void
// ʹ��ʾ��     iic_transfer_8bit_array(IIC_1, addr, data, 64, data, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_transfer_8bit_array (soft_iic_info_struct *soft_iic_obj, const uint8_t *write_data, uint32_t write_len, uint8_t *read_data, uint32_t read_len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    while(write_len --)
    {
        soft_iic_send_data(soft_iic_obj, *write_data ++);
    }
    if(read_len)
    {
        soft_iic_start(soft_iic_obj);
        soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
        while(read_len --)
        {
            *read_data ++ = soft_iic_read_data(soft_iic_obj, 0 == read_len);
        }
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴ��� 16bit ���� ��д���ȡ
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     *write_data     �������ݴ�Ż�����
// ����˵��     write_len       ���ͻ���������
// ����˵��     *read_data      ��ȡ���ݴ�Ż�����
// ����˵��     read_len        ��ȡ����������
// ���ز���     void
// ʹ��ʾ��     iic_transfer_16bit_array(IIC_1, addr, data, 64, data, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_transfer_16bit_array (soft_iic_info_struct *soft_iic_obj, const uint16_t *write_data, uint32_t write_len, uint16_t *read_data, uint32_t read_len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    while(write_len--)
    {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*write_data & 0xFF00) >> 8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*write_data ++ & 0x00FF));
    }
    if(read_len)
    {
        soft_iic_start(soft_iic_obj);
        soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
        while(read_len --)
        {
            *read_data = soft_iic_read_data(soft_iic_obj, 0);
            *read_data = (const uint8)(((*read_data << 8) | soft_iic_read_data(soft_iic_obj, 0 == read_len)));
            read_data ++;
        }
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ� SCCB ģʽ�򴫸����Ĵ���д 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ����˵��     data            Ҫд�������
// ���ز���     void
// ʹ��ʾ��     soft_iic_sccb_write_register(soft_iic_obj, 0x01, 0x01);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_sccb_write_register (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, uint8_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿ� SCCB ģʽ�Ӵ������Ĵ�����ȡ 8bit ����
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// ����˵��     register_name   �������ļĴ�����ַ
// ���ز���     uint8           ���ض�ȡ�� 8bit ����
// ʹ��ʾ��     soft_iic_sccb_read_register(soft_iic_obj, 0x01);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_sccb_read_register (soft_iic_info_struct *soft_iic_obj, const uint8_t register_name)
{
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_stop(soft_iic_obj);

    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8)(soft_iic_obj->addr << 1 | 0x01));
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڳ�ʼ�� Ĭ�� MASTER ģʽ ���ṩ SLAVE ģʽ
// ����˵��     *soft_iic_obj   ��� IIC ָ����Ϣ��Žṹ���ָ��
// ����˵��     addr            ��� IIC ��ַ ������Ҫע�� ��׼��λ��ַ ���λ���� д��ʱ�����ȷ������
// ����˵��     delay           ��� IIC ��ʱ ����ʱ�Ӹߵ�ƽʱ�� Խ�� IIC ����Խ��
// ����˵��     scl_pin         ��� IIC ʱ������ ���� zf_driver_gpio.h �� gpio_pin_enum ö���嶨��
// ����˵��     sda_pin         ��� IIC �������� ���� zf_driver_gpio.h �� gpio_pin_enum ö���嶨��
// ���ز���     void
// ʹ��ʾ��     soft_iic_init(&soft_iic_obj, addr, 100, B6, B7);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_init (soft_iic_info_struct *soft_iic_obj, uint8_t addr, uint32_t delay, bsp_io_port_pin_t scl_pin, bsp_io_port_pin_t sda_pin)
{
    soft_iic_obj->scl_pin = scl_pin;
    soft_iic_obj->sda_pin = sda_pin;
    system_delay_ms(1);
    soft_iic_obj->addr = addr;
    system_delay_ms(1);
    soft_iic_obj->delay = delay;
    system_delay_ms(1);
}
//-------------------------------------------------------------------------------------------------------------------
// 专门为 MT9V034 增加的 16位写入函数 (8位寄存器地址 + 16位数据)
//-------------------------------------------------------------------------------------------------------------------
uint8_t mt9v03x_force_write_16bit(soft_iic_info_struct *soft_iic_obj, uint8_t reg, uint16_t data)
{
    uint8_t err = 0;
    soft_iic_start(soft_iic_obj);

    // 如果摄像头拒收(NACK)，soft_iic_send_data 会返回 1，我们用 err 累计起来
    err |= soft_iic_send_data(soft_iic_obj, (uint8_t)(soft_iic_obj->addr << 1));
    err |= soft_iic_send_data(soft_iic_obj, reg);
    err |= soft_iic_send_data(soft_iic_obj, (uint8_t)(data >> 8));
    err |= soft_iic_send_data(soft_iic_obj, (uint8_t)(data & 0xFF));

    soft_iic_stop(soft_iic_obj);

    return err; // 返回 0 表示完美写完，返回大于 0 表示摄像头拒收
}
//-------------------------------------------------------------------------------------------------------------------
// 专门为 MT9V034 增加的 16位读取函数 (8位寄存器地址 + 16位数据)
//-------------------------------------------------------------------------------------------------------------------
uint16_t mt9v03x_force_read_16bit(soft_iic_info_struct *soft_iic_obj, uint8_t reg)
{
    uint16_t temp = 0;

    // 1. 发送写地址，定位寄存器
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8_t)(soft_iic_obj->addr << 1));
    soft_iic_send_data(soft_iic_obj, reg); // ⚠️ 仅仅发送 8 位寄存器地址！

    // 2. 重新发起 Start 信号，转为读取模式
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (const uint8_t)((soft_iic_obj->addr << 1) | 0x01));

    // 3. 读取高 8 位 (回复 ACK = 0, 告诉摄像头继续发)
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp <<= 8; // 把高 8 位推到左边

    // 4. 读取低 8 位 (回复 NACK = 1, 告诉摄像头读完了)
    temp |= soft_iic_read_data(soft_iic_obj, 1);

    // 5. 挂断电话
    soft_iic_stop(soft_iic_obj);

    return temp;
}
