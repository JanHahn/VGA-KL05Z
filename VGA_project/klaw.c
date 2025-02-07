#include "klaw.h"

void Klaw_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //aktywacja pottu B
	
	PORTB->PCR[S1] |= PORT_PCR_MUX(1); 
	PORTB->PCR[S2] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[S1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTB->PCR[S2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	
}
void Klaw_IQR_Int(void)
{
	PORTB -> PCR[S1] |= PORT_PCR_IRQC(0xb);	 //0xb - for both edges
	PORTB -> PCR[S2] |= PORT_PCR_IRQC(0xb);	
	
	NVIC_SetPriority(PORTB_IRQn, 5); 
	NVIC_ClearPendingIRQ(PORTB_IRQn);
	NVIC_EnableIRQ(PORTB_IRQn);
	
}
