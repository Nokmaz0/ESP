#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define samples 200

int val= 0;
int vout = 0;
uint vout2 = 0;
int v = 0;
int v1 =0;

//gpio 36


void app_main(void){

	adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
	adc1_config_width(ADC_WIDTH_BIT_12);

//	esp_adc_cal_characteristics_t conf;
//	conf.adc_num = ADC1_CHANNEL_0;
//	conf.atten = ADC_ATTEN_DB_11;
//	conf.bit_width = ADC_WIDTH_BIT_12;
//
//	esp_adc_cal_characterize(ADC1_CHANNEL_0, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &conf);

	while(1){

		//multisampling
		for(int i = 0; i<samples; i++){
			val = adc1_get_raw(ADC1_CHANNEL_0);
			vout += val;
		}
		vout2 = vout/samples;
		v = vout2 * 3300/4096 +150;
		if(vout2>3600) v-= 150;
		if(v == 150) v=0;
//		v1 = esp_adc_cal_raw_to_voltage(vout, &conf);
//		printf("Voltage : %d, raw: %d\n", v1 , vout2);
		printf("Voltage: %d mv, raw: %d\n", v , vout2);
		v = 0;
		v1 = 0;
		vout = 0;
		vTaskDelay(10);

	}
}

