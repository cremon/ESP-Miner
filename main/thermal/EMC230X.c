#include "esp_log.h"
#include <stdio.h>

#include "EMC230X.h"

static const char * TAG = "EMC230x";

static i2c_master_dev_handle_t emc230x_dev_handle;

// void EMC230X_start()
// {
//   MIN_RPM_MULTIPLIER   = 1;
//   MULTIPLIER           = 1;
//   POLES           = 2;
// }

// run this first. sets up the config register
esp_err_t EMC230X_init(int product, EMC230X_FAN fan)
{
	uint8_t product_ID;
	uint8_t Manufacturer_ID;
    uint8_t silicon_Revision;

    if (i2c_bitaxe_add_device(EMC230X_ADDRESS, &emc230x_dev_handle, TAG) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device");
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(i2c_bitaxe_register_read(emc230x_dev_handle, PRODUCT_ID, &product_ID, 1));
    ESP_ERROR_CHECK(i2c_bitaxe_register_read(emc230x_dev_handle, MANUFACTURER_ID, &Manufacturer_ID, 1));
    ESP_ERROR_CHECK(i2c_bitaxe_register_read(emc230x_dev_handle, REVISION, &silicon_Revision, 1));
    ESP_LOGI(TAG, "MANUFACTURER_ID=0x%02X PRODUCT_ID=0x%02X SILICON_REVISION=0x%02x", Manufacturer_ID, product_ID, silicon_Revision);

    //LV07 - EMC2302
	if(Manufacturer_ID == EMC230X_MANUFACTURER && product_ID == product) {
        EMC230X_get_fan_speed(fan);
		EMC230X_set_fan_speed(fan, 1.0);
	} else {
		ESP_LOGI(TAG,"chip not found!");
        ESP_ERROR_CHECK(ESP_FAIL);
	}
    return ESP_OK;
}

// takes a fan speed percent
void EMC230X_set_fan_speed(EMC230X_FAN fan, float percent)
{
    uint8_t pwm;
    pwm = (uint8_t) (255 * percent);
    //min 10%
    if(pwm < 25) pwm = 25;
    
    ESP_ERROR_CHECK(i2c_bitaxe_register_write_byte(emc230x_dev_handle, EMC230X_FAN_CONFIG2 + fan * 0x10, 0x40));
    ESP_ERROR_CHECK(i2c_bitaxe_register_write_byte(emc230x_dev_handle, EMC230X_FAN_CONFIG1 + fan * 0x10, EMC230X_FANCONFIG1_FANPOLES_2));
    ESP_ERROR_CHECK(i2c_bitaxe_register_write_byte(emc230x_dev_handle, EMC230X_FAN_SETTING + fan * 0x10, pwm));
    ESP_LOGI(TAG, "Set Fan[%d] Speed (%f%%) pwm=0x%02X", fan, percent * 100, pwm);
}

// RPM = 5400000/reading
uint16_t EMC230X_get_fan_speed(EMC230X_FAN fan)
{
    uint8_t tach_lsb, tach_msb;
    uint16_t channel_tach;
    uint16_t fan_rpm;

    ESP_ERROR_CHECK(i2c_bitaxe_register_read(emc230x_dev_handle, EMC230X_TACH_READMSB + fan * 0x10, &tach_msb, 1));
    ESP_ERROR_CHECK(i2c_bitaxe_register_read(emc230x_dev_handle, EMC230X_TACH_READLSB + fan * 0x10, &tach_lsb, 1));

    channel_tach = (tach_lsb | (tach_msb << 8)) >> 3;

    ESP_LOGI(TAG, "Raw Fan[%d] TACH MSB=0x%02X LSB=0x%02X %04x", fan, tach_msb, tach_lsb, channel_tach);

	fan_rpm = (FAN_RPM_FACTOR * FAN_TACH_MULTIPLIER) / channel_tach;

    ESP_LOGI(TAG, "Fan[%d] Speed = %d RPM", fan, fan_rpm);

    if (fan_rpm < 100) {
        return 0;
    }
    return fan_rpm;
}