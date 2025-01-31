#include "TPM.h"

void Tpm_Init()
{
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;	// Dołączenie sygnału zegara do modułów TPM1
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);	// Wybierz źródło taktowania TPMx MCGFLLCLK=41943040Hz

	//TPM1->SC &= (~TPM_SC_CPWMS_MASK);	//	TPM1 w trybie "up counting"
	//TPM1->SC &= (~TPM_SC_PS_MASK);	//	Dzielnik zegara wejściowegoo równy 1; zegar = 41943040Hz
	TPM1->SC |= TPM_SC_PS(0);
	
	TPM1->MOD = TPM_MOD_MOD(79); // (41943040 / 10^6) - 1 = 41   WE SET OUR PIXEL RATE TO 10MHZ
	TPM1->SC |= TPM_SC_TOIE_MASK; // Odblokowanie przerwania od OVERFLOW dla TPM1
	
	NVIC_EnableIRQ(TPM1_IRQn); 
	TPM1->SC |= TPM_SC_CMOD(1);
	//TPM0->CONF = 0;
	
}

//void InCap_OutComp_Init()
//{
//	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;			// Dołączenie sygnału zegara do portu B
//	// Wybór odpowiedniej roli pinu
//	PORTB->PCR[8] |= PORT_PCR_MUX(2);		// LED R 	PWM
//	PORTB->PCR[9] |= PORT_PCR_MUX(2);		// LED G	PWM
//	PORTB->PCR[10] |= PORT_PCR_MUX(2);	// LED B	PWM
//	PORTB->PCR[0] |= PORT_PCR_MUX(2);		// PTB0 - EXTRG_IN
//	PORTB->PCR[13] |= PORT_PCR_MUX(2);	// PTB13 - TPM1_CH1 (Input Capture)
//	PORTB->PCR[11] |= PORT_PCR_MUX(1);	// PTB11 - GPIO
//	PORTB->PCR[11] &= (~PORT_PCR_SRE_MASK);
//	PTB->PDDR |= (1<<11);									// PTB11 - wyjście
//	PTB->PCOR = (1<<11);
//	
//	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;				// Dołączenie sygnału zegara do modułów TPM1
//	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);		// Wybierz źródło taktowania TPMx MCGFLLCLK=41943040Hz
//	
//	TPM1->SC &= (~TPM_SC_CPWMS_MASK);				//	TPM1 w trybie "up counting"
//	TPM1->SC &= (~TPM_SC_PS_MASK);					//	Dzielnik zegara wejściowegoo równy 1; zegar = 41943040Hz
//	TPM1->CONF |= (TPM_CONF_CSOT_MASK | TPM_CONF_CROT_MASK | TPM_CONF_CSOO_MASK);	// TPM1: wyzwalanie sprzętowe, reload po wyzwoleniu, stop po OVERFLOW
//	TPM1->CONF &= (~TPM_CONF_TRGSEL_MASK);				// Wyzwalanie zewnętrznym sygnałem EXTRG_IN
//	TPM1->CONTROLS[1].CnSC = (TPM_CnSC_ELSB_MASK | TPM_CnSC_CHIE_MASK);// Input Capture w kanale nr 1 na opadającym zboczu, aktywne przerwanie dla tego trybu
//	
//	TPM1->SC |= TPM_SC_TOIE_MASK;					// Odblokowanie przerwania od OVERFLOW dla TPM1
//	NVIC_ClearPendingIRQ(TPM1_IRQn);
//  NVIC_EnableIRQ(TPM1_IRQn);
//	
//	TPM1->SC |= TPM_SC_CMOD(1);		// Włączenie TPM1
//}



	