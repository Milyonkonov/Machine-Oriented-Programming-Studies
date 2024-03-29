__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

typedef struct 
{
	unsigned char pin0 : 2;
	unsigned char pin1 : 2;
	unsigned char pin2 : 2;
	unsigned char pin3 : 2;
	unsigned char pin4 : 2;
	unsigned char pin5 : 2;
	unsigned char pin6 : 2;
	unsigned char pin7 : 2;
	unsigned char pin8 : 2;
	unsigned char pin9 : 2;
	unsigned char pin10 : 2;
	unsigned char pin11 : 2;
	unsigned char pin12 : 2;
	unsigned char pin13 : 2;
	unsigned char pin14 : 2;
	unsigned char pin15 : 2;
}
TwoBitsPerPin;

typedef struct 
{
	unsigned char pin0 : 1;
	unsigned char pin1 : 1;
	unsigned char pin2 : 1;
	unsigned char pin3 : 1;
	unsigned char pin4 : 1;
	unsigned char pin5 : 1;
	unsigned char pin6 : 1;
	unsigned char pin7 : 1;
	unsigned char pin8 : 1;
	unsigned char pin9 : 1;
	unsigned char pin10 : 1;
	unsigned char pin11 : 1;
	unsigned char pin12 : 1;
	unsigned char pin13 : 1;
	unsigned char pin14 : 1;
	unsigned char pin15 : 1;
}
OneBitPerPin;

typedef TwoBitsPerPin PortModeMap;
#define MODE_INPUT 0b00
#define MODE_OUTPUT 0b01

typedef OneBitPerPin OutputTypeMap;
#define OTYPE_PUSH_PULL 0
#define OTYPE_OPEN_DRAIN 1

typedef TwoBitsPerPin OutputSpeedMap;
#define OSPEED_LOW 0b00
#define OSPEED_MEDIUM 0b01
#define OSPEED_FAST 0b10
#define OSPEED_HIGHERTHANFAST 0b11

typedef TwoBitsPerPin PullUpPullDownMap;
#define PUPD_FLOATING 0b00
#define PUPD_PULL_UP 0b01
#define PUPD_PULL_DOWN 0b10
#define PUPD_RESERVED 0B11

typedef volatile struct
{
	// Could be even more defined but I already spent 3 hours on this...
	union {
		unsigned int MODER;
		struct { unsigned short MODER_LOW, MODER_HIGH; };
		PortModeMap mode;
	};
	
	union {
		unsigned short OTYPER;
		struct { unsigned char OTYPER_LOW, OTYPER_HIGH; };
		OutputTypeMap outputType;
	};
	
	union {
		unsigned int OSPEEDR;
		struct { unsigned short OSPEEDR_LOW, OSPEEDR_HIGH; };
		OutputSpeedMap outputSpeed;
	};
	
	union {
		unsigned int PUPDR;
		struct { unsigned short PUPDR_LOW, PUPDR_HIGH; };
		PullUpPullDownMap pullUpPullDown;
	};
	
	volatile union {
		unsigned short IDR;
		struct { unsigned char IDR_LOW, IDR_HIGH; };
		OneBitPerPin inputData;
	};
	
	unsigned short unused0;
	
	union {
		unsigned short ODR;
		struct { unsigned char ODR_LOW, ODR_HIGH; };
		OneBitPerPin outputData;
	};
}
GPIO;

typedef struct 
{
	union {
		unsigned int CTRL;
		struct {
			unsigned int ENABLE : 1;
			unsigned int TICKINT : 1;
			unsigned int CLKSOURCE : 1;
			unsigned int unused0 : 13;
			volatile unsigned int COUNTFLAG :1;
		};
	};
	
	unsigned int LOAD;
	
	volatile unsigned int VAL;
}
SysTick;

#define STK_ADDRESS 0xE000E010
#define CLK_SRC_ACTUAL 1
#define CLK_SRC_EIGHTH 0

typedef struct 
{
	unsigned int IMR;	// Interrupt Mask Register. (1=interrupt enabled, 0=interrupt disabled)
	unsigned int EMR;	// Event Mask Register.
	unsigned int RTSR; 	// Rising Trigger Selection Register.
	unsigned int FTSR;	// Falling Trigger Selection Register.
	unsigned int SWIER;	// Software Interrupt Event Register. Can be set to trigger interrupt programmatically.
	unsigned int PR;	// Pending Register. Interrupt handlers MUST set a 1 here to acknowledge interrupt as complete!(!!)
}
EXTI;

#define EXTI_ADDRESS 0x40013C00

#define SYSCFG_ADDRESS 0x40013800
#define PORT_E_ADDRESS 0x40021000
#define VTOR_ADDRESS 0x2001C000
#define EXTI9_5_OFFSET 0x9C
#define EXTI15_10_OFFSET 0xE0
#define NVIC_ADDRESS 0xE000E100

GPIO* portE = (GPIO*) PORT_E_ADDRESS;
SysTick* sysTick = (SysTick*) STK_ADDRESS;
EXTI* exti = (EXTI*) EXTI_ADDRESS;
	
// Changes the state of a single row to newState.
void setRowState (int row, int newState)
{
	if (row == 0) 		{portE->outputData.pin12 = newState; }
	else if (row == 1) 	{portE->outputData.pin13 = newState; }
	else if (row == 2) 	{portE->outputData.pin14 = newState; }
	else if (row == 3) 	{portE->outputData.pin15 = newState; }
}

// Returns a column with a pressed button. 0xFF is returned with no button pressed.
unsigned char readColumns ()
{
	// In the simulator we actually want to look for 1's in the IDR, since we don't have pull-up.
	if (portE->inputData.pin8 == 1) { return 0; }
	if (portE->inputData.pin9 == 1) { return 1; }
	if (portE->inputData.pin10 == 1) { return 2; }
	if (portE->inputData.pin11 == 1) { return 3; }
	
	return 0xFF;
}

unsigned char keyValue[4][4] = 
{
{ 1, 2,  3, 10},
{ 4, 5,  6, 11},
{ 7, 8,  9, 12},
{14, 0, 15, 13}	
};
	
// Returns the hexadecimal digit pressed on the keypad, or 0xFF if none is pressed.
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
	
volatile unsigned char currently_pressed_key = 0xFF; // Global variabel som skall vara 0xFF om ingen tangent
											// är nedtryckt och annars identifierar den nedtryckta
											// knappen. 
								
void KeyPressedHandler()
{
	exti->IMR = 0;		// Mask interrupts so the activating of the rows to read which key is pressed doesn't cause interrupts..?
	
	// keyb() activates each row sequentially to read which key is pressed, so all rows must be low when function is called.
	portE->ODR_HIGH = 0;
	currently_pressed_key = keyb();
	portE->ODR_HIGH = 0xF0;		// Activate all rows again to "listen" for keypress interrupts.
	
	exti->IMR |= 0b1111 << 8;	// Allow interrupts again
	exti->PR |= 0b1111 << 8;	// Acknowledge interrupt as complete for all pins.
}
									 
// Den här funktionen skall konfigurera övre byten av Port E och aktivera ALLA rader på keyboarden.
// Interrupts skall initieras så att en interrupt handler kallas när någon knapp trycks ner. 
// Interrupthandlern skall uppdatera "currently_pressed_key"
void InitKeyboard()
{
	// <--Mode configuration for port E-->
	// Lower nibble of higher byte, the columns, are input without pull-up since it's broken on simulator.
	portE->mode.pin8 = MODE_INPUT;
	portE->mode.pin9 = MODE_INPUT;
	portE->mode.pin10 = MODE_INPUT;
	portE->mode.pin11 = MODE_INPUT;
	
	// Higher nibble of higher byte, the rows, are output in open-drain.
	portE->mode.pin12 = MODE_OUTPUT;
	portE->mode.pin13 = MODE_OUTPUT;
	portE->mode.pin14 = MODE_OUTPUT;
	portE->mode.pin15 = MODE_OUTPUT;
	// Simulator doesn't understand pull-up so we have to use push-pull (which would short output pins, bruh)
	portE->outputType.pin12 = OTYPE_PUSH_PULL;
	portE->outputType.pin13 = OTYPE_PUSH_PULL;
	portE->outputType.pin14 = OTYPE_PUSH_PULL;
	portE->outputType.pin15 = OTYPE_PUSH_PULL;
	
	portE->ODR_HIGH = 0xF0;		// Activate all rows (to sense a press for interrupts)
	
	// <--Configure interrupts-->
	// Connect port E to external interrupts on pin 8-11 through SYSCFG.
	*(unsigned short*) (SYSCFG_ADDRESS + 0x10) = 0x4444; // Mask for E = 0100 = 0x4 (* 4 nibbles)
	
	// EXTI specifies which pins can cause an interrupt and under what condition:
	exti->IMR |= 0b1111 << 8;	// Pins 8-11 can activate interrupts...
	exti->RTSR |= 0b1111 << 8;	// on a rising edge, or rather, a button press.
	
	// Setting interrupt handler pointers in vector table.
	// Expecting interrupts on pins 8-11 which spans over both higher interrupt lines.
	*(void (**)(void)) (VTOR_ADDRESS + EXTI9_5_OFFSET) = &KeyPressedHandler;
	*(void (**)(void)) (VTOR_ADDRESS + EXTI15_10_OFFSET) = &KeyPressedHandler;
	
	//Enable IRQ 23 and IRQ 40 in NVIC (so interrupts actually calls our handler function) 
	*(unsigned int*) NVIC_ADDRESS = 1 << 23;
	*(unsigned int*) (NVIC_ADDRESS + 4) = 1 << 8;
}

void InvertWave()
{
	portE->outputData.pin0 ^= 1;
}

// I den här funktionen skall lägre byten av Port E förberedas för att lägga
// ut en signal på pinne 0. Den skall bara kallas en gång. 
void InitSquareWave()
{
	portE->mode.pin0 = MODE_OUTPUT;
	portE->outputType.pin0 = OTYPE_PUSH_PULL;
	portE->outputSpeed.pin0 = OSPEED_LOW; // Jag vet inte hur snabbt ni vill ha det men men... ¯\_(ツ)_/¯
}

// När man kallat den här funktionen skall en fyrkantsvåg med given period 
// (i mikrosekunder) läggas ut på pinne 0, Port E. Perioden skall kunna 
// vara upp till 99000 mikrosekunder.
void StartSquareWave(unsigned int period_in_us)
{
	sysTick->CLKSOURCE = CLK_SRC_ACTUAL;
	*((void (**)(void)) (VTOR_ADDRESS + 0x3C)) = &InvertWave;
	sysTick->TICKINT = 1;
	sysTick->LOAD = period_in_us * 1000 / 6 / 2; // Perioden måste halveras eftersom vågen ska byta nivå 2 gånger per period.
	sysTick->ENABLE = 1;
}

// Den här funktionen skall avbryta fyrkantsvågen. 
void StopSquareWave()
{
	portE->outputData.pin0 = 0;
	sysTick->ENABLE = 0;
}

#define PERIOD 1000

void main()
{
  InitSquareWave();
  InitKeyboard();
  StartSquareWave(PERIOD); 
  while(currently_pressed_key != 0) { /* Gör ingenting */ }
  StopSquareWave(); 
  while(currently_pressed_key != 5) { /* Gör ingenting */ }
  StartSquareWave(PERIOD * 2); 
  while(currently_pressed_key != 10) { /* Gör ingenting */ }
  StopSquareWave(); 
}

