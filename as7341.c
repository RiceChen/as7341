#include "as7341.h"

#ifdef PKG_USING_AS7341

#define DBG_ENABLE
#define DBG_SECTION_NAME "as7341"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#include <string.h>

static rt_err_t as7341_write_reg(as7341_device_t dev, rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];
    struct rt_i2c_msg msgs;

    buf[0] = reg;
    buf[1] = data;

    msgs.addr = AS7341_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 2;

    if(rt_i2c_transfer(dev->i2c, &msgs, 1) != 1)
    {
        LOG_E("as7341 write data fail");
        return RT_ERROR;
    }
    return RT_EOK;
}

static rt_err_t as7341_read_regs(as7341_device_t dev, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr = AS7341_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg;
    msgs[0].len = 1;

    msgs[1].addr = AS7341_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = buf;
    msgs[1].len = len;

    if(rt_i2c_transfer(dev->i2c, msgs, 2) != 2)
    {
        LOG_E("as7341 read data fail");
        return RT_ERROR;
    }
    return RT_EOK;
}

rt_int8_t as7341_read_id(as7341_device_t dev)
{
    rt_uint8_t id = 0;
    if(as7341_read_regs(dev, REG_AS7341_ID, 1, &id) == RT_EOK)
    {
        return id;
    }
    else
    {
        return -RT_ERROR;
    }
}

void as7341_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_ENABLE, 1, &data);
    if(enable)
    {
        data |= (1 << 0);
    }
    else
    {
        data &= ~(1 << 0);
    }
    as7341_write_reg(dev, REG_AS7341_ENABLE, data);
}

void as7341_spectral_measure_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_ENABLE, 1, &data);
    if(enable)
    {
        data |= (1 << 1);
    }
    else
    {
        data &= ~(1 << 1);
    }
    as7341_write_reg(dev, REG_AS7341_ENABLE, data);
}

void as7341_wait_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_ENABLE, 1, &data);
    if(enable)
    {
        data |= (1 << 3);
    }
    else
    {
        data &= ~(1 << 3);
    }
    as7341_write_reg(dev, REG_AS7341_ENABLE, data);
}

void as7341_smux_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_ENABLE, 1, &data);
    if(enable)
    {
        data |= (1 << 4);
    }
    else
    {
        data &= ~(1 << 4);
    }
    as7341_write_reg(dev, REG_AS7341_ENABLE, data);
}

void as7341_flicker_detection_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_ENABLE, 1, &data);
    if(enable)
    {
        data |= (1 << 6);
    }
    else
    {
        data &= ~(1 << 6);
    }
    as7341_write_reg(dev, REG_AS7341_ENABLE, data);
}

static rt_bool_t as7341_measure_complete(as7341_device_t dev)
{
    rt_uint8_t statue;
    as7341_read_regs(dev, REG_AS7341_STATUS_2, 1, &statue);

    if(statue & (1 << 6))
    {
        return RT_TRUE;
    }
    return RT_FALSE;
}

void as7341_set_gpio_mode(as7341_device_t dev, GPIO_MODE_t mode)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_GPIO_2, 1, &data);
    if(mode == INPUT)
    {
        data |= (1 << 2);
    }
    else if(mode == OUTPUT)
    {
        data &= ~(1 << 2);
    }
    as7341_write_reg(dev, REG_AS7341_GPIO_2, data);
}

void as7341_set_gpio(as7341_device_t dev, rt_bool_t connect)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_CPIO, 1, &data);
    if(connect)
    {
        data |= (1 << 1);
    } 
    else
    {
        data &= ~(1 << 1);
    }
    as7341_write_reg(dev, REG_AS7341_CPIO, data);
}

void as7341_set_bank(as7341_device_t dev, rt_uint8_t addr)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_CFG_0, 1, &data);
    if(addr)
    {
        data |= (1 << 4);
    }
    else
    {
        data &= ~(1 << 4);
    }
    as7341_write_reg(dev, REG_AS7341_CFG_0, data);
}

void as7341_enable_led(as7341_device_t dev, rt_bool_t on)
{
    rt_uint8_t config_data = 0;
    rt_uint8_t led_data = 0;

    as7341_set_bank(dev, 1);

    as7341_read_regs(dev, REG_AS7341_CONFIG, 1, &config_data);
    as7341_read_regs(dev, REG_AS7341_LED, 1, &led_data);
    if(on)
    {
        config_data |= (1 << 3);
        led_data |= (1 << 7);
    }
    else
    {
        config_data &= ~(1 << 3);
        led_data &= ~(1 << 7);
    }
    as7341_write_reg(dev, REG_AS7341_CONFIG, config_data);
    as7341_write_reg(dev, REG_AS7341_LED, led_data);
    as7341_set_bank(dev, 0);
}

void as7341_control_led(as7341_device_t dev, rt_uint8_t current)
{
    uint8_t data=0;
    if(current < 1) 
    {
        current = 1;
    }
    current--;
    if(current > 19) 
    {
        current = 19;
    }

    as7341_set_bank(dev, 1);
  
    as7341_read_regs(dev, REG_AS7341_LED, 1, &data);
    data |= (1<<7);
    data |= (current & 0x7f);
    as7341_write_reg(dev, REG_AS7341_LED, data);
    rt_thread_delay(100);
    as7341_set_bank(dev, 0);
}

void as7341_config(as7341_device_t dev, MODE_t mode)
{
    rt_uint8_t data = 0;

    as7341_set_bank(dev, 1);

    as7341_read_regs(dev, REG_AS7341_CONFIG, 1, &data);
    switch (mode)
    {
    case eSpm:
        data = (data & (~3)) | eSpm;
        break;
    case eSyns:
        data = (data & (~3)) | eSyns;
        break;
    case eSynd:
        data = (data & (~3)) | eSynd;
        break;
    default:
        break;
    }
    as7341_write_reg(dev, REG_AS7341_CONFIG, data);

    as7341_set_bank(dev, 0);
}

void as7341_f1_f4_clear_nir(as7341_device_t dev)
{
    as7341_write_reg(dev, 0x00, 0x30);
    as7341_write_reg(dev, 0x01, 0x01);
    as7341_write_reg(dev, 0x02, 0x00);
    as7341_write_reg(dev, 0x03, 0x00);
    as7341_write_reg(dev, 0x04, 0x00);
    as7341_write_reg(dev, 0x05, 0x42);
    as7341_write_reg(dev, 0x06, 0x00);
    as7341_write_reg(dev, 0x07, 0x00);
    as7341_write_reg(dev, 0x08, 0x50);
    as7341_write_reg(dev, 0x09, 0x00);
    as7341_write_reg(dev, 0x0A, 0x00);
    as7341_write_reg(dev, 0x0B, 0x00);
    as7341_write_reg(dev, 0x0C, 0x20);
    as7341_write_reg(dev, 0x0D, 0x04);
    as7341_write_reg(dev, 0x0E, 0x00);
    as7341_write_reg(dev, 0x0F, 0x30);
    as7341_write_reg(dev, 0x10, 0x01);
    as7341_write_reg(dev, 0x11, 0x50);
    as7341_write_reg(dev, 0x12, 0x00);
    as7341_write_reg(dev, 0x13, 0x06);   
}

void as7341_f5_f8_clear_nir(as7341_device_t dev)
{
    as7341_write_reg(dev, 0x00, 0x00);
    as7341_write_reg(dev, 0x01, 0x00);
    as7341_write_reg(dev, 0x02, 0x00);
    as7341_write_reg(dev, 0x03, 0x40);
    as7341_write_reg(dev, 0x04, 0x02);
    as7341_write_reg(dev, 0x05, 0x00);
    as7341_write_reg(dev, 0x06, 0x10);
    as7341_write_reg(dev, 0x07, 0x03);
    as7341_write_reg(dev, 0x08, 0x50);
    as7341_write_reg(dev, 0x09, 0x10);
    as7341_write_reg(dev, 0x0A, 0x03);
    as7341_write_reg(dev, 0x0B, 0x00);
    as7341_write_reg(dev, 0x0C, 0x00);
    as7341_write_reg(dev, 0x0D, 0x00);
    as7341_write_reg(dev, 0x0E, 0x24);
    as7341_write_reg(dev, 0x0F, 0x00);
    as7341_write_reg(dev, 0x10, 0x00);
    as7341_write_reg(dev, 0x11, 0x50);
    as7341_write_reg(dev, 0x12, 0x00);
    as7341_write_reg(dev, 0x13, 0x06);
}

void as7341_fd_config(as7341_device_t dev)
{
    as7341_write_reg(dev, 0x00, 0x00);
    as7341_write_reg(dev, 0x01, 0x00);
    as7341_write_reg(dev, 0x02, 0x00);
    as7341_write_reg(dev, 0x03, 0x00);
    as7341_write_reg(dev, 0x04, 0x00);
    as7341_write_reg(dev, 0x05, 0x00);
    as7341_write_reg(dev, 0x06, 0x00);
    as7341_write_reg(dev, 0x07, 0x00);
    as7341_write_reg(dev, 0x08, 0x00);
    as7341_write_reg(dev, 0x09, 0x00);
    as7341_write_reg(dev, 0x0A, 0x00);
    as7341_write_reg(dev, 0x0B, 0x00);
    as7341_write_reg(dev, 0x0C, 0x00);
    as7341_write_reg(dev, 0x0D, 0x00);
    as7341_write_reg(dev, 0x0E, 0x00);
    as7341_write_reg(dev, 0x0F, 0x00);
    as7341_write_reg(dev, 0x10, 0x00);
    as7341_write_reg(dev, 0x11, 0x00);
    as7341_write_reg(dev, 0x12, 0x00);
    as7341_write_reg(dev, 0x13, 0x60);
}

void as7341_start_measure(as7341_device_t dev, CH_CHOOSE_t mode)
{
    rt_uint8_t data = 0;
  
    as7341_read_regs(dev, REG_AS7341_CFG_0, 1, &data);
    data &= ~(1<<4);
    as7341_write_reg(dev, REG_AS7341_CFG_0, data);
    
    as7341_spectral_measure_enable(dev, RT_FALSE);

    data = 0x10;
    as7341_write_reg(dev, REG_AS7341_CFG_6, data);

    if(mode == eF1F4ClearNIR)
    {
        as7341_f1_f4_clear_nir(dev);
    }
    else if(mode  == eF5F8ClearNIR)
    {
        as7341_f5_f8_clear_nir(dev);
    }

    as7341_smux_enable(dev, RT_TRUE);
    if(dev->measure_mode == eSyns)
    {
        as7341_set_gpio_mode(dev, INPUT);
        as7341_config(dev, eSyns);
    }
    else if(dev->measure_mode == eSpm)
    {
        as7341_config(dev, eSpm);
    }
    as7341_spectral_measure_enable(dev, RT_TRUE);

    if(dev->measure_mode == eSpm){
        while(!as7341_measure_complete(dev))
		{
            rt_thread_delay(1);
        }
    }
}

rt_uint8_t as7341_read_flicker_data(as7341_device_t dev)
{
    rt_uint8_t flicker = 0;
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_CFG_0, 1, &data);
    data &= ~(1<<4);
    as7341_write_reg(dev, REG_AS7341_CFG_0, data);

    as7341_spectral_measure_enable(dev, RT_FALSE);

    data = 0x10;
    as7341_write_reg(dev, REG_AS7341_CFG_6, data);

    as7341_fd_config(dev);
    as7341_smux_enable(dev, RT_TRUE);
    as7341_spectral_measure_enable(dev, RT_TRUE);
    
    as7341_flicker_detection_enable(dev, RT_TRUE);
    rt_thread_delay(600);
    as7341_read_regs(dev, REG_AS7341_STATUS, 1, &flicker);
    as7341_flicker_detection_enable(dev, RT_FALSE);

    switch(flicker)
    {
        case 44:
            flicker = 1;
            break;
        case 45:
            flicker = 50;
            break;
        case 46:
            flicker = 60;
            break;
        default:
            flicker = 0;
            break;
    }
    return flicker;
}

rt_uint16_t as7341_get_channel_data(as7341_device_t dev, rt_uint8_t channel)
{
    rt_uint8_t data[2] = {0};
    rt_uint16_t ch_data = 0x0000;

    as7341_read_regs(dev, REG_AS7341_CH0_DATA_L + channel * 2, 1, &data[0]);
    as7341_read_regs(dev, REG_AS7341_CH0_DATA_H + channel * 2, 1, &data[1]);

    ch_data = data[1];
    ch_data = (ch_data << 8) | data[0];
    rt_thread_delay(50);
    return ch_data;
}

MODE_ONE_DATA_t as7341_read_spectral_data_one(as7341_device_t dev)
{
    MODE_ONE_DATA_t data;
    data.ADF1 = as7341_get_channel_data(dev, 0);
    data.ADF2 = as7341_get_channel_data(dev, 1);
    data.ADF3 = as7341_get_channel_data(dev, 2);
    data.ADF4 = as7341_get_channel_data(dev, 3);
    data.ADCLEAR = as7341_get_channel_data(dev, 4);
    data.ADNIR = as7341_get_channel_data(dev, 5);
    return data;
}

MODE_TOW_DATA_t as7341_read_spectral_data_tow(as7341_device_t dev)
{
    MODE_TOW_DATA_t data;
    data.ADF5 = as7341_get_channel_data(dev, 0);
    data.ADF6 = as7341_get_channel_data(dev, 1);
    data.ADF7 = as7341_get_channel_data(dev, 2);
    data.ADF8 = as7341_get_channel_data(dev, 3);
    data.ADCLEAR = as7341_get_channel_data(dev, 4);
    data.ADNIR = as7341_get_channel_data(dev, 5);
    return data;
}

void as7341_set_gpio_interrupt(as7341_device_t dev, rt_bool_t connect)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_CPIO, 1, &data);
    if(connect)
    {
        data |= (1 << 1);
    }
    else
    {
        data &= ~(1 << 1);
    }
    as7341_write_reg(dev, REG_AS7341_CPIO, data);
}

void as7341_sys_int_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_INTENAB, 1, &data);
    if(enable)
    {
        data |= (1 << 0);
    }
    else
    {
        data &= ~(1 << 0);
    }
    as7341_write_reg(dev, REG_AS7341_INTENAB, data);
}

void as7341_fifo_init_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_INTENAB, 1, &data);
    if(enable)
    {
        data |= (1 << 2);
    }
    else
    {
        data &= ~(1 << 2);
    }
    as7341_write_reg(dev, REG_AS7341_INTENAB, data);
}

void as7341_spectral_init_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_INTENAB, 1, &data);
    if(enable)
    {
        data |= (1 << 3);
    }
    else
    {
        data &= ~(1 << 3);
    }
    as7341_write_reg(dev, REG_AS7341_INTENAB, data);
}

void as7341_clear_fifo(as7341_device_t dev)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_CONTROL, 1, &data);
    data |= (1 << 1);
    data &= ~(1 << 1);
    as7341_write_reg(dev, REG_AS7341_CONTROL, data);
}

void as7341_spectral_auto_zero(as7341_device_t dev)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_CONTROL, 1, &data);
    data |= (1 << 2);
    as7341_write_reg(dev, REG_AS7341_CONTROL, data);
}

void as7341_flicker_int_enable(as7341_device_t dev, rt_bool_t enable)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_INTENAB, 1, &data);
    if(enable)
    {
        data |= (1 << 2);
    }
    else
    {
        data &= ~(1 << 2);
    }
    as7341_write_reg(dev, REG_AS7341_INTENAB, data);
}

void as7341_set_atime(as7341_device_t dev, rt_uint8_t value)
{
    as7341_write_reg(dev, REG_AS7341_ATIME, value);
}

void as7341_set_again(as7341_device_t dev, rt_uint8_t value)
{
    if(value > 10)
    {
        value = 10;
    }
    as7341_write_reg(dev, REG_AS7341_CFG_1, value);
}

void as7341_set_astep(as7341_device_t dev, rt_uint16_t value)
{
    rt_uint8_t high_value, low_value;
    low_value  = value & 0x00ff;
    high_value = value >> 8 ;
    as7341_write_reg(dev, REG_AS7341_ASTEP_L, low_value);
    as7341_write_reg(dev, REG_AS7341_ASTEP_H, high_value);
}

float as7341_get_integration_time(as7341_device_t dev)
{
    rt_uint8_t data;
    rt_uint8_t astep_Data[2]={0};
    rt_uint16_t astep;
    as7341_read_regs(dev, REG_AS7341_ATIME, 1, &data);
    as7341_read_regs(dev, REG_AS7341_ASTEP_L, 1, &astep_Data[0]);
    as7341_read_regs(dev, REG_AS7341_ASTEP_L, 1, &astep_Data[1]);
    
    astep = astep_Data[1];
    astep = (astep << 8) | astep_Data[0];
    if(data == 0){

    }
    else if(data >0 && data <255){
        
    }
    else if(data == 255){
        
    }
	return 0;
}

void as7341_set_wtime(as7341_device_t dev, rt_uint8_t value)
{
    as7341_write_reg(dev, REG_AS7341_WTIME, value);
}

float as7341_get_wtime(as7341_device_t dev)
{
    float value;
    rt_uint8_t data;
    as7341_read_regs(dev, REG_AS7341_WTIME, 1, &data);
    if(data == 0)
    {
        value = 2.78;
    }
    else if(data == 1)
    {
        value = 5.56;
    }
    else if(data > 1 && data < 255)
    {
        value = 2.78 * (data + 1);
    }
    else if(data == 255)
    {
        value = 711;
    }
    return value;
}

void as7341_set_threshold(as7341_device_t dev, uint16_t low_threshold, uint16_t high_threshold)
{
    if(low_threshold >= high_threshold)
    {
        return;
    }

    as7341_write_reg(dev, REG_AS7341_SP_TH_H_MSB, high_threshold >> 8);
    as7341_write_reg(dev, REG_AS7341_SP_TH_H_LSB, high_threshold);

    as7341_write_reg(dev, REG_AS7341_SP_TH_L_MSB, low_threshold >> 8);
    as7341_write_reg(dev, REG_AS7341_SP_TH_L_LSB, low_threshold);

    rt_thread_delay(20);
}

rt_uint16_t as7341_get_low_threshold(as7341_device_t dev)
{
    rt_uint8_t data[2] = {0};
    rt_uint16_t th_data = 0x0000;

    as7341_read_regs(dev, REG_AS7341_SP_TH_L_MSB, 1, &data[0]);
    as7341_read_regs(dev, REG_AS7341_SP_TH_L_LSB, 1, &data[1]);

    th_data = data[1];
    th_data = (th_data << 8) | data[0];
    return th_data; 
}

rt_uint16_t as7341_get_high_threshold(as7341_device_t dev)
{
    rt_uint8_t data[2] = {0};
    rt_uint16_t th_data = 0x0000;

    as7341_read_regs(dev, REG_AS7341_SP_TH_H_MSB, 1, &data[0]);
    as7341_read_regs(dev, REG_AS7341_SP_TH_H_LSB, 1, &data[1]);

    th_data = data[1];
    th_data = (th_data << 8) | data[0];
    return th_data; 
}

void as7341_clean_interrupt(as7341_device_t dev)
{
    as7341_write_reg(dev, REG_AS7341_STATUS_1, 0xff);
}

void as7341_set_channel_int(as7341_device_t dev, rt_uint8_t channel)
{
    rt_uint8_t data = 0;

    if(channel >= 5)
    {
        return;
    }

    as7341_read_regs(dev, REG_AS7341_CFG_12, 1, &data);
    data &= ~(7);
    data |= channel;
    as7341_write_reg(dev, REG_AS7341_CFG_12, data);
}

void as7341_set_apers(as7341_device_t dev, rt_uint8_t num)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_PERS, 1, &data);
    data &= ~(15);
    data |= num;
    as7341_write_reg(dev, REG_AS7341_PERS, data);
}

rt_uint8_t as7341_get_int_source(as7341_device_t dev)
{
    rt_uint8_t data = 0;

    as7341_read_regs(dev, REG_AS7341_STATUS_3, 1, &data);
    return data;
}

rt_bool_t as7341_get_interrupt(as7341_device_t dev)
{
    rt_uint8_t data = 0;
    as7341_read_regs(dev, REG_AS7341_STATUS_1, 1, &data);
    if(data & 0x80)
    {
        return RT_TRUE;
    }
    else
    {
        return RT_FALSE;
    }
}

rt_bool_t as7341_check_wtime(as7341_device_t dev)
{
    rt_uint8_t status = 0;
    rt_uint8_t value = 0;
    as7341_read_regs(dev, REG_AS7341_STATUS_6, 1, &status);
    value = status & (1 << 2);
    if(value > 0)
    {
        return RT_FALSE;
    }
    else
    {
        return RT_TRUE;
    }
}

as7341_device_t as7341_init(const char *i2c_bus_name, MODE_t measure_mode)
{
    as7341_device_t dev = RT_NULL;
    rt_uint8_t as7341_id = 0;

    RT_ASSERT(i2c_bus_name);

    dev = rt_calloc(1, sizeof(struct as7341_device));
    if (dev == RT_NULL)
    {
        LOG_E("Can't allocate memory for as7341 device on '%s' ", i2c_bus_name);
        return RT_NULL;
    }
   
    dev->i2c = rt_i2c_bus_device_find(i2c_bus_name);
    if (dev->i2c == RT_NULL)
    {
        LOG_E("Can't find as7341 device on '%s' ", i2c_bus_name);
        rt_free(dev);
        return RT_NULL;
    }

    dev->measure_mode = measure_mode;

	as7341_enable(dev, RT_TRUE);

    as7341_id = as7341_read_id(dev);

    rt_kprintf("as7341 id: 0x%2x\n", as7341_id);

    return dev;
}

void as7341_deinit(as7341_device_t dev)
{
    RT_ASSERT(dev);

    rt_free(dev);
}

static void as7341_usage(void)
{
    rt_kprintf("Usage:\n");
    rt_kprintf("\tas7341 probe <i2c dev name> -- probe sensor by i2c dev name\n");
    rt_kprintf("\tas7341 read spectral        -- read sensor as7341 spectral data\n");
    rt_kprintf("\tas7341 read flicker         -- read sensor as7341 flicker data\n");
}

static void as7341(int argc, char *argv[])
{
    static as7341_device_t dev = RT_NULL;
    
    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            if (argc >= 3)
            {
                /* initialize the sensor when first probe */
                if (!dev || strcmp(dev->i2c->parent.parent.name, argv[2]))
                {
                    /* deinit the old device */
                    if(dev)
                    {
						rt_kprintf("Deinit as7341\n");
                        as7341_deinit(dev);
                    }

                    dev = as7341_init(argv[2], eSpm);
                    if(!dev)
                    {
                        rt_kprintf("as7341 probe failed, check input args\n");
                    }else
					{
						rt_kprintf("as7341 probed, addr:0x%x\n", AS7341_ADDR) ;
					}
                }
            }
            else
            {
                as7341_usage();
            }
        }
        else if (!strcmp(argv[1], "read"))
        {
            if (dev)
            {
                if(!strcmp(argv[2], "spectral"))
                {
                    MODE_ONE_DATA_t data1;
                    MODE_TOW_DATA_t data2;

                    as7341_start_measure(dev, eF1F4ClearNIR);
                    data1 = as7341_read_spectral_data_one(dev);
                    rt_kprintf("F1(405-425nm): %d\n", data1.ADF1);
                    rt_kprintf("F2(435-455nm): %d\n", data1.ADF2);
                    rt_kprintf("F3(470-490nm): %d\n", data1.ADF3);
                    rt_kprintf("F4(505-525nm): %d\n", data1.ADF4);

                    as7341_start_measure(dev, eF5F8ClearNIR);
                    data2 = as7341_read_spectral_data_tow(dev);
                    rt_kprintf("F5(545-565nm): %d\n", data2.ADF5);
                    rt_kprintf("F6(580-600nm): %d\n", data2.ADF6);
                    rt_kprintf("F7(620-640nm): %d\n", data2.ADF7);
                    rt_kprintf("F8(670-690nm): %d\n", data2.ADF8);

                    rt_kprintf("Clear: %d\n", data2.ADCLEAR);
                    rt_kprintf("NIR: %d\n", data2.ADNIR);
                }
                else if(!strcmp(argv[2], "flicker"))
                {
                    rt_uint8_t freq = 0;
                    freq = as7341_read_flicker_data(dev);
                    if(freq == 1)
                    {
                        rt_kprintf("Unknown frequency\n");
                    }
                    else if(freq == 0)
                    {
                        rt_kprintf("No flicker\n");
                    }
                    else
                    {
                        rt_kprintf("freq: %dHz\n", freq);
                    }
                }
                else
                {
                    as7341_usage();
                }
                
            }
            else
            {
                rt_kprintf("Please using 'as7341 probe <i2c dev name>' first\n");
            }
        }
        else
        {
            as7341_usage();
        }
    }
    else
    {
        as7341_usage();   
    }
}

MSH_CMD_EXPORT(as7341, as7341 sensor);

#endif