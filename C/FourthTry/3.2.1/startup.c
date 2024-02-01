/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

int pack(unsigned short year, unsigned short month, unsigned short day)
{
    return ((year & 0b1111111111111) << 9) | ((month & 0b1111) << 5) | (day & 0b11111);
}

void main(void)
{
}

