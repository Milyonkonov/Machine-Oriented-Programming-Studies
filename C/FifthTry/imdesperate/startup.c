__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#include "GPIO-verbose.h"
#include "systick.h"

#define PORT_E_ADDRESS 0x40021000
#define VTOR_ADDRESS 0x2001C000

GPIO* portE = (GPIO*) PORT_E_ADDRESS;

SysTick* sysTick = (SysTick*) STK_ADDRESS;

unsigned char state = 1;
void InvertWave()
{
	portE->outputData.pin0 = state;
	state ^= 1;
}

void InitSquareWave()
{
	portE->mode.pin0 = MODE_OUTPUT;
	portE->outputType.pin0 = OTYPE_PUSH_PULL;
	portE->outputSpeed.pin0 = OSPEED_LOW;
}


void StartSquareWave(unsigned int period_in_us)
{
	sysTick->CLKSOURCE = CLK_SRC_ACTUAL;
	*((void (**)(void)) (VTOR_ADDRESS + 0x3C)) = &InvertWave;
	sysTick->TICKINT = 1;
	sysTick->LOAD = period_in_us * 1000 / 6 / 2;
	sysTick->ENABLE = 1;
}

void StopSquareWave()
{
	portE->outputData.pin0 = 0;
	sysTick->ENABLE = 0;
}

#define PERIOD 10000

void main() {
	InitSquareWave();
	StartSquareWave(PERIOD); 
	InvertWave();
	while(1) { /* Gör ingenting */ }
	StopSquareWave(); 
}