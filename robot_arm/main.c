#include "frdm_bsp.h" 
#include "ADC.h"
#include "led.h" 
#include "lcd1602.h" 
#include "tsi.h"
#include "pit.h"
#include "tpm.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



static uint8_t sliderVal = 50;
static uint8_t sliderOld = 0;
static uint8_t msTicks = 0;
static uint8_t newTick = 0;


float volt_coeff = ((float)(((float)2.91) / 4095) );			// Wspólczynnik korekcji wyniku, w stosunku do napiecia referencyjnego przetwornika
uint8_t wynik_ok=0;
uint16_t temp;
float	wynik;
uint8_t i = 0;

void PIT_IRQHandler()
{
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;		// Skasuj flage zadania przerwania
	NVIC_ClearPendingIRQ(PIT_IRQn);
	
}

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];		// Odczyt danej i skasowanie flagi COCO
	
	//zmiana kanalu ADC, robiona natychmiast po odczytaniu wartosci, aby nowa wartosc nie pojawila sie przed zmiana kanalu
	if(i==0){ //zmiana na x
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(2);
		i++;
	}	
	else if(i==1){ //zmiana na z
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(3);
		i++;
		
	}
	else if(i==2){ //zmiana na g
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(7);
		i++;
		
	}
	else if(i==3){ //zmiana na y
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0);
		i=0;
		
	}
	if(!wynik_ok)					// Sprawdz, czy wynik skonsumowany przez petle glówna
	{
		wynik = temp;				// Wyslij nowa dana do petli glównej
		wynik_ok=1;
	}
	NVIC_ClearPendingIRQ(ADC0_IRQn);
}


int main (void) { 
	
	
	

	
	LED_Init ();	 													/* initialize all LEDs */ 
	LED_Welcome();  												/* blink with all LEDs */




	TPM0_Init_PWM ();			
	

	uint8_t	kal_error;
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	LCD1602_Init();		 // Inicjalizacja wyswietlacza LCD
	LCD1602_Backlight(TRUE);
	LCD1602_Print("---");		// Ekran kontrolny - nie zniknie, jesli dalsza czesc programu nie dziala
	PIT_Init();							// Inicjalizacja licznika PIT0
	
	kal_error=ADC_Init();		// Inicjalizacja i kalibracja przetwornika A/C
	if(kal_error)
	{
		while(1);							// Klaibracja sie nie powiodla
	}
	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0);		// Odblokowanie przerwania i wybranie kanalu nr 0
	
	while(1)
	{
		if(wynik_ok)
		{
			wynik = wynik*volt_coeff;		// Dostosowanie wyniku do zakresu napieciowego
			//sprintf(display,"U=%aV",(int)(wynik*33.793));
			if(i==0){ //os G
				TPM0_SetVal_G((int)(7500-(wynik*259+188)));
				//TPM0_SetVal_G((int)(6750));
				sprintf(display,"G=%iV",(int)(wynik*62-90));
				LCD1602_SetCursor(8,1);
			}
			else if(i==1){ //os X
				TPM0_SetVal_X((int)(7500-(wynik*259+188)));
				//TPM0_SetVal_X((int)(6750));
				sprintf(display,"X=%iV",(int)(wynik*62-90));
				LCD1602_SetCursor(0,0);
			}
			else if(i==2){ //os Y
				TPM0_SetVal_Y((int)(7500-(wynik*259+188)));
				//TPM0_SetVal_Y((int)(6750));
				sprintf(display,"Y=%iV",(int)(wynik*62-90));
				LCD1602_SetCursor(8,0);
			}
			else{  //os Z
				TPM0_SetVal_Z((int)(7500-(wynik*259+188)));
				//TPM0_SetVal_Z((int)(6750));
				sprintf(display,"Z=%iV",(int)(wynik*62-90));
				LCD1602_SetCursor(0,1);
			}
			
			
			
			LCD1602_Print(display);
			
			wynik_ok=0;
		}
	}
	
	
	
	
}
