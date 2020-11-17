/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "uart.h"
#include "motor.h"
#include "buzzer.h"
#include "led.h"
#include <limits.h>

volatile unsigned char rxData = 0x0;
volatile uint8_t dir = STOP;

osEventFlagsId_t start_flag;
osEventFlagsId_t end_race_flag;

void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	if(UART2->S1 & UART_S1_TDRE_MASK) {
	}	
	if(UART2->S1 & UART_S1_RDRF_MASK) {
		rxData = UART2->D;
	}
}

//Decode the data from Serial Port and perfrom the necessary action
void tBrain (void *argument) {
	for(;;) {
		if (rxData == INITIALIZE_PACKET) {
			osEventFlagsSet(start_flag, 0x0001);
		} else if (rxData == END_PACKET) {
			osEventFlagsSet(end_race_flag, 0x0001);
		} else if (rxData == STOP_PACKET) {
			dir = STOP;
		} else if (rxData == FORWARD_PACKET || rxData == BACKWARD_PACKET || rxData == LEFT_PACKET || rxData == RIGHT_PACKET) {
			dir |= rxData; 
		}
	}
}

//Control the Action of the Motors
void tMotorControl (void *argument) {
  for (;;) {
		move(dir);
	}
}

//Control the LEDs
void tLED (void *argument) {
	osEventFlagsWait(start_flag, 0x0001, osFlagsWaitAny, osWaitForever);
	blinkGreenLedTwice();
	for (;;) {
		if (dir == STOP) {
			ledControlStopMode();
		} else {
			ledControlMovingMode();
		}
	}	
}

//Provide Audio Output at the beginning and while running
void tAudio(void *argument){
	osEventFlagsWait(start_flag, 0x0001, osFlagsWaitAny, osWaitForever);
	initPWM();
	playConnectSong();
	for (;;) {
		for(int i=0; i < RUNNING_SONG_LENGTH; i++){
			if(osEventFlagsGet(end_race_flag) == 0x0001) {
				osDelay(INT_MAX);
			}
			playNote(travellingNotes[i][0]);
			osDelay(travellingNotes[i][1]);
		}
	}
}

//Play the ending tune
void tPlayEndSong(void *argument) {
	for(;;) {
		osEventFlagsWait(end_race_flag, 0x0001, osFlagsWaitAny, osWaitForever);
		osEventFlagsClear(end_race_flag, 0x0001);
		playEndSong();
		SIM->SCGC6 &= ~SIM_SCGC6_TPM1_MASK;
		SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	}
}
 
int main (void) {
	
	// System Initialization
  SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	initGPIO();
	initMotor();

  osKernelInitialize();                 // Initialize CMSIS-RTOS        

	start_flag = osEventFlagsNew(NULL);
	end_race_flag = osEventFlagsNew(NULL);

	osThreadNew(tBrain, NULL, NULL); 	
  osThreadNew(tMotorControl, NULL, NULL); 	 
	osThreadNew(tLED, NULL, NULL);										
	osThreadNew(tAudio, NULL, NULL);									
	osThreadNew(tPlayEndSong, NULL, NULL);	
	osKernelStart();                      						
  
	for (;;) {
	}
}
