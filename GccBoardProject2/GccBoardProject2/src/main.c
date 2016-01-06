#define F_PCU 16000000
#include <hd44780.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void) 
{
	LCD_Initalize();
	LCD_Clear();
	LCD_Home();
	
	LCD_WriteText("Hello World");
	LCD_GoTo(3,1);
	LCD_WriteText("test");
	
	while(1)
	{
		
	}
	return 0;
}
	
	
	