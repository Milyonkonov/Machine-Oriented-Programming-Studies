# 0 "startup.c"
# 1 "D:\\Studies\\Machine-Oriented-Programming\\Work\\Machine-Oriented-Programming-Studies\\C\\FourthTry\\basic_io//"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "startup.c"




__attribute__((naked))
__attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");
__asm__ volatile(".L1: B .L1\n");
}

void app_init ( void )
{
* ( (unsigned long *) 0x40020C00) = 0x00005555;
}
void main(void)
{
    unsigned char c;
    app_init();
    while(1)
    {
        c = (unsigned char) *(( unsigned short *) 0x40021010);
        * ( (unsigned char *) 0x40020C14) = c;
    }
}
