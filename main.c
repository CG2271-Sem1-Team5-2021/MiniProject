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

osMutexId_t audioMutex;
osSemaphoreId_t dataSemaphore;

//Prioritity of tPlayEndSong
const osThreadAttr_t above_normal_attr = {
    .priority = osPriorityAboveNormal
};

const osThreadAttr_t high_priority_attr = {
		.priority = osPriorityHigh
};

void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	if(UART2->S1 & UART_S1_TDRE_MASK) {
	}	
	if(UART2->S1 & UART_S1_RDRF_MASK) {
		rxData = UART2->D;
		osSemaphoreRelease(dataSemaphore);
	}
}

//Decode the data from Serial Port and perfrom the necessary action
void tBrain (void *argument) {
	for(;;) {
		osSemaphoreAcquire(dataSemaphore, osWaitForever);
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
			osMutexAcquire(audioMutex, osWaitForever);
			if(osEventFlagsGet(end_race_flag) == 0x0001) {
				osMutexRelease(audioMutex);
			}
			playNote(travellingNotes[i][0]);
			osDelay(travellingNotes[i][1]);
			osMutexRelease(audioMutex);
		}
	}
}

//Play the ending tune
void tPlayEndSong(void *argument) {
	for(;;) {
		osEventFlagsWait(end_race_flag, 0x0001, osFlagsWaitAny, osWaitForever);
		osEventFlagsClear(end_race_flag, 0x0001);
		osMutexAcquire(audioMutex, osWaitForever);
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
	audioMutex = osMutexNew(NULL);
	dataSemaphore = osSemaphoreNew(1,0, NULL);
	start_flag = osEventFlagsNew(NULL);
	end_race_flag = osEventFlagsNew(NULL);

	osThreadNew(tBrain, NULL, &above_normal_attr); 	
  osThreadNew(tMotorControl, NULL, NULL); 	 
	osThreadNew(tLED, NULL, NULL);										
	osThreadNew(tAudio, NULL, NULL);									
	osThreadNew(tPlayEndSong, NULL, &high_priority_attr);	
	osKernelStart();                      						
  
	for (;;) {
	}
}
