/*
 * Sterowanie diodami WS2812 - wst�p do programowania z wykorzytsaniem modu��w RMT
 *
 */

#include "time.h"
#include "stdio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
//#include "driver/adc.h"  // biblioteka do adc
//#include "driver/ledc.h" // biblioteka do pwm
//#include "driver/touch_sensor_common.h" // biblioteka do touchpada

#include "driver/rmt.h"
//#include "led_strip.h"

#include "WS2812.h"

//static const char *TAG = "example";

#define RMT_TX_CHANNEL 0	//Numer kana�u RMT na jakim maj� dzia�a� diody

QueueHandle_t kolejka1;

led_strip_t *pasek_LED; // struktura LEDÓW, wskaźnikl do nich
//#define EXAMPLE_CHASE_SPEED_MS (10)

//TaskHandle_t Handle_magicLED;


/**** Pocz�tek obs�ugi di�d RGB *****/

//Obs�uga zdarze� zwi�zanych z diodami programowalnymi
void magicLED0( void * arg )
{
	uint32_t red = 0;
	uint32_t green = 0;
	uint32_t blue = 0;
	uint16_t hue = 0;
	uint16_t start_rgb = 0;
	uint8_t krok;

	//Utworzenie wska�nika do di�d led oraz zainicjowanie wysy�ania danych do di�d LED
	led_strip_t *pasek_LED = ProgrammableLED_init( RMT_TX_CHANNEL, GPIO, WS_LED_CNT);

	//Obliczenie r�nicy pomi�dzy posczeg�lnymi warto�ciami w zale�no�ci od liczby programowanych di�d
	krok = 255/(WS_LED_CNT) ;

	//Prosty przyk�ad kolorowej teczy na wy�wietlaczu
	while (true) {
		for (int j = 0; j < WS_LED_CNT; j++)
		{
			hue = krok * j + start_rgb;	//Obliczenie koloru dla kolejnych di�d w pasku RGB
//			led_strip_hsv2rgb(hue, 255, 1, &red, &green, &blue);	//Przeliczenie koloru na RGB
			hsv_to_rgb(hue, 255, 15, &red, &green, &blue);	//Zamiana palety HSV na RGB
			pasek_LED->set_pixel(pasek_LED, j, red, green, blue);	//Umieszczenie danych w pami�ci dla di�d RGB
//			printf("J: %d, R: %d, G: %d, B: %d \n",j, red, green, blue);
		}
		// Wy�lij dane z pami�ci do di�d RGB
		pasek_LED->refresh(pasek_LED, 0);

			vTaskDelay(5); //50ms

//			pasek_LED->clear(pasek_LED, 50);	//Czyszczenie wszystkich di�d
//			vTaskDelay(1);
//		}
		start_rgb += krok;
////		kolor += 1;
	}
}
/**** KONIEC obs�ugi di�d RGB *****/








// duże M  	   		int pasek1[26] = {4,5,6,8,9,10,14,22,23,24,32,33,34,39,40,41,43,44,45,49,57,58,59,67,68,69};
// małe M 			int pasek3[18] = {4,5,9,10,15,23,24,32,33,39,40,44,45,50,58,59,67,68};
// małe M tło 		int pasek2[52] = {0,1,2,3,6,7,8,11,12,13,14,16,17,18,19,20,21,22,25,26,27,28,29,30,31,34,35,36,37,38,41,
//						42,43,46,47,48,49,51,52,53,54,55,56,57,60,61,62,63,64,65,66,69};


void magicLED_1( void * arg ){
//	pierwsza wersja kodu, aby wybrać konkretną diodę było trzeba liczyć od 0 do 69, czyli dioda z wyśiwetlacza odjąć 1
//	do przerobienia jak w drugim tasku, kiedyś!
	// duże M
	int pasek1[26] = {4,5,6,8,9,10,14,22,23,24,32,33,34,39,40,41,43,44,45,49,57,58,59,67,68,69};
	// tło
	int pasek2[53] = {0,1,2,3,6,7,8,11,12,13,14,15,16,17,18,19,20,21,22,25,26,27,28,29,30,31,34,35,36,37,38,41,
						42,43,46,47,48,49,51,52,53,54,55,56,57,60,61,62,63,64,65,66,69};
	// małe M
	int pasek3[18] = {4,5,9,10,15,23,24,32,33,39,40,44,45,50,58,59,67,68};

	while (true) {

		for (int j = 0; j < 53; j++){pasek_LED->set_pixel(pasek_LED, pasek2[j], 10, 0, 0);}

		for (int j = 0; j < 26; j++){pasek_LED->set_pixel(pasek_LED, pasek1[j], 25, 25, 0);}

		pasek_LED->refresh(pasek_LED, 0);
		vTaskDelay(120);

		for (int j = 0; j < 53; j++){pasek_LED->set_pixel(pasek_LED, pasek2[j], 10, 0, 0);}

		for (int j = 0; j < 18; j++){pasek_LED->set_pixel(pasek_LED, pasek3[j], 25, 25, 0);}

		pasek_LED->refresh(pasek_LED, 0);
		vTaskDelay(120);
	}
}

void magicLED_2( void * arg ){

	int pasek1[] = {1,8,15,22,29,36,43,50,57,64,7,14,21,28,35,42,49,56,63,70,2,3,4,5,6,65,66,67,68,69};
	int ile_pasek1 = sizeof(pasek1)/sizeof(pasek1[0]); // zliczanie elementów tablicy, żeby nie zmieniać pętli for

	int pasek2[] = {9,10,11,12,13,16,23,30,37,44,51,58,20,27,34,41,48,55,62,59,60,61};
	int ile_pasek2 = sizeof(pasek2)/sizeof(pasek2[0]);

	int pasek3[] = {17,18,19,24,31,38,45,52,53,54, };
	int ile_pasek3 = sizeof(pasek3)/sizeof(pasek3[0]);

	int pasek4[] = {25,32,39,46,26,33,40,47};
	int ile_pasek4 = sizeof(pasek4)/sizeof(pasek4[0]);

	int j = 1;

	uint32_t red1 = 0;
	uint32_t green1 = 0;
	uint32_t blue1 = 0;
	uint16_t hue1 = 0;

	uint32_t red2 = 0;
	uint32_t green2 = 0;
	uint32_t blue2 = 0;
	uint16_t hue2 = 0;

	uint32_t red3 = 0;
	uint32_t green3 = 0;
	uint32_t blue3 = 0;
	uint16_t hue3 = 0;

	uint32_t red4 = 0;
	uint32_t green4 = 0;
	uint32_t blue4 = 0;
	uint16_t hue4 = 0;

	while(1){

		hue1 = (hue1 *j)+j;
		hue2 = (2*hue1 *j)+j;
		hue3 = (3*hue1 *j)+j;
		hue4 = (4*hue1 *j)+j;

		for(int i =0; i<ile_pasek1; i++){
			hsv_to_rgb(hue1, 255, 15, &red1, &green1, &blue1);
			pasek_LED->set_pixel(pasek_LED, (pasek1[i]-1), red1, green1, blue1);
		}
		for(int i =0; i<ile_pasek2; i++){
			hsv_to_rgb(hue2, 255, 15, &red2, &green2, &blue2);
			pasek_LED->set_pixel(pasek_LED, (pasek2[i]-1), red2, green2, blue2);
		}
		for(int i =0; i<ile_pasek3; i++){
			hsv_to_rgb(hue3, 255, 15, &red3, &green3, &blue3);
			pasek_LED->set_pixel(pasek_LED, (pasek3[i]-1), red3, green3, blue3);
		}
		for(int i =0; i<ile_pasek4; i++){

			hsv_to_rgb(hue4, 255, 15, &red4, &green4, &blue4);
			pasek_LED->set_pixel(pasek_LED, (pasek4[i]-1), red4, green4, blue4);
		}
		pasek_LED->refresh(pasek_LED, 0);
		j++;
		vTaskDelay(30);

	}

}


void magicLED_3( void * arg ){

	int j = 0;
	int p[] = {71,72,73,74,75,76,77,78,81,85,88,93,94};
	int ip = sizeof(p)/sizeof(p[0]);

	int a[] = {107,108,109,110,111,112,113,116,120,123,128,129,130,131,132,133};
	int ia = sizeof(a)/sizeof(a[0]);

	while(1){

		for(int i =0; i<70; i++){

			pasek_LED->set_pixel(pasek_LED, i, 0, 20,0); // zielone
		}

		if(j>11){
//			pasek_LED->refresh(pasek_LED,0);
			for(int i =0; i<ip; i++){

				pasek_LED->set_pixel(pasek_LED, ((p[i]-1)+200), 0, 15,20); //niebieskie
			}
		}else{
			for(int i =0; i<ip; i++){

				pasek_LED->set_pixel(pasek_LED, ((p[i]-1)-7*j), 0, 15,20); //niebieskie
			}

		}
		if(j>16){
//			pasek_LED->refresh(pasek_LED,0);
			for(int i =0; i<ia; i++){

						pasek_LED->set_pixel(pasek_LED, ((a[i]-1)+200), 0, 15,20); //niebieskie
				}
			}else{
				for(int i =0; i<ia; i++){

					pasek_LED->set_pixel(pasek_LED, ((a[i]-1)-7*j), 0, 15,20); //niebieskie
				}

			}


		pasek_LED->refresh(pasek_LED, 0);
		j++;
		vTaskDelay(50);
	}

}
void magicLED_4( void * arg ){

	int kropka[]= {32,39};
	int ik = sizeof(kropka)/sizeof(kropka[0]);

	int linia1[] = {36,37,38};
	int i1 = sizeof(linia1)/ sizeof(linia1[0]);

	int linia2[] = {43,44,50};
	int i2 = sizeof(linia2)/ sizeof(linia2[0]);

	int linia3[] = {45,51,57};
	int i3 = sizeof(linia3)/ sizeof(linia3[0]);

	int linia4[] = {52,59,58,64,65,66};
	int i4 = sizeof(linia4)/ sizeof(linia4[0]);

	int linia5[] = {46,53,60,67};
	int i5 = sizeof(linia5)/ sizeof(linia5[0]);

	int linia6[] = {54,61,68,62,69,70};
	int i6 = sizeof(linia6)/ sizeof(linia6[0]);

	int linia7[] = {47,55,63};
	int i7 = sizeof(linia7)/ sizeof(linia7[0]);

	int linia8[] = {48,49,56};
	int i8 = sizeof(linia8)/ sizeof(linia8[0]);

	int linia9[] = {40,41,42};
	int i9 = sizeof(linia9)/ sizeof(linia9[0]);

	int linia10[] = {33,34,35};
	int i10 = sizeof(linia10)/ sizeof(linia10[0]);

	int linia11[] = {21,27,28};
	int i11 = sizeof(linia11)/ sizeof(linia11[0]);

	int linia12[] = {14,20,26};
	int i12 = sizeof(linia12)/ sizeof(linia12[0]);

	int linia13[] = {5,6,7,12,13,19};
	int i13 = sizeof(linia13)/ sizeof(linia13[0]);

	int linia14[] = {4,11,18,25};
	int i14 = sizeof(linia14)/ sizeof(linia14[0]);

	int linia15[] = {1,2,3,9,10,17};
	int i15 = sizeof(linia15)/ sizeof(linia15[0]);

	int linia16[] = {8,16,24};
	int i16 = sizeof(linia16)/ sizeof(linia16[0]);

	int linia17[] = {15,22,23};
	int i17 = sizeof(linia17)/ sizeof(linia17[0]);

	int linia18[] = {29,30,31};
	int i18 = sizeof(linia18)/ sizeof(linia18[0]);


	int hue = 0;
	int start_rgb = 0;
	int j = 0; //
	int krok = 255/18;
//	int krok = 255/9;
	int k = 1;
	while(1){
		j++;
		for(int i=0; i<ik; i++){

			pasek_LED->set_pixel(pasek_LED, (kropka[i]-1), 15,15,15);

		}

		for(int i=0; i<i1; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia1[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i2; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia2[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i3; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia3[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i4; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia4[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i5; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia5[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i6; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia6[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i7; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia7[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i8; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia8[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i9; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia9[i]-1), red,green,blue);

		}
		k++;
		for(int i=0; i<i10; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia10[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i11; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia11[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i12; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia12[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i13; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia13[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i14; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia14[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i15; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia15[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i16; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia16[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i17; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia17[i]-1), red,green,blue);

		}
		k++;

		for(int i=0; i<i18; i++){
			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			hue = krok * k + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, (linia18[i]-1), red,green,blue);

		}
		k++;
// 26 i 40 dla pełnego obrotu prawie :)
// 26 i 33 dla cofających sie o pełen obrót i w prawo o pół obrotu
//		zakomentować ify i odkomentować jedną z dwóch komend przed delayem aby w nieskończoność się kręciło
		if (j<26){start_rgb += krok;}
		else if(j>26 && j<40){start_rgb -= krok;}
		else if(j>40){j=0;}

		pasek_LED->refresh(pasek_LED,0);
//		start_rgb -= krok; // obroty prawe
//		start_rgb += krok; // obroty lewe
		vTaskDelay(10);

	}

}

void magicLED_5(void *arg){

	int linia[] = {1,8,15,22,29,36,43,50,57,64,65,66,67,68,69,70,63,56,49,42,35,28,21,14,7,6,5,4,3,2,
				   9,16,23,30,37,44,51,58,59,60,61,62,55,48,41,34,27,20,13,12,11,10,
				   17,24,31,38,45,52,53,54,47,40,33,26,19,18,
				   25,32,39,46};
	int il = sizeof(linia)/ sizeof(linia[0]);

	int hue = 0;
	int krok = 255/il;
	int start_rgb=0;

	int k = 0;


	while(1){

		for(int i = 0; i<il; i++){
			uint32_t red = 0;
			uint32_t green = 0;
			uint32_t blue = 0;
			hue = krok *i + start_rgb;
			hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED, linia[i]-1, red, green, blue);

		}
		k++;


		if (k < 72){
			start_rgb -= krok; // leci w prawo
		}else if (k>72 && k<144){
			start_rgb += krok; //leci w lewo
		}else if (k>144){ k = 0;}
		printf("%d\n", k);
//		start_rgb += krok; //leci w lewo
//		start_rgb -= krok; // leci w prawo

		pasek_LED->refresh(pasek_LED,0);
		vTaskDelay(4);


	}
}

void magicLED_6(void *arg){

	int a=0;

	while(1){

		srand(time(NULL));
		a =rand()%70;
//		printf("%d\n", a);
//		vTaskDelay(5);
		pasek_LED->set_pixel(pasek_LED,a,5,0,10);
		pasek_LED->refresh(pasek_LED,0);
		vTaskDelay(5);
//		pasek_LED->clear(pasek_LED,0);
	}


}

void magicLED_7(void *arg){

	int kropka[] = {4};
	int ik = sizeof(kropka)/sizeof(kropka[0]);

	int linia2[] = {3,5,11};
	int i2 = sizeof(linia2)/sizeof(linia2[0]);

	int linia3[] = {2,10,18,12,6};
	int i3 = sizeof(linia3)/sizeof(linia3[0]);

	int linia4[] = {1,9,17,25,19,13,7};
	int i4 = sizeof(linia4)/sizeof(linia4[0]);

	int linia5[] = {8,16,24,32,26,20,14};
	int i5 = sizeof(linia5)/sizeof(linia5[0]);

	int linia6[] = {15,23,31,39,33,27,21};
	int i6 = sizeof(linia6)/sizeof(linia6[0]);

	int linia7[] = {22,30,38,46,40,34,28};
	int i7 = sizeof(linia7)/sizeof(linia7[0]);

	int linia8[] = {29,37,45,53,47,41,35};
	int i8 = sizeof(linia8)/sizeof(linia8[0]);

	int linia9[] = {36,44,52,60,54,48,42};
	int i9 = sizeof(linia9)/sizeof(linia9[0]);

	int linia10[] = {43,51,59,67,61,55,49};
	int i10 = sizeof(linia10)/sizeof(linia10[0]);

	int linia11[] = {50,58,66,68,62,56};
	int i11 = sizeof(linia11)/sizeof(linia11[0]);

	int linia12[] = {57,65,63,69};
	int i12 = sizeof(linia12)/sizeof(linia12[0]);

	int linia13[] = {64,70};
	int i13 = sizeof(linia13)/sizeof(linia13[0]);


	int hue=0;
	int krok = 255/13;
	int start_rgb=0;
	int k =0;
	while(1){

		for(int i = 0; i<ik; i++){
					uint32_t red = 0;
					uint32_t green = 0;
					uint32_t blue = 0;
					hue = krok *k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, kropka[i]-1, red, green, blue);

		}
		k++;
		for(int i=0; i<i2; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia2[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i3; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia3[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i4; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia4[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i5; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia5[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i6; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia6[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i7; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia7[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i8; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia8[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i9; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia9[i]-1), red,green,blue);

				}
				k++;
				for(int i=0; i<i10; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia10[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i11; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia11[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i12; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia12[i]-1), red,green,blue);

				}
				k++;

				for(int i=0; i<i13; i++){
					uint32_t red = 0;
					uint32_t  green = 0;
					uint32_t  blue = 0;
					hue = krok * k + start_rgb;
					hsv_to_rgb(hue, 255, 12, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, (linia13[i]-1), red,green,blue);

				}
				k++;

				start_rgb-=krok;
				pasek_LED->refresh(pasek_LED,0);
				vTaskDelay(11);
//				opóznienie 5 dla "obrotów lewych"
//				opóznienie 11 dla "obrotów prawych"

	}

}

void magicLED_8(void *arg){
	int j=0;
	while(1){

		for(int i=0; i<50; i++){
			pasek_LED->set_pixel(pasek_LED,i,0,0,0);  //0,10,15 ładny niebieski
		}

			uint32_t red = 0;
			uint32_t  green = 0;
			uint32_t  blue = 0;
			uint8_t hue = 255/10*j;
			uint8_t sat = 255/15*j;
			hsv_to_rgb(hue, 255, sat, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED,j,red,green,blue);
			pasek_LED->set_pixel(pasek_LED,100-j,red,green,blue);

		j++;
		if (j>100){j=0;}
		pasek_LED->refresh(pasek_LED,0);
		vTaskDelay(16);
	}

}

void magicLED_9(void *arg){
	uint8_t hue = 0;
	while(1){
		for(int i=0;i<70;i++){
			pasek_LED->set_pixel(pasek_LED,i,0,0,0);
		}

			uint32_t red = 0;
			uint32_t green = 0;
			uint32_t blue = 0;
			srand(time(NULL));
			uint8_t ld = rand()%70;
			hsv_to_rgb(hue, 255, 200, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED,ld,red,green,blue);
			hue++;

		pasek_LED->refresh(pasek_LED,0);
		vTaskDelay(1);

	}
}

void magicLED_10(void *arg){
	uint8_t hue = 0;
	uint32_t red = 0;
	uint32_t green = 0;
	uint32_t blue = 0;

	while(1){

		for(int i=0; i<50; i++){

			hsv_to_rgb(hue, 255, 10, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED,i,red,green,blue);

		}
		for(int j = 0; j<6; j++){
		srand(time(NULL));
		uint8_t j = rand()%50;
		uint8_t sat = 10;
		for(int i=0; i<245; i++){
			hsv_to_rgb(hue, 255, sat, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED,j,red,green,blue);
			sat++;
			pasek_LED->refresh(pasek_LED,0);
			vTaskDelay(1);
		}
		}
		hue +=35;
		vTaskDelay(10);

	}
}

void magicLED_11(void *arg){
	uint8_t hue = 0;
	uint32_t red = 0;
	uint32_t green = 0;
	uint32_t blue = 0;

	while(1){


		for(int j = 0; j<5; j++){ // zmieniając j w tej pętli zmieniamy liczbę ledów które swięcą sie po sobie w danym ciągu
		srand(time(NULL));
		uint8_t ld = rand()%70;
		uint8_t sat = 10;
		for(int i=0; i<70; i++){

			hsv_to_rgb(hue, 255, 10, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED,i,red,green,blue);

		}
		for(int i=0; i<245; i++){
			hsv_to_rgb(hue, 255, sat, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED,ld,red,green,blue);
			sat++;
			pasek_LED->refresh(pasek_LED,0);
			vTaskDelay(1);
		}
		}
		hue +=35;
		vTaskDelay(10);

	}
}

void magicLED_12(void *arg){
	int j = 0;
	uint8_t hue = 30;
	uint32_t red = 0;
	uint32_t green = 0;
	uint32_t blue = 0;

	while(1){

		for(int z = 0; z<50;z++)pasek_LED->set_pixel(pasek_LED,z,15,0,0);

		if(j==120) j=0;

		if(j>=1){

			for(int z = 0; z<50;z++)pasek_LED->set_pixel(pasek_LED,z,15,0,0);

			for(int i = 0; i<10; i++){

				int j1 = 110-i-j;

				if(j1>=0){

					hsv_to_rgb(hue, 255, (i*25+15), &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED,(i+j),red,green,blue);
					pasek_LED->set_pixel(pasek_LED,j1,red,green,blue);
					hue=hue+30;

				}else if(j1<0){

					int k = 110-j;
					pasek_LED->set_pixel(pasek_LED,k,0,0,0);

				}

			}

		}

		if(j==0){

			for(int i = 0; i<9;i++){

				hsv_to_rgb(hue, 255, (i*25+15), &red, &green, &blue);
				pasek_LED->set_pixel(pasek_LED,i,red,green,blue);
				hue=hue+30;
				pasek_LED->refresh(pasek_LED,0);
				vTaskDelay(6);

			}
			hsv_to_rgb(hue, 255, 9, &red, &green, &blue);
			pasek_LED->set_pixel(pasek_LED,10,red,green,blue);
			hue=hue+30;

		}

		j++;
		pasek_LED->refresh(pasek_LED,0);
		vTaskDelay(6);

	}

}

void magicLED_13(void *arg){

	int tab[10];
	int itab = sizeof(tab)/ sizeof(tab[0]);

	uint32_t redw = 0;
	uint32_t greenw = 0;
	uint32_t bluew = 0;

	hsv_to_rgb(0, 0, 255, &redw, &greenw, &bluew);// biały

	while(1){

		srand(time(NULL));

		for(int z = 0; z<50;z++)pasek_LED->set_pixel(pasek_LED,z,0,0,20);

		for(int i =0; i<itab; i++){

			tab[i] = rand()%50;
			pasek_LED->set_pixel(pasek_LED, tab[i], redw, greenw, bluew);
		}

		pasek_LED->refresh(pasek_LED,0);
		vTaskDelay(1);

	}

}

void magicLED_14(void *arg){

		int k =0;
		uint32_t red = 0;
		uint32_t green = 0;
		uint32_t blue = 0;
		uint16_t hue = 0;
		uint16_t start_rgb = 0;
		uint8_t krok;
		krok = 255/25 ;

		while (true) {

			for(int z = 0; z<50;z++)pasek_LED->set_pixel(pasek_LED,z,0,0,0);

			if(k<100){
				for (int j = 0; j < 25; j++){
					hue = krok * j + start_rgb;
					hsv_to_rgb(hue, 255, 50, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, 2*j, red, green, blue);

				}
				start_rgb += krok;
			}

			if(k>100 && k<200){
				for (int j = 0; j < 25; j++){
					hue = krok * j + start_rgb;
					hsv_to_rgb(hue, 255, 50, &red, &green, &blue);
					pasek_LED->set_pixel(pasek_LED, 2*j+1, red, green, blue);

				}
				start_rgb -= krok;
			}

			if(k==200) k=0;
			k++;
			pasek_LED->refresh(pasek_LED, 0);
			vTaskDelay(5);

		}

}

void magicLED(void * arg){
//	magicLED0(&arg);
//	magicLED_1(&arg); // McDonald #mastryca
//	magicLED_2(&arg); // Abstrakcja #matrycas
//	magicLED_3(&arg); // Pływające imie(niedkończone) #matryca
//	magicLED_4(&arg); // okrągła tarcza # matryca
//	magicLED_5(&arg); // ślimaczek lewo prawo #matryca
//	magicLED_6(&arg); // wszystkie diody po kolei sie zapalają #bombki
//	magicLED_7(&arg); // strzłka w prawo biegnie w prawo #matryca
//	magicLED_8(&arg); // kropka lewo prawo, zmiana kloru, zmiana jasności #bombki
//	magicLED_9(&arg); // randomowa kropka zmienia kolory #bombki
//	magicLED_10(&arg); // matryca zmienia kolry i 10 ledów(rand)po jednej zwiększa swoją jasność, potem zmienuia kolor #bombki
//	magicLED_11(&arg); // to samo co wyżej tylko ze tylko jeden led swieci sie w jednym momencie i zmienia sie led i tak 5 #bombki
//	magicLED_12(&arg); // ala sinoleon z wleda
//	magicLED_13(&arg); // "wirujące koła"
	magicLED_14(&arg); // efekt ze co druga dioda świeci w prawo, potem sie zamieniajaą diody i w lewo

}

void LED(void *arg){

	int licznik = 0;
	//################
	int j = 0;
	uint16_t hue = 30;
	uint32_t red = 0;
	uint32_t green = 0;
	uint32_t blue = 0;

	int tab[10];
	int itab = sizeof(tab)/ sizeof(tab[0]);
	uint32_t redw = 0;
	uint32_t greenw = 0;
	uint32_t bluew = 0;
	hsv_to_rgb(0, 0, 255, &redw, &greenw, &bluew);// biały

	int k =0;
	uint16_t start_rgb = 0;
	uint8_t krok;
	krok = 255/50 ;

	while(1){

		xQueueReceive(kolejka1, &licznik, 0);
			printf("licznik:%d\n",licznik);

		//#########################################################
		//Efekt 1

		if(licznik<=20){
			printf("efekt1\n");

			for(int z = 0; z<50;z++)pasek_LED->set_pixel(pasek_LED,z,15,0,0);

					if(j==120) j=0;

					if(j>=1){

						for(int z = 0; z<50;z++)pasek_LED->set_pixel(pasek_LED,z,15,0,0);

						for(int i = 0; i<10; i++){

							int j1 = 110-i-j;

							if(j1>=0){

								hsv_to_rgb(hue, 255, (i*25+15), &red, &green, &blue);
								pasek_LED->set_pixel(pasek_LED,(i+j),red,green,blue);
								pasek_LED->set_pixel(pasek_LED,j1,red,green,blue);
								hue=hue+30;

							}else if(j1<0){

								int k = 110-j;
								pasek_LED->set_pixel(pasek_LED,k,0,0,0);

							}

						}

					}

					if(j==0){

						for(int i = 0; i<9;i++){

							hsv_to_rgb(hue, 255, (i*25+15), &red, &green, &blue);
							pasek_LED->set_pixel(pasek_LED,i,red,green,blue);
							hue=hue+30;
							pasek_LED->refresh(pasek_LED,0);
							vTaskDelay(6);

						}
						hsv_to_rgb(hue, 255, 9, &red, &green, &blue);
						pasek_LED->set_pixel(pasek_LED,10,red,green,blue);
						hue=hue+30;

					}

					j++;
					pasek_LED->refresh(pasek_LED,0);
					vTaskDelay(6);
		}

		//#########################################################
		//Efekt 2

		if(licznik>20 && licznik <=40){
			printf("efekt2\n");

			srand(time(NULL));

					for(int z = 0; z<50;z++)pasek_LED->set_pixel(pasek_LED,z,0,0,20);

					for(int i =0; i<itab; i++){

						tab[i] = rand()%50;
						pasek_LED->set_pixel(pasek_LED, tab[i], redw, greenw, bluew);
					}

					pasek_LED->refresh(pasek_LED,0);
					vTaskDelay(1);



		}

		//#########################################################
		//Efekt 3

		if(licznik>40){
			printf("efekt3\n");

			for(int z = 0; z<50;z++) pasek_LED->set_pixel(pasek_LED,z,0,0,0);

						if(k<100){
							for (int j = 0; j < 25; j++){
								hue = krok * j + start_rgb;
								hsv_to_rgb(hue, 255, 50, &red, &green, &blue);
								pasek_LED->set_pixel(pasek_LED, 2*j, red, green, blue);

							}
							start_rgb += krok;
						}

						if(k>100 && k<200){
							for (int j = 0; j < 25; j++){
								hue = krok * j + start_rgb;
								hsv_to_rgb(hue, 255, 50, &red, &green, &blue);
								pasek_LED->set_pixel(pasek_LED, 2*j+1, red, green, blue);

							}
							start_rgb -= krok;
						}

						if(k==200) k=0;
						k++;
						pasek_LED->refresh(pasek_LED, 0);
						vTaskDelay(5);


		}
//		vTaskDelay(1);

	}
}

void app_main(void){

	int licznik = 1;
	kolejka1 = xQueueCreate(10, sizeof(int));
	pasek_LED = ProgrammableLED_init( RMT_TX_CHANNEL, GPIO, WS_LED_CNT);
	xTaskCreatePinnedToCore( magicLED, "", 4096, NULL, 1, NULL, 1); // inicjacja taska do ledów
//	xTaskCreatePinnedToCore(LED, "", 4096, NULL, 1, NULL,1); // przechodzenie efektów

//	vTaskDelete(NULL);	//usuwanie taska głownego

	while(1){
		if(licznik == 61) licznik = 1;
		xQueueSend(kolejka1, &licznik,0);
		licznik++;
		vTaskDelay(100);
	}
}
