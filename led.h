#include "MKL25Z4.h"

#define RED_LED 					8 	// PortC Pin 8		
#define GREEN_LED_1       11  // PortC Pin 11 
#define GREEN_LED_2       10  // PortC Pin 10
#define GREEN_LED_3       6   // PortC Pin 6 
#define GREEN_LED_4       5   // PortC Pin 5 
#define GREEN_LED_5       4   // PortC Pin 4 
#define GREEN_LED_6       3   // PortC Pin 3 
#define GREEN_LED_7       0   // PortC Pin 0 	
#define GREEN_LED_8      	7   // PortC Pin 7

#define NUM_OF_GREEN_LEDS 8

#define MASK(x)         (1 << (x)) 

#define MASK_RED_LED 0x00000100
#define MASK_ALL_GREEN 0x00000CF9 //0b 1100 1111 1001

#define LED_DELAY_MOVING		500		
#define LED_DELAY_STATIONERY 		250	
#define BLINK_CONNECTION 250
								
extern const int LEDSequence[8];

void initGPIO(void);
void blinkGreenLedTwice(void);
void ledControlMovingMode(void);
void ledControlStopMode(void);

