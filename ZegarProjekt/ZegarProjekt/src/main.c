#define F_CPU 16000000 //czestotliwosc taktowania uC

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <hd44780.h>
#include <math.h>

unsigned char hours = 0;
unsigned char minutes = 0;
unsigned char seconds = 0;
char time[] = "00:00:00";



//ISR(TIMER1_COMPA_vect);

#define SET_HOUR	(bit_is_clear(PINB,0))
#define SET_MINUTE	(bit_is_clear(PINB,1))


int main(void) //petla glowna
{
	LCD_Initalize();
	LCD_WriteText("czas");
	
	DDRB=0b11111100;
	PORTB=0b00000011;
void rtc_init(void)
{
	OCR1A = 15625;

	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A

	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt on compare match

	TCCR1B |= (1 << CS12) | (1 << CS10);
	// set prescaler to 1024 and start the timer

rtc_init();
	sei();
	// enable interrupts
}
LCD_Update_Time();	
while(1)
{
	
	if(SET_HOUR)
	
	{
		hours++;
		if(hours > 23)
		hours = 0;
		
	}
	if(SET_MINUTE)
	
	{
		minutes++;
		if(minutes > 59)
		minutes = 0;
		
	}
	
	
}

	
		
}
//############ Procedura obs³ugi przerwania od przepe³nienia timera ############

ISR(TIMER1_COMPA_vect)
{
 	
}




//##############################################################################


