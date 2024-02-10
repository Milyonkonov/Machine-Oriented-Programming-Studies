#define SIMULATOR 1
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define GPIO_E 0x40021000

#include "systick.h"

#define STK_ADDRESS 0xE000E010
SysTick* sysTick = (SysTick*)STK_ADDRESS;

void set_bargraph(unsigned char state) 
{
	*(unsigned char*)(GPIO_E+0x14) = state;
}

void init_app()
{
	*(unsigned char*)GPIO_E = 0x5555;	// E0-E7 as output, to the bargraph.
	sysTick->CTRL = 0;
}

void delay_250ns()
{
	sysTick->CTRL = 0;
	sysTick->LOAD = 42;
	sysTick->CLKSOURCE = 1;
	sysTick->ENABLE = 1;
	while (sysTick->COUNTFLAG == 0) {}
	sysTick->CTRL = 0;
}

void delay_mikro(unsigned int us)
{
	for (int i = 0; i < 4*us; i++)
	{
		delay_250ns();		// 250 * 4 ns = 1000ns
	}
}

void delay_milli(unsigned int ms)
{
	#ifdef SIMULATOR
	ms /= 1000;
	ms++;
	#endif
	
	for (int i = 0; i < ms; i++)
	{
		delay_mikro(1000);		// 1000 microseconds = 1 millisecond
	}
}

void main(void)
{
	init_app();
	while(1)
	{
		set_bargraph(0x0);
		delay_milli(500);
		set_bargraph(0xFF);
		delay_milli(500);
	}
}

