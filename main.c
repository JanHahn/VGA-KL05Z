#include "MKL05Z4.h" 
#include "TPM.h"
#include "gpio.h"
#include <stdlib.h>

//amount of pixel per vertial sections
#define VER_VISABLE_AREA 200
#define VER_FORNT_PORCH 10
#define VER_SYNC_PULSE 32
#define VER_BACK_PORSH 22
#define WHOLE_LINE (VER_VISABLE_AREA + VER_FORNT_PORCH + VER_SYNC_PULSE + VER_BACK_PORSH)

//amount of pixel per horizontal sections
#define HOR_VISABLE_AREA 600
#define HOR_FORNT_PORCH 1
#define HOR_SYNC_PULSE 4
#define HOR_BACK_PORSH 23
#define WHOLE_FRAME 628

#define V_SYNC_PIN 10
#define H_SYNC_PIN 11

volatile uint16_t actualPixel = 0;
volatile uint16_t actualRow = 0;

void TPM1_IRQHandler(){
	actualPixel += 1;
	TPM1->STATUS |= TPM_STATUS_TOF_MASK;
}
	

int main(void){ 
	
	gpio_set_output(PORTB, GPIOB, V_SYNC_PIN);
	gpio_set_output(PORTB, GPIOB, H_SYNC_PIN);
	gpio_set_high(GPIOB, V_SYNC_PIN);
	gpio_set_high(GPIOB, H_SYNC_PIN);
	Tpm_Init();
	
	
	while(1) { 
		
		if (actualPixel >= VER_VISABLE_AREA + VER_FORNT_PORCH){
			gpio_set_low(GPIOB, V_SYNC_PIN);
		}
		if (actualPixel >= VER_VISABLE_AREA + VER_FORNT_PORCH + VER_SYNC_PULSE){
			gpio_set_high(GPIOB, V_SYNC_PIN);
		}
		if (actualPixel >= WHOLE_LINE){
			actualPixel = 0;
			actualRow += 1;
		}
		
		if (actualRow >= HOR_VISABLE_AREA + HOR_FORNT_PORCH){
			gpio_set_low(GPIOB, H_SYNC_PIN);
		}
		if (actualRow >= HOR_VISABLE_AREA + HOR_FORNT_PORCH + HOR_SYNC_PULSE){
			gpio_set_high(GPIOB, H_SYNC_PIN);
		}
		if (actualRow >= WHOLE_FRAME){
			actualRow = 0;
		}
		
	}
}
