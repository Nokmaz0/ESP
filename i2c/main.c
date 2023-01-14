#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
//#include "led_strip.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "FreeRTOSConfig.h"
#include "driver/i2c.h"

//#include "driver/adc.h"  // biblioteka do adc


//#include "nok_i2c.h"

#define LED1 2
#define LED2 4

#define address 0b01110000 // dla write
// define address 0b01110001 // dla read

// przy pcf8574an od texasa kod działa
// przy pcf8574ap od nxp nie chce działac


void app_main()
{

//	gpio_set_direction(2, GPIO_MODE_OUTPUT);
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = 18;
	conf.sda_pullup_en = true;
	conf.scl_io_num = 19;
	conf.scl_pullup_en = true;
	conf.master.clk_speed = 100000;
//	conf.clk_flags = 0;


	i2c_driver_install(I2C_NUM_1, I2C_MODE_MASTER, 0, 0, 0);
	i2c_param_config(I2C_NUM_1, &conf);

	i2c_cmd_handle_t cmd;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, address, 1);
	i2c_master_write_byte(cmd, 0b00000000, 1); // wyzeruj wszystkie porty 0-7
	i2c_master_write_byte(cmd, 0b00000001, 1); // ustaw PD0 jako high
	i2c_master_stop(cmd);
	esp_err_t res = i2c_master_cmd_begin(I2C_NUM_1, cmd, 10);
	if(res == ESP_OK) printf("ok1\n");
	if(res == ESP_ERR_INVALID_ARG) printf("ok2\n");
	if(res == ESP_FAIL) printf("ok3\n"); // caly czas sie to wyswietla
	if(res == ESP_ERR_INVALID_STATE) printf("ok4\n");
	if(res == ESP_ERR_TIMEOUT) printf("ok5\n");

	i2c_cmd_link_delete(cmd);
	i2c_driver_delete(I2C_NUM_1);



	while(1){

		vTaskDelete(NULL);

	}


}


