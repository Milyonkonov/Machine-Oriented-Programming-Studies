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