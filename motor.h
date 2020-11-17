#define FORWARD_RIGHT	 	0		//PortD Pin 0
#define BACKWARD_RIGHT	2		//PortD Pin 2
#define FORWARD_LEFT		3 	//PortD Pin 3
#define BACKWARD_LEFT		5		//PortD Pin 5

#define STOP											0b00000000 //0
#define FORWARD 									0b00000001 //1
#define BACKWARD 									0b00000010 //2
#define LEFT											0b00000100 //4
#define RIGHT											0b00001000 //8
#define CURVE_LEFT_FORWARD			  0b00000101 //5
#define CURVE_RIGHT_FORWARD			  0b00001001 //9
#define	CURVE_LEFT_BACKWARD			  0b00000110 //6
#define	CURVE_RIGHT_BACKWARD			0b00001010 //10
	
#define IS_MOVING(x)	(x & 0x1)

#define FULL_SPEED 100		
#define CURVE_SPEED 75		

#define MOD_VALUE 0xFFF

#define MASK_DIRECTION 0xf

int calculateCnV(int);
void initMotor(void);
void move(int);
