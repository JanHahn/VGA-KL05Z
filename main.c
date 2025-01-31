#include "MKL05Z4.h" 
#include "TPM.h"
#include "gpio.h"
#include "klaw.h"
#include <stdlib.h>
#include "frdm_bsp.h"

//amount of pixel per vertial sections
#define VER_VISABLE_AREA 14 //200
#define VER_FORNT_PORCH 0 //10
#define VER_SYNC_PULSE 2//32
#define VER_BACK_PORSH 1 //22
#define WHOLE_LINE (VER_VISABLE_AREA + VER_FORNT_PORCH + VER_SYNC_PULSE + VER_BACK_PORSH)

//amount of lines per horizontal sections
#define HOR_VISABLE_AREA 480
#define HOR_FORNT_PORCH 10
#define HOR_SYNC_PULSE 2
#define HOR_BACK_PORSH 33
#define WHOLE_FRAME 525

#define H_SYNC_PIN 10
#define V_SYNC_PIN 11

#define RED_PIN 1
#define BLUE_PIN 2
#define GREEN_PIN 5

//amount of pixel to move on one button press
#define xMovement 2
#define yMovement 2

volatile uint16_t actualPixel = 0;
volatile uint16_t actualRow = 0;

volatile uint8_t rightPressed=0;
volatile uint8_t downPressed=0;

uint16_t cursorX = 1;
uint16_t cursorY = 1;


void TPM1_IRQHandler(){
	actualPixel += 1;
	TPM1->STATUS |= TPM_STATUS_TOF_MASK;
}

void PORTB_IRQHandler(void)
{
	uint32_t buf;
	buf=PORTB->ISFR & (S1_MASK | S2_MASK);
	
	switch(buf)
	{
		case S1_MASK:	DELAY(100)
									if(!(PTB->PDIR&S1_MASK))		// Minimalizacja drgan zestyk?w
									{
										DELAY(100)
										if(!(PTB->PDIR&S1_MASK))	// Minimalizacja drgan zestyk?w (c.d.)
										{
											if(!rightPressed)
											{
												rightPressed=1;
											}
										}
									}
									break;
		case S2_MASK:	DELAY(100)
									if(!(PTB->PDIR&S2_MASK))		// Minimalizacja drgan zestyk?w
									{
										DELAY(100)
										if(!(PTB->PDIR&S2_MASK))	// Minimalizacja drgan zestyk?w (c.d.)
										{
											if(!downPressed)
											{
												downPressed=1;
											}
										}
									}
									break;
		default:			break;
	}	
	PORTB->ISFR |= S1_MASK | S2_MASK;	// Kasowanie wszystkich bit?w ISF
	NVIC_ClearPendingIRQ(PORTB_IRQn);
}

	
int main(void){ 
		
	/*SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;  // Wlacz zegar dla PORTB
	
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;  // Wlacz zegar dla DAC
	PORTB->PCR[1] = PORT_PCR_MUX(0);
	DAC0->C0 |= DAC_C0_DACEN_MASK;  // Wlacz DAC
	DAC0->C0 |= DAC_C0_DACRFS_MASK; // Wybierz wewnetrzne zródlo referencyjne (np. 3.3V) */
	
	gpio_set_output(PORTB, GPIOB, V_SYNC_PIN);
	gpio_set_output(PORTB, GPIOB, H_SYNC_PIN);
	gpio_set_output(PORTB, GPIOB, RED_PIN);
	gpio_set_output(PORTB, GPIOB, BLUE_PIN);
	gpio_set_output(PORTB, GPIOB, GREEN_PIN);
	
	gpio_set_high(GPIOB, V_SYNC_PIN);
	gpio_set_high(GPIOB, H_SYNC_PIN);
	gpio_set_low(GPIOB, RED_PIN);
	gpio_set_low(GPIOB, BLUE_PIN);
	gpio_set_low(GPIOB, GREEN_PIN);
	Tpm_Init();
	
	
	while(1) { 
		
		
		//button pressed actualization 
		if (rightPressed == 1){
			cursorX += xMovement; //przesun kursor w prawo
			rightPressed = 0;
		}
		if (downPressed == 1){
			cursorY += yMovement;	//przesun kursor w dol
			downPressed = 0;
		}
		
		
		//synchronization signals actualization 
		if (actualPixel >= WHOLE_LINE){
			actualPixel = 0;
			actualRow += 1;
		}
		else if (actualPixel >= VER_VISABLE_AREA + VER_FORNT_PORCH + VER_SYNC_PULSE){
			gpio_set_high(GPIOB, H_SYNC_PIN);
		}
		else if (actualPixel >= VER_VISABLE_AREA + VER_FORNT_PORCH && actualPixel < VER_VISABLE_AREA + VER_FORNT_PORCH + VER_SYNC_PULSE){
			gpio_set_low(GPIOB, H_SYNC_PIN);
		}
		
		
		if (actualRow >= WHOLE_FRAME){
			actualRow = 0;
		}
		else if (actualRow >= HOR_VISABLE_AREA + HOR_FORNT_PORCH + HOR_SYNC_PULSE){
			gpio_set_high(GPIOB, V_SYNC_PIN);
		}
		else if (actualRow >= HOR_VISABLE_AREA + HOR_FORNT_PORCH){
			gpio_set_low(GPIOB, V_SYNC_PIN);
		}
		
		
		
		//print cursor
		if (actualPixel < VER_VISABLE_AREA && actualRow < HOR_VISABLE_AREA){
			if (actualPixel > cursorX && actualPixel < cursorX + 5 && actualRow > cursorY && actualRow <= cursorY + 5){
				//set curosr color
				//gpio_set_high(GPIOB, RED_PIN);
			}
		}
		
		
	}
}
