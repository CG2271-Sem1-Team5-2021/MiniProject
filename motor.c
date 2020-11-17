#include "MKL25Z4.h"                    
#include "motor.h"

void initMotor(void) {

	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	PORTD->PCR[FORWARD_RIGHT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[FORWARD_RIGHT] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[FORWARD_LEFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[FORWARD_LEFT] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[BACKWARD_RIGHT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BACKWARD_RIGHT] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[BACKWARD_LEFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BACKWARD_LEFT] |= PORT_PCR_MUX(4);
	
	//Enable Clock Gating for Timer0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //MCGFLLCLK or MCGPLLCLK/2

	TPM0->MOD = MOD_VALUE;
		
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK); //upcounting
	
	//Center-aligned PWM MSB:MSA = 10 and ELSB:ELSA = 10
	TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));

	TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));	
}

int calculateCnV(int duty_cycle){
	float mod_value = (float)(duty_cycle/100)* MOD_VALUE;
	return (int) mod_value;
}

void move(int dir) {
	switch(dir){
		case (FORWARD):
			TPM0_C0V = calculateCnV(FULL_SPEED);				// RIGHT FORWARD
			TPM0_C3V = calculateCnV(FULL_SPEED);				// LEFT FORWARD
			break;
		case (BACKWARD):
			TPM0_C5V = calculateCnV(FULL_SPEED);				// LEFT BACKWARD
			TPM0_C2V = calculateCnV(FULL_SPEED);				// RIGHT BACKWARD
			break;
		case (LEFT):
			TPM0_C2V = calculateCnV(FULL_SPEED);				// RIGHT BACKWARD	
			TPM0_C3V = calculateCnV(FULL_SPEED);				// LEFT FORWARD
			break;
		case (RIGHT):
			TPM0_C0V = calculateCnV(FULL_SPEED);				// RIGHT FORWARD
			TPM0_C5V = calculateCnV(FULL_SPEED);				// LEFT	REVERSE
			break;
		case (CURVE_LEFT_FORWARD):
			TPM0_C0V = calculateCnV(CURVE_SPEED);				// RIGHT BACKWARD
			TPM0_C3V = calculateCnV(FULL_SPEED);				// LEFT FORWARD
			break;
		case (CURVE_RIGHT_FORWARD):
			TPM0_C0V = calculateCnV(FULL_SPEED);				// RIGHT BACKWARD
			TPM0_C3V = calculateCnV(CURVE_SPEED);				// LEFT FORWARD
			break;
		case (CURVE_LEFT_BACKWARD):
			TPM0_C5V = calculateCnV(FULL_SPEED);				// LEFT BACKWARD
			TPM0_C2V = calculateCnV(CURVE_SPEED);				// RIGHT BACKWARD
			break;
		case (CURVE_RIGHT_BACKWARD):
			TPM0_C5V = calculateCnV(CURVE_SPEED);				// LEFT BACKWARD
			TPM0_C2V = calculateCnV(FULL_SPEED);				// RIGHT BACKWARD
			break;
		default:
			TPM0_C0V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = 0;
			TPM0_C5V = 0;
			break;			
	}
}
