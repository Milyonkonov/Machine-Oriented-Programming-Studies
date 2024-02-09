__attribute__((naked)) 
__attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#include "GPIO-verbose.h"

#define GPIO_D 0x40020C00
GPIO* portD = (GPIO*) GPIO_D; 

void app_init (void)
{
	portD->MODER_LOW = 0x5555;	// D0-D7 as output for 7-segment display.
	portD->OTYPER_LOW = 0;		// D0-D7 as push-pull.
	
	// Lower nibble of higher byte, the columns, are input without pull-up since it's broken on simulator.
	portD->mode.pin8 = MODE_INPUT;
	portD->mode.pin9 = MODE_INPUT;
	portD->mode.pin10 = MODE_INPUT;
	portD->mode.pin11 = MODE_INPUT;
	/*
	portD->pullUpPullDown.pin8 = PUPD_PULL_UP;
	portD->pullUpPullDown.pin9 = PUPD_PULL_UP;
	portD->pullUpPullDown.pin10 = PUPD_PULL_UP;
	portD->pullUpPullDown.pin11 = PUPD_PULL_UP;
	*/
	
	// Higher nibble of higher byte, the rows, are output in open-drain.
	portD->mode.pin12 = MODE_OUTPUT;
	portD->mode.pin13 = MODE_OUTPUT;
	portD->mode.pin14 = MODE_OUTPUT;
	portD->mode.pin15 = MODE_OUTPUT;
	// Simulator doesn't understand pull-up so we have to use push-pull (which would short output pins, bruh)
	portD->outputType.pin12 = OTYPE_PUSH_PULL;
	portD->outputType.pin13 = OTYPE_PUSH_PULL;
	portD->outputType.pin14 = OTYPE_PUSH_PULL;
	portD->outputType.pin15 = OTYPE_PUSH_PULL;

}

// Changes the state of a single row to newState.
void setRowState (int row, int newState)
{
	if (row == 0) 		{portD->outputData.pin12 = newState; }
	else if (row == 1) 	{portD->outputData.pin13 = newState; }
	else if (row == 2) 	{portD->outputData.pin14 = newState; }
	else if (row == 3) 	{portD->outputData.pin15 = newState; }
}

// Returns a column with a pressed button. 0xFF is returned with no button pressed.
unsigned char readColumns ()
{
	// In the simulator we actually want to look for 1's in the IDR, since we don't have pull-up.
	if (portD->inputData.pin8 == 1) { return 0; }
	if (portD->inputData.pin9 == 1) { return 1; }
	if (portD->inputData.pin10 == 1) { return 2; }
	if (portD->inputData.pin11 == 1) { return 3; }
	
	return 0xFF;
}

unsigned char keyValue[4][4] = 
{
{ 1, 2,  3, 10},
{ 4, 5,  6, 11},
{ 7, 8,  9, 12},
{14, 0, 15, 13}	
};

unsigned char keyb(void)
{
	for (int row = 0; row < 4; row++) {
		
		// setRowState in the opposite order from real hardware, since we have to read the 1's without pull-up...
		setRowState(row, 1);
		unsigned char columnPressed = readColumns ();
		setRowState(row, 0);
		
		if (columnPressed != 0xFF)
		{
			return keyValue[row][columnPressed];
		}
	}
	
	return 0xFF;
}

char segments[16] = {
	0b0111111,  // 0
	0b0000110,  // 1
	0b1011011,  // 2
	0b1001111,  // 3
	0b1100110,  // 4
	0b1101101,  // 5
	0b1111101,  // 6
	0b0000111,  // 7
	0b1111111,  // 8
	0b1101111,  // 9
	0b1110111,  // A
	0b1111100,  // b
	0b0111001,  // C
	0b1011110,  // d
	0b1111001,  // E
	0b1110001   // F
};

void out7Seg (unsigned char c)
{
	if (c > 15)
	{
		portD->ODR_LOW = 0;
	}
	else
	{
		portD->ODR_LOW = segments[c];
	}
}

int main(void)
{
	app_init();
	while(1)
	{
		out7Seg(keyb());
	}
}