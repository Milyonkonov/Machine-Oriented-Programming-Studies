__attribute__((naked)) 
__attribute__((section (".start_section")) )
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

typedef struct
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
	
	union {
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

#define GPIO_D 0x40020C00
GPIO* portD = (GPIO*) GPIO_D; 

void app_init (void)
{
	portD->MODER_LOW = 0x55;	// D0-D7 as output for 7-segment display.
	portD->OTYPER_LOW = 0;		// D0-D7 as push-pull.
	
	// Lower nibble of higher byte, the columns, are input with pull-up.
	portD->mode.pin8 = MODE_INPUT;
	portD->mode.pin9 = MODE_INPUT;
	portD->mode.pin10 = MODE_INPUT;
	portD->mode.pin11 = MODE_INPUT;
	portD->pullUpPullDown.pin8 = PUPD_PULL_UP;
	portD->pullUpPullDown.pin9 = PUPD_PULL_UP;
	portD->pullUpPullDown.pin10 = PUPD_PULL_UP;
	portD->pullUpPullDown.pin11 = PUPD_PULL_UP;
	
	// Higher nibble of higher byte, the rows, are output in open-drain.
	portD->mode.pin12 = MODE_OUTPUT;
	portD->mode.pin13 = MODE_OUTPUT;
	portD->mode.pin14 = MODE_OUTPUT;
	portD->mode.pin15 = MODE_OUTPUT;
	portD->outputType.pin12 = OTYPE_OPEN_DRAIN;
	portD->outputType.pin13 = OTYPE_OPEN_DRAIN;
	portD->outputType.pin14 = OTYPE_OPEN_DRAIN;
	portD->outputType.pin15 = OTYPE_OPEN_DRAIN;
}

void activateRow (char row)
{
	// TODO
}

unsigned char readColumn (char column)
{
	// TODO
	return 0;
}

unsigned char keyb(void)
{
	// TODO
	return 0xFF;
}

void out7Seg (unsigned char c)
{
	// TODO
}

int main(void)
{
	app_init();
	while(1)
	{
		out7Seg(keyb());
	}
}

