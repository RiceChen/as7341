#ifndef __AS7341_H__
#define __AS7341_H__

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#define AS7341_ADDR            0X39

#define REG_AS7341_CONFIG      0X70
#define REG_AS7341_STAT        0X71
#define REG_AS7341_EDGE        0X72
#define REG_AS7341_CPIO        0X73
#define REG_AS7341_LED         0X74

#define REG_AS7341_ENABLE      0X80
#define REG_AS7341_ATIME       0X81
#define REG_AS7341_WTIME       0X83

#define REG_AS7341_SP_TH_L_LSB 0X84
#define REG_AS7341_SP_TH_L_MSB 0X85
#define REG_AS7341_SP_TH_H_LSB 0X86
#define REG_AS7341_SP_TH_H_MSB 0X87
#define REG_AS7341_AUXID       0X90
#define REG_AS7341_REVID       0X91

#define REG_AS7341_ID          0X92
#define REG_AS7341_STATUS_1    0X93
#define REG_AS7341_ASTATUS     0X94

#define REG_AS7341_CH0_DATA_L  0X95
#define REG_AS7341_CH0_DATA_H  0X96
#define REG_AS7341_CH1_DATA_L  0X97
#define REG_AS7341_CH1_DATA_H  0X98
#define REG_AS7341_CH2_DATA_L  0X99
#define REG_AS7341_CH2_DATA_H  0X9A
#define REG_AS7341_CH3_DATA_L  0X9B
#define REG_AS7341_CH3_DATA_H  0X9C
#define REG_AS7341_CH4_DATA_L  0X9D
#define REG_AS7341_CH4_DATA_H  0X9E
#define REG_AS7341_CH5_DATA_L  0X9F
#define REG_AS7341_CH5_DATA_H  0XA0

#define REG_AS7341_STATUS_2    0XA3
#define REG_AS7341_STATUS_3    0XA4
#define REG_AS7341_STATUS_5    0XA6
#define REG_AS7341_STATUS_6    0XA7
#define REG_AS7341_CFG_0       0XA9
#define REG_AS7341_CFG_1       0XAA
#define REG_AS7341_CFG_3       0XAC
#define REG_AS7341_CFG_6       0XAF
#define REG_AS7341_CFG_8       0XB1
#define REG_AS7341_CFG_9       0XB2
#define REG_AS7341_CFG_10      0XB3
#define REG_AS7341_CFG_12      0XB5

#define REG_AS7341_PERS          0XBD
#define REG_AS7341_GPIO_2        0XBE
#define REG_AS7341_ASTEP_L       0XCA
#define REG_AS7341_ASTEP_H       0XCB
#define REG_AS7341_AGC_GAIN_MAX  0XCF
#define REG_AS7341_AZ_CONFIG     0XD6
#define REG_AS7341_FD_TIME_1     0XD8
#define REG_AS7341_TIME_2        0XDA
#define REG_AS7341_CFG0          0XD7
#define REG_AS7341_STATUS        0XDB
#define REG_AS7341_INTENAB       0XF9
#define REG_AS7341_CONTROL       0XFA
#define REG_AS7341_FIFO_MAP      0XFC
#define REG_AS7341_FIFO_LVL      0XFD
#define REG_AS7341_FDATA_L       0XFE
#define REG_AS7341_FDATA_H       0XFF

#define AS7341_GPIO               4

/**
GPIO MODE 
*/
typedef enum {
    INPUT,
    OUTPUT,
}GPIO_MODE_t;

/**
The measurement mode of spectrum snesor 
*/
typedef enum {
    eSpm = 0,/**<SPM>*/
    eSyns = 1,/**<SYNS>*/
    eSynd = 3,/**<SYND>*/
}MODE_t;

/**
The modes of channel mapping 
*/
typedef enum{
    eF1F4ClearNIR,/**<Map the values of the registers of 6 channels to F1,F2,F3,F4,clear,NIR>*/
    eF5F8ClearNIR,/**<Map the values of the registers of 6 channels to F5,F6,F7,F8,clear,NIR>*/
}CH_CHOOSE_t;

/**
Represent 10 different photodiode measurement channels 
*/
typedef enum{
    eCH_F1,
    eCH_F2,
    eCH_F3,
    eCH_F4,
    eCH_F5,
    eCH_F6,
    eCH_F7,
    eCH_F8,
    eCH_CLEAR,
    eCH_NIR,
}CHANNAL_t;

/**
The values of the registers of 6 channels under eF1F4ClearNIR
*/
typedef struct{
    uint16_t ADF1;/**<F1 diode data>*/
    uint16_t ADF2;/**<F2 diode data>*/
    uint16_t ADF3;/**<F3 diode data>*/
    uint16_t ADF4;/**<F4 diode data>*/
    uint16_t ADCLEAR;/**<clear diode data>*/
    uint16_t ADNIR;/**<NIR diode data>*/
}MODE_ONE_DATA_t;

/**
The values of the registers of 6 channels under eF5F8ClearNIR
*/
typedef struct{
    uint16_t ADF5;/**<F5 diode data>*/
    uint16_t ADF6;/**<F6 diode data>*/
    uint16_t ADF7;/**<F7 diode data>*/
    uint16_t ADF8;/**<F8 diode data>*/
    uint16_t ADCLEAR;/**<clear diode data>*/
    uint16_t ADNIR;/**<NIR diode data>*/
}MODE_TOW_DATA_t;

struct as7341_device
{
    struct rt_i2c_bus_device *i2c;
    rt_uint8_t as7341_addr;

    MODE_t measure_mode;
};
typedef struct as7341_device *as7341_device_t;

/**
 * This function read as7341 id.
 *
 * @param dev the name of as7341 device
 * 
 * @return as7341 id
 */
rt_int8_t as7341_read_id(as7341_device_t dev);

/**
 * This function power on.
 *
 * @param dev the name of as7341 device
 * @param enable 0: AS7341 disabled
 *               1: AS7341 enabled
 */
void as7341_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function spectral measurement enable.
 *
 * @param dev the name of as7341 device
 * @param enable 0: Spectral Measurement Disabled
 *               1: Spectral Measurement Enabled
 */
void as7341_spectral_measure_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function wait enable.
 *
 * @param dev the name of as7341 device
 * @param enable 0: Wait time between two consecutive spectral measurements disabled
 *               1: Wait time between two consecutive spectral measurements enabled
 */
void as7341_wait_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function SMUX Enable.
 *
 * @param dev the name of as7341 device
 * @param enable 1: Starts SMUX command
 */
void as7341_smux_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function flicker detection enable.
 *
 * @param dev the name of as7341 device
 * @param enable 0: Flicker Detection disabled
 *               1: Flicker Detection enabled
 */
void as7341_flicker_detection_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function set gpio mode.
 *
 * @param dev the name of as7341 device
 * @param mode gpio mode
 */
void as7341_set_gpio_mode(as7341_device_t dev, GPIO_MODE_t mode);

/**
 * This function set gpio.
 *
 * @param dev the name of as7341 device
 * @param connect 1: photo diode connected to pin GPIO
 */
void as7341_set_gpio(as7341_device_t dev, rt_bool_t connect);

/**
 * This function register bank access.
 *
 * @param dev the name of as7341 device
 * @param addr 0: Register access to register 0x80 and above
 *             1: Register access to register 0x60 to 0x74
 */
void as7341_set_bank(as7341_device_t dev, rt_uint8_t addr);

/**
 * This function set led state.
 *
 * @param dev the name of as7341 device
 * @param on len state
 */
void as7341_enable_led(as7341_device_t dev, rt_bool_t on);

/**
 * This function led control.
 *
 * @param dev the name of as7341 device
 * @param current LED driving strength.
 */
void as7341_control_led(as7341_device_t dev, rt_uint8_t current);

/**
 * This function set ambient light sensing mode.
 *
 * @param dev the name of as7341 device
 * @param mode ambient light sensing mode
 */
void as7341_config(as7341_device_t dev, MODE_t mode);

/**
 * This function clear f1-f4 channel nir.
 *
 * @param dev the name of as7341 device
 */
void as7341_f1_f4_clear_nir(as7341_device_t dev);

/**
 * This function clear f5-f8 channel nir.
 *
 * @param dev the name of as7341 device
 */
void as7341_f5_f8_clear_nir(as7341_device_t dev);

void as7341_fd_config(as7341_device_t dev);

/**
 * This function start measure.
 *
 * @param dev the name of as7341 device
 * @param mode channel mode
 */
void as7341_start_measure(as7341_device_t dev, CH_CHOOSE_t mode);

/**
 * This function get flicker data.
 *
 * @param dev the name of as7341 device
 * 
 * @return flicker data.
 */
rt_uint8_t as7341_read_flicker_data(as7341_device_t dev);

/**
 * This function get channel data.
 *
 * @param dev the name of as7341 device
 * 
 * @return spectral data.
 */
rt_uint16_t as7341_get_channel_data(as7341_device_t dev, rt_uint8_t channel);

/**
 * This function get spectral data.
 *
 * @param dev the name of as7341 device
 * 
 * @return spectral data.
 */
MODE_ONE_DATA_t as7341_read_spectral_data_one(as7341_device_t dev);

/**
 * This function get spectral data.
 *
 * @param dev the name of as7341 device
 * 
 * @return spectral data.
 */
MODE_TOW_DATA_t as7341_read_spectral_data_tow(as7341_device_t dev);

/**
 * This function set gpio interrupt state.
 *
 * @param dev the name of as7341 device
 * @param connect interrupt state
 */
void as7341_set_gpio_interrupt(as7341_device_t dev, rt_bool_t connect);

/**
 * This function set system interrupt state.
 *
 * @param dev the name of as7341 device
 * @param enable interrupt state
 */
void as7341_sys_int_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function set fifo interrupt state.
 *
 * @param dev the name of as7341 device
 * @param enable interrupt state
 */
void as7341_fifo_init_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function set spectral interrupt state.
 *
 * @param dev the name of as7341 device
 * @param enable interrupt state
 */
void as7341_spectral_init_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function FIFO buffer clear.
 *
 * @param dev the name of as7341 device
 */
void as7341_clear_fifo(as7341_device_t dev);

/**
 * This function spectral engine manual autozero.
 *
 * @param dev the name of as7341 device
 */
void as7341_spectral_auto_zero(as7341_device_t dev);

/**
 * This function set flicker interrupt state.
 *
 * @param dev the name of as7341 device
 * @param enable interrupt state
 */
void as7341_flicker_int_enable(as7341_device_t dev, rt_bool_t enable);

/**
 * This function set integration time.
 *
 * @param dev the name of as7341 device
 * @param value integration time
 */
void as7341_set_atime(as7341_device_t dev, rt_uint8_t value);

/**
 * This function set spectral engines gain setting.
 *
 * @param dev the name of as7341 device
 * @param value spectral engines gains
 */
void as7341_set_again(as7341_device_t dev, rt_uint8_t value);

/**
 * This function set integration time step size.
 *
 * @param dev the name of as7341 device
 * @param value integration time step size
 */
void as7341_set_astep(as7341_device_t dev, rt_uint16_t value);

/**
 * This function get integration time step size.
 *
 * @param dev the name of as7341 device
 * 
 * @return size.
 */
float as7341_get_integration_time(as7341_device_t dev);

/**
 * This function set spectral measurement wait time.
 *
 * @param dev the name of as7341 device
 * @param value wait time value
 */
void as7341_set_wtime(as7341_device_t dev, rt_uint8_t value);

/**
 * This function get spectral measurement wait time.
 *
 * @param dev the name of as7341 device
 * 
 * @return wait time.
 */
float as7341_get_wtime(as7341_device_t dev);

/**
 * This function set low and hight threshold.
 *
 * @param dev the name of as7341 device
 * @param low_threshold low threshold
 * @param high_threshold hight threshold
 */
void as7341_set_threshold(as7341_device_t dev, uint16_t low_threshold, uint16_t high_threshold);

/**
 * This function get low threshold.
 *
 * @param dev the name of as7341 device
 */
rt_uint16_t as7341_get_low_threshold(as7341_device_t dev);

/**
 * This function get hight threshold.
 *
 * @param dev the name of as7341 device
 */
rt_uint16_t as7341_get_high_threshold(as7341_device_t dev);

/**
 * This function clean interrupt.
 *
 * @param dev the name of as7341 device
 */
void as7341_clean_interrupt(as7341_device_t dev);

/**
 * This function set spectral threshold channel.
 *
 * @param dev the name of as7341 device
 * @param channel channel num
 */
void as7341_set_channel_int(as7341_device_t dev, rt_uint8_t channel);

/**
 * This function set spectral interrupt persistence.
 *
 * @param dev the name of as7341 device
 * @param num 0 ~ 15
 */
void as7341_set_apers(as7341_device_t dev, rt_uint8_t num);

/**
 * This function get interrupt soucrce.
 *
 * @param dev the name of as7341 device
 *
 * @return interrupt source.
 */
rt_uint8_t as7341_get_int_source(as7341_device_t dev);

/**
 * This function get interrupt status.
 *
 * @param dev the name of as7341 device
 *
 * @return interrupt status.
 */
rt_bool_t as7341_get_interrupt(as7341_device_t dev);

/**
 * This function is check spectral trigger error.
 *
 * @param dev the name of as7341 device
 *
 * @return spectral trigger error.
 */
rt_bool_t as7341_check_wtime(as7341_device_t dev);

/**
 * This function initializes as7341 registered device driver
 *
 * @param dev the name of as7341 device
 *
 * @param measure_mode as7341 spectral measurement mode
 * @return the as7341 device.
 */
as7341_device_t as7341_init(const char *i2c_bus_name, MODE_t measure_mode);

/**
 * This function releases memory
 *
 * @param dev the pointer of device driver structure
 */
void as7341_deinit(as7341_device_t dev);

#endif /* _AS7341_H__ */
