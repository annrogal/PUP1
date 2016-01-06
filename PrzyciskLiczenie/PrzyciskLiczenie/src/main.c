#define F_CPU 16000000
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "hd44780.h"
#include "hd44780.c"

volatile char buffer[20];

int main(void)
{
	DDRD=0b0;
	DDRD=0xff;
	int y=1234;

	LCD_Initalize();
	LCD_Clear();

	itoa(y, buffer, 10);


	LCD_Clear();
	LCD_WriteText("abcd");
	LCD_GoTo(0,1);
	LCD_WriteText(buffer);
	_delay_ms(5);



}