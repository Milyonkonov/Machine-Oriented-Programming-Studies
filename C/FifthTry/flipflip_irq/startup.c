__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define PORTD 0x40020C00
#define PORTE 0x40021000
#define NVIC 0xE000E100
#define SYSCFG 0x40013800
#define EXTI 0x40013C00
#define EXTI3_OFFSET 0x64
#define SCB_VTOR (unsigned int*)(0xE000ED00 + 0x8)

unsigned short count = 0;

void plus_count()
{
	count++;
	*(unsigned int*) (EXTI + 0x14) |= 1 << 3;	// Declare interrupt as done.
}

void init_app()
{
	*(unsigned int*) PORTD = 0x5555;			// PD0-7 as output for double hexadecimal display.
	
	*(unsigned short*) (SYSCFG + 8)	&= ~(0xF << (4 * 3));
	*(unsigned short*) (SYSCFG + 8)	|= 0x4 << (4 * 3); 	// Route PE3 to EXTI3.
	
	*(unsigned int*) EXTI |= 1 << 3; 		// Configure EXTI3 to generate interrupts.
	*(unsigned int*) (EXTI + 8) |= 1 << 3;	// Configure interrupts to rising edge.
	
	// Set up interrupt vector
	*SCB_VTOR = 0x2001C000;					// Relocate the vector table (I don't know why).
	*(unsigned int*) NVIC |= 1 << 9;		// EXTI3 = IRQ 9, enable IRQ 9 in NVIC.
	*((void (**)(void)) (0x2001C000 + EXTI3_OFFSET)) = &plus_count;	// Set interrupt handler
	
}

int main(void)
{
	init_app();
	while(1)
	{
		*(unsigned int*) (PORTD + 0x14) = count;
	}
}

