#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

void initUART2(int);

#define FORWARD_PACKET 			0b00000001
#define BACKWARD_PACKET 		0b00000010
#define LEFT_PACKET 				0b00000100
#define RIGHT_PACKET 				0b00001000
#define STOP_PACKET 				0b00010000
#define INITIALIZE_PACKET 	0b00100000
#define END_PACKET 					0b01000000

