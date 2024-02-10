__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

//---Data setup and formatting---
typedef struct
{
	unsigned int MODER;
	unsigned int unused0;
	unsigned int unused1;
	unsigned int unused3;
	unsigned char unused4;
	volatile unsigned char readable_data;
	unsigned short unused5;
	union
	{
		unsigned char ctrl;
		struct
		{
			unsigned char RS : 1;
			unsigned char RW : 1;
			unsigned char SELECT : 1;
			unsigned char unused6 : 3;
			unsigned char E : 1;
		};
	};
	
	unsigned char writable_data;
}
DisplayGPIO;

#define PORT_E_ADDRESS 0x40021000
DisplayGPIO* displayPortE = (DisplayGPIO*)PORT_E_ADDRESS;

#include "systick.h"
SysTick* sysTick = (SysTick*)STK_ADDRESS;

#define PORT_OUTPUT_WRITE 1
#define PORT_INPUT_READ 0
// MUST FIRST BE USED IF READING FROM DISPLAY, although method chains might do so automatically...
void configure_dataport(unsigned char in_write_mode)
{
	displayPortE->MODER = in_write_mode ? 0x5555 : 0x0055;
}
//-------------------------------

//---Delay methods---
void delay_nano(unsigned int ns)
{
	sysTick->CTRL = 0;
	sysTick->LOAD = ns / 6;
	sysTick->CLKSOURCE = 1;
	sysTick->ENABLE = 1;
	while (sysTick->COUNTFLAG == 0) {}
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
//-------------------

//---Dumb---
// This is actually so dumb.
void ascii_ctrl_bit_set(unsigned char rs_data, unsigned char read_write, unsigned char enable)
{
	if (rs_data) 		{ displayPortE->RS = 1; }
	if (read_write) 	{ displayPortE->RW = 1; }
	if (enable) 		{ displayPortE->E = 1; }
}

void ascii_ctrl_bit_clear(unsigned char rs_data, unsigned char read_write, unsigned char enable)
{
	if (!rs_data) 		{ displayPortE->RS = 0; }
	if (!read_write) 	{ displayPortE->RW = 0; }
	if (!enable) 		{ displayPortE->E = 0; }
}
//----------

//---ASCII display communication handlers---
void ascii_write_controller(unsigned char byte)
{
	delay_nano(40);
	displayPortE->E = 1;
	displayPortE->writable_data = byte;
	delay_250ns();
	displayPortE->E = 0;
	delay_nano(10);
}

unsigned char ascii_read_controller()
{
	displayPortE->E = 1;
	delay_nano(360);
	unsigned char rv = displayPortE->readable_data;
	displayPortE->E = 0;
	return rv;
}

void ascii_write_cmd(unsigned char command)
{
	displayPortE->RS = 0;
	displayPortE->RW = 0;
	ascii_write_controller(command);
}

void ascii_write_data(unsigned char data)
{
	displayPortE->RS = 1;
	displayPortE->RW = 0;
	ascii_write_controller(data);
}

unsigned char ascii_read_status()
{
	configure_dataport(PORT_INPUT_READ);
	displayPortE->RS = 0;
	displayPortE->RW = 1;
	unsigned char rv = ascii_read_controller();
	configure_dataport(PORT_OUTPUT_WRITE);
	return rv;
}

unsigned char ascii_read_data()
{
	configure_dataport(PORT_INPUT_READ);
	displayPortE->RS = 1;
	displayPortE->RW = 1;
	unsigned char rv = ascii_read_controller();
	configure_dataport(PORT_OUTPUT_WRITE);
	return rv;
}
//------------------------------------------

//---ASCII display commands---
void await_display_ready()
{
	while(ascii_read_status() & 0b10000000) {} // Wait for BF-bit (on E7) to go to 0, since a 1 means the display is busy.
	delay_mikro(8);			// 8 microseconds have to pass in between command done and new command.
}

void clear_display()
{
	await_display_ready();
	ascii_write_cmd(0b1);
	delay_milli(2);			// Command specific delay for the clear command: 2ms.
}

void function_set_display(int multiple_rows, int big_characters)
{
	await_display_ready();
	unsigned char function_set_command = 		0b00110000;
	
	if (multiple_rows) { function_set_command |= 	0b1000; }
	if (big_characters) { function_set_command |= 	0b0100; }
	ascii_write_cmd(function_set_command);
	delay_mikro(39);		// Command specific delay for the function set command: 39us.
}

void display_control(int display_on, int cursor_on, int blinking_cursor)
{
	await_display_ready();
	unsigned char display_control_command = 		0b00001000;
	
	if (display_on) { display_control_command |= 		0b0100; }
	if (cursor_on) { display_control_command |= 		0b0010; }
	if (blinking_cursor) { display_control_command |= 	0b0001; }
	delay_mikro(39);		// Command specific delay for the display control command: 39us.
}

void display_entry_mode_set(int increment_decrement, int shift_address_buffer)
{
	await_display_ready();
	unsigned char entry_mode_set_command = 				0b00000100;
	
	if (increment_decrement) { entry_mode_set_command |= 	0b0010; }
	if (shift_address_buffer) { entry_mode_set_command |= 	0b0001; }
	ascii_write_cmd(entry_mode_set_command);
	delay_mikro(39);		// Command specific delay for the entry mode set command: 39us.
}

void ascii_gotoxy(int x, int y)
{
	unsigned char address_set_command = 0x80;
	x--;
	y--;
	address_set_command |= 64*y + x;
	ascii_write_cmd(address_set_command);
	delay_mikro(39);		// Command specific delay for the address set command: 39us.
}

void ascii_init()
{
	function_set_display(1, 0);
	display_control(1, 1, 1);
	clear_display();
	display_entry_mode_set(1, 0);
}

void ascii_write_char(unsigned char c)
{
	await_display_ready();
	ascii_write_data(c);
	delay_mikro(43);		// Command specific delay for the write operation: 43us.
}

//----------------------------

void init_app()
{
	configure_dataport(PORT_OUTPUT_WRITE);
}

int main(void)
{
	char* s;
	char test1[] = "Alfanumerisk ";
	char test2[] = "Display - test";
	
	init_app();
	ascii_init();
	ascii_gotoxy(1,1);
	s = test1;
	while (*s)
	{
		ascii_write_char(*s++);
	}
	ascii_gotoxy(1, 2);
	s = test2;
	while (*s)
	{
		ascii_write_char(*s++);
	}
	return 0;
}

