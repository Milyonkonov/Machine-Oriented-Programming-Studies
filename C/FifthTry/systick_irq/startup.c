__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define SIMULATOR
#ifdef SIMULATOR
unsigned int DELAY_COUNT = 100;
#else
unsigned int DELAY_COUNT = 1000000;
#endif

#define VTOR 0x2001C000

#include "GPIO-verbose.h"
#include "systick.h"

GPIO* gpioD = (GPIO*) 0x40020C00;
SysTick* systick = (SysTick*)STK_ADDRESS;

unsigned int delay_count;
unsigned int systick_flag;

void delay_1mikro ()
{
	systick_flag = 0;
	systick->TICKINT = 1;
	systick->LOAD = 1000 / 6;
	systick->ENABLE = 1;
}

void systick_irq_handler ()
{
	systick->ENABLE = 0;
	delay_count--;
	if (delay_count > 0)
	{
		delay_1mikro();
	}
	else
	{
		systick_flag = 1;
	}
}

void delay (unsigned int count)
{
	delay_count = count;
	delay_1mikro();
}

void init_app ()
{
	gpioD->MODER_LOW = 0x5555;
	
	*((void (**)(void)) (VTOR + 0x3C)) = &systick_irq_handler;
	systick_flag = 0;
}

int main (void)
{
	init_app();
	gpioD->ODR_LOW = 0;
	delay(DELAY_COUNT);
	gpioD->ODR_LOW = 0xFF;
	while (1)
	{
		if (systick_flag)
		{
			break;
		}
		// Code that can be ran in the delay time can be placed here.
	}
	// Code that runs after the delay is done can be placed here.
	gpioD->ODR_LOW = 0;
}