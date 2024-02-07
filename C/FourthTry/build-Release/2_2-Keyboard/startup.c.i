# 0 "startup.c"
# 1 "D:\\Studies\\Machine-Oriented-Programming\\Work\\Machine-Oriented-Programming-Studies\\C\\FourthTry\\2.2-Keyboard//"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "startup.c"





unsigned char keyValue[4][4] =
{
{ 1, 2, 3, 10},
{ 4, 5, 6, 11},
{ 7, 8, 9, 12},
{14, 0, 15, 13}
};

unsigned char segments[16] = {
 0b0111111,
 0b0000110,
 0b1011011,
 0b1001111,
 0b1100110,
 0b1101101,
 0b1111101,
 0b0000111,
 0b1111111,
 0b1101111,
 0b1110111,
 0b1111100,
 0b0111001,
 0b1011110,
 0b1111001,
 0b1110001
};

__attribute__((naked))
__attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");
__asm__ volatile(".L1: B .L1\n");
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



typedef OneBitPerPin OutputTypeMap;



typedef TwoBitsPerPin OutputSpeedMap;





typedef TwoBitsPerPin PullUpPullDownMap;





typedef struct
{

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


GPIO* portD = (GPIO*) 0x40020C00;

void app_init (void)
{
 portD->MODER_LOW = 0x55;
 portD->OTYPER_LOW = 0;


 portD->mode.pin8 = 0b00;
 portD->mode.pin9 = 0b00;
 portD->mode.pin10 = 0b00;
 portD->mode.pin11 = 0b00;
 portD->pullUpPullDown.pin8 = 0b01;
 portD->pullUpPullDown.pin9 = 0b01;
 portD->pullUpPullDown.pin10 = 0b01;
 portD->pullUpPullDown.pin11 = 0b01;


 portD->mode.pin12 = 0b01;
 portD->mode.pin13 = 0b01;
 portD->mode.pin14 = 0b01;
 portD->mode.pin15 = 0b01;
 portD->outputType.pin12 = 1;
 portD->outputType.pin13 = 1;
 portD->outputType.pin14 = 1;
 portD->outputType.pin15 = 1;
}





void setRowState (int row, int newState)
{
 if (row == 0) {portD->outputData.pin12 = newState; }
 else if (row == 1) {portD->outputData.pin13 = newState; }
 else if (row == 2) {portD->outputData.pin14 = newState; }
 else if (row == 3) {portD->outputData.pin15 = newState; }
}


unsigned char readColumns ()
{


 if (portD->inputData.pin8 == 0) { return 0; }
 if (portD->inputData.pin9 == 0) { return 1; }
 if (portD->inputData.pin10 == 0) { return 2; }
 if (portD->inputData.pin11 == 0) { return 3; }

 return 0xFF;
}

unsigned char keyb(void)
{
 for (int row = 0; row < 4; row++) {
  setRowState(row, 0);

  unsigned char columnPressed = readColumns ();
  if (columnPressed != 0xFF)
  {
   return keyValue[row][columnPressed];
  }

  setRowState(row, 1);
 }

 return 0xFF;
}

void out7Seg (unsigned char c)
{
 if (c > 15)
 {
  portD->ODR_LOW = 0;
 }
 else
 {
  portD->ODR_LOW = segments[c];
 }
}

void main(void)
{
 app_init();
 while(1)
 {
  out7Seg(keyb());
 }
}
