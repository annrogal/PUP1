#define F_CPU 16000000 //czestotliwosc taktowania uC
#include <avr/io.h>	//wykorzystane biblioteki
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <hd44780.h>

//#define timer_start 6
//volatile uint8_t cnt=0;

unsigned char hours = 0;
unsigned char minutes = 0;
unsigned char seconds = 0;
char time[] = "00:00:00";


//#define LCD_CNTRL_PIN PIND
//#define LCD_CNTRL_PORT PORTD
#define SET_HOUR	PB0
#define SET_MINUTE	PB1
//ISR(TIMER1_COMPA_vect);

int main(void) //petla glowna
{
	DDRB=0x00;
	PORTB=0x00;
	LCD_Initalize();
	LCD_WriteText("czas");
	LCD_GoTo(1,1);
	LCD_WriteText(time);
	
	
	
	//LCD_CNTRL_PORT = (1<<SET_HOUR | 1<<SET_MINUTE);
	
	
	OCR1A = 15625;

	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A

	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt on compare match

	TCCR1B |= (1 << CS12) | (1 << CS10);
	// set prescaler to 1024 and start the timer


	sei();
	// enable interrupts


	
	while(1)
	{
				
		if((1<<SET_HOUR))
		
		{
			hours++;
			if(hours > 23)
			hours = 0;
		}
		if((1<<SET_MINUTE))
		
		{
			minutes++;
			if(minutes > 59)
			minutes = 0;
		}
		
	}
	
}
	//############ Procedura obs�ugi przerwania od przepe�nienia timera ############

		ISR(TIMER1_COMPA_vect)
		{
			seconds++;
			
			if(seconds == 60)
			{
				seconds = 0;
				minutes++;
			}
			if(minutes == 60)
			{
				minutes = 0;
				hours++;
			}
			if(hours > 23)
			hours = 0;
			
			LCD_Update_Time();
				
		}

	


	//##############################################################################
	
