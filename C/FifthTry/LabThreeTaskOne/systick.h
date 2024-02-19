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