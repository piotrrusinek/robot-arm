#include "tpm.h"


void TPM1_IRQHandler(void);

static uint16_t tpm1Diff = 0;
static uint16_t tpm1New = 0;
static uint16_t tpm1Old = 0;
static uint8_t tpm0Enabled = 0;

void TPM1_Init_InputCapture(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;		
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; 
	PORTA->PCR[0] = PORT_PCR_MUX(2);  
	
	TPM1->SC |= TPM_SC_PS(7);  				
	TPM1->SC |= TPM_SC_CMOD(1);				
	

	TPM1->SC &= ~TPM_SC_CPWMS_MASK; 		/* up counting */
	TPM1->CONTROLS[0].CnSC &= ~ (TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK); //CONTROLS -> channel
	TPM1->CONTROLS[0].CnSC |= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK); /* capture on both edges */ 
  
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_CHIE_MASK; 
	
	NVIC_SetPriority(TPM1_IRQn, 1);  /* TPM1 interrupt priority level  */

	NVIC_ClearPendingIRQ(TPM1_IRQn); 
	NVIC_EnableIRQ(TPM1_IRQn);	/* Enable Interrupts */

}

void TPM0_Init_PWM(void) { 
		
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  
	

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 
	
	PORTB->PCR[8] = PORT_PCR_MUX(2);		//ch3
	PORTB->PCR[9] = PORT_PCR_MUX(2);    //ch2
	PORTB->PCR[10] = PORT_PCR_MUX(2);   //ch1
	PORTB->PCR[11] = PORT_PCR_MUX(2);   //ch0
	TPM0->SC |= TPM_SC_PS(7);  					
	TPM0->SC |= TPM_SC_CMOD(1);					

	TPM0->MOD = 7500; 										
	

	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		/* up counting */
	TPM0->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK); /* set Output on match, clear Output on reload */ 
	TPM0->CONTROLS[2].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
	TPM0->CONTROLS[3].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
	TPM0->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);

	TPM0->CONTROLS[1].CnV = 750; 				
	TPM0->CONTROLS[2].CnV = 750;
	TPM0->CONTROLS[3].CnV = 750;
	TPM0->CONTROLS[0].CnV = 750;
	tpm0Enabled = 1;  /* set local flag */
}



uint32_t TPM1_GetVal(void) {
	return tpm1Diff;
}

void TPM0_SetVal_X(uint32_t value) {
	value = value - 1; 				
	//if (tpm0Enabled) TPM0->CONTROLS[2].CnV = value;    
	if (tpm0Enabled){
		TPM0->CONTROLS[1].CnV = value;
		
	}
}
void TPM0_SetVal_Y(uint32_t value) {
	value = value - 1; 		
	//if (tpm0Enabled) TPM0->CONTROLS[2].CnV = value;    
	if (tpm0Enabled){
		
		TPM0->CONTROLS[2].CnV = value;
		
	}
}
void TPM0_SetVal_Z(uint32_t value) {
	value = value - 1; 			
	//if (tpm0Enabled) TPM0->CONTROLS[2].CnV = value;    
	if (tpm0Enabled){
		
		TPM0->CONTROLS[3].CnV = value;
	}
}
void TPM0_SetVal_G(uint32_t value) {
	value = value - 1; 				
	//if (tpm0Enabled) TPM0->CONTROLS[2].CnV = value;    
	if (tpm0Enabled){
		
		TPM0->CONTROLS[0].CnV = value;
	}
}
/**
 * @brief Interrupt handler for TPM1.
 */
void TPM1_IRQHandler(void) {
	
	tpm1Old = tpm1New;
	tpm1New = TPM1->CONTROLS[0].CnV & 0xFFFF;  
	tpm1Diff = tpm1New - tpm1Old;	/* calculate difference */
	
	LED_Ctrl(LED_RED, LED_TOGGLE); 
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK; 
}
