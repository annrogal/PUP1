#define F_CPU 16000000
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <HD44780.h>
 

int main(void)
{
	LCD_Initalize();
	LCD_Clear();
	LCD_Home();
	
	
		
		LCD_WriteText("Hello World");
		LCD_GoTo(3,1);
		LCD_WriteText("Test");
		
	while(1)
	{	
	}
	return 0;
}