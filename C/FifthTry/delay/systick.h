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
	
	unsigned int VAL;
}
SysTick;