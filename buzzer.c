#include "buzzer.h"
#include "MKL25Z4.h"
#include "cmsis_os2.h"

//Travelling song
const unsigned long travellingNotes[][2] = {
	{D4, QUAVER},
	{B4, QUAVER + SEMIQUAVER},
	{A4, SEMIQUAVER},
	{G4, QUAVER},
	{G4, QUAVER},
	{Gb4, QUAVER + SEMIQUAVER},
	{G4, SEMIQUAVER},
	{A4, QUAVER},
	{D4, QUAVER},
	{Z5, QUAVER + SEMIQUAVER},
	{B4, SEMIQUAVER},
	{A4, QUAVER},
	{A4, QUAVER},
	{G4, QUAVER + SEMIQUAVER},
	{A4, SEMIQUAVER},
	{B4, QUAVER},
	{G4, SEMIQUAVER},
	{D5, SEMIQUAVER},
	{D5, QUAVER},
	{D5, QUAVER},
	{D5, QUAVER},
	{Eb5, QUAVER},
	{E5, QUAVER + SEMIQUAVER},
	{Z5, SEMIQUAVER},
	{A4, QUAVER},
	{A4, QUAVER},
	{B4, QUAVER + SEMIQUAVER},
	{B4, SEMIQUAVER},
	{Z5, SEMIQUAVER},
	{D4, SEMIQUAVER},
	{Z5, SEMIQUAVER},
	{A4, SEMIQUAVER},
	{A4, CROTCHET},
	{G4, QUAVER},
	{375000, QUAVER}//SORGEN, 0:27 
};

//Ending song
const unsigned long DoReMi_notes[][2] = {
	{Z4, CROTCHET2},
	{D4, CROTCHET2},
	{E4, CROTCHET2},
	{Z4, CROTCHET2},
	{Z4, CROTCHET2},
	{D4, CROTCHET2},
	{E4, CROTCHET2},
	{Z4, CROTCHET2},
	{E4, CROTCHET2},
	{F4, CROTCHET2},
	{G4, CROTCHET2*2},
	{E4, CROTCHET2},
	{F4, CROTCHET2},
	{G4, CROTCHET2*2},
	{G4, CROTCHET2/2},
	{A4, CROTCHET2/2},
	{G4, CROTCHET2/2},
	{F4, CROTCHET2/2},
	{E4, CROTCHET2},
	{Z4, CROTCHET2},
	{G4, CROTCHET2/2},
	{A4, CROTCHET2/2},
	{G4, CROTCHET2/2},
	{F4, CROTCHET2/2},
	{E4, CROTCHET2},
	{Z4, CROTCHET2},
	{Z4, CROTCHET2},
	{G3, CROTCHET2},
	{Z4, CROTCHET2*2},
	{Z4, CROTCHET2},
	{G3, CROTCHET2},
	{Z4, CROTCHET2*2},
	{375000, CROTCHET2*100}
};

//Connecting Song
const unsigned long connectingSong[][2] = {
	{Z4, QUAVER},
	{D4, QUAVER},
	{E4, QUAVER},
	{F4, QUAVER},
	{G4, QUAVER},
	{A4, QUAVER},
	{B4, QUAVER},
	{Z5, QUAVER},
	{375000, QUAVER}
};

void initPWM(void){
	//Enable clock for port B
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	//Configure Mode 3 for the PWM pin operation
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	//Enable Clock Gating for Timer1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //MCGFLLCLK or MCGPLLCLK/2
		
	//Update SnC register: CMOD = 01, PS = 111(128)
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7)); //LPTPM counter increments on every LPTPM counter clock and //128
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK); //upcounting 
	
	/*Edge-Aligned PWM*/
	//Enable PWM on TPM1 Channel 0 -> PTB0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));	
}

void playNote(int freq){
	TPM1->MOD = FREQ_2_MOD(freq);
	TPM1_C0V = FREQ_2_MOD(freq) / 2;
}

void playConnectSong() {
	for(int i=0; i<START_SONG_LENGTH; i++){
		playNote(connectingSong[i][0]);
		osDelay(connectingSong[i][1]);
	}
}

void playRunningSong() {
	for(int i=0; i < RUNNING_SONG_LENGTH; i++){
		playNote(travellingNotes[i][0]);
		osDelay(travellingNotes[i][1]);
	}
}

void playEndSong() {
	for(int i=0; i<END_SONG_LENGTH; i++){
		playNote(DoReMi_notes[i][0]);
		osDelay(DoReMi_notes[i][1]);
	}
}
