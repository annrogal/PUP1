#define F_CPU 16000000 //czestotliwosc taktowania uC

#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <hd44780.h>
#include <math.h>
#include <stdlib.h>


unsigned char hours = 0;
unsigned char minutes = 0;
unsigned char seconds = 0;
unsigned long obieg;
float pomiar;
char str[8];
char time[] = "00:00:00";

ISR(TIMER1_COMPA_vect);

#define SET_HOUR	PB0
#define SET_MINUTE	PB1
#define  SW1_1 PORTD|=(1<<PD0);
#define SW1_0 PORTD&=~(1<<PD0);
#define  SW2_1 PORTD|=(1<<PD1);
#define SW2_0 PORTD&=~(1<<PD1);
#define  SW3_1 PORTD|=(1<<PD2);
#define SW3_0 PORTD&=~(1<<PD2);
#define  GRZALKA_1 PORTD|=(1<<PD3);
#define  GRZALKA_0 PORTD&=~(1<<PD3);


int main(void) //petla glowna
{
	
	DDRD=0x7F;
	PORTD=0x70;
		
	LCD_Initalize();
	LCD_WriteText("czas");
	LCD_GoTo(6,0);
	LCD_WriteText(time);
	//LCD_GoTo(0,1);
	//LCD_WriteText("Temperatura");
	
	 //--------------------------------------------------------------------------------------------	
	PINB = (1<<SET_HOUR | 1<<SET_MINUTE);
	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A
	OCR1A = 15625-1;
	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt on compare match
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// set prescaler to 1024 and start the timer
	sei();
	// enable interrupts
	while(1)
	{
		if(!(PINB & (1<<SET_HOUR)))
		{
			hours++;
			if(hours > 23)
			hours = 0;
		}
		if(!(PINB & (1<<SET_MINUTE)))
		{
			minutes++;
			if(minutes > 59)
			minutes = 0;
		}
		//_delay_ms(250);
		
		if(time == "07:30:00")
		{
			SW1_1;
		}
		if(time == "19:00:00")
		{
			SW1_0;
		}
		if(time == "08:00:00")
		{
			SW2_1;
		}
		if(time == "19:30:00")
		{
			SW2_0;
		}
		if(time == "08:30:00")
		{
			SW3_1;
		}
		if(time == "20:00:00")
		{
			SW3_0;			
		}
	}

}
void LCD_update_time()
{
	unsigned char temp[8];
	
	LCD_GoTo(6,0);
	
	itoa(hours/10,temp,10);
	LCD_WriteText(temp);
	itoa(hours%10,temp,10);
	LCD_WriteText(temp);
	LCD_WriteText(":");
	
	itoa(minutes/10,temp,10);
	LCD_WriteText(temp);
	itoa((minutes%10),temp,10);
	LCD_WriteText(temp);
	LCD_WriteText(":");
	
	itoa(seconds/10,temp,10);
	LCD_WriteText(temp);
	itoa(seconds%10,temp,10);
	LCD_WriteText(temp);
}
//############ Procedura obs³ugi przerwania od przepe³nienia timera ############

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
 	
 	LCD_update_time();
	
		
		ADMUX=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);  // wybór kana³u ADC0
		ADMUX |= (1 << REFS0);  // ustaw zewnetrzne napiecie
		ADMUX &= ~(0 << REFS1); // odniesienia na AVCC

		ADCSRA=(1<<ADEN)			 //ustawienie bity ADEN=1 - wlaczenie
		|(1<<ADPS0)			 // ustawienie preskalera na 128 // ustawienie
		|(1<<ADPS1)			 // czestotliwosc taktowania przetwornika A/C, f=16Mhz/128
		|(1<<ADPS2);
		ADCSRA |= (1 << ADSC); // rozpocznij przetwarzanie
		
		
		float a=488.5/979;
		float b=511-a*1023;
		
		pomiar = a*ADC + b;  // bity na volty

		//pomiar
		//pomiar
		dtostrf(pomiar, 8, 1, str);
		LCD_GoTo(0,1);
		LCD_WriteText(str);
		LCD_WriteText(" oC");

		if(pomiar<17.0)
		{
			GRZALKA_1;
		}
		else
		{
			GRZALKA_0;
		}
}




//##############################################################################
