#define F_CPU 16000000 //czestotliwosc taktowania uC

#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <interrupt.h>
#include <avr/interrupt.h>
#include <hd44780.h>
#include <math.h>
#include <stdlib.h>


unsigned char hours = 0;    // unsigned char - ca³kowite od 0 do 255;
unsigned char minutes = 0;
unsigned char seconds = 0;
float pomiar;  
float pomiar_old;           // float - rzeczywisty, do 6 miejsc precyzji
float pomiar_sr;
char str[8];
char time[] = "00:00:00";  // char - ca³kowity od -128 do 127;
int czas_1;
int pomiar_takt=0;
volatile uint8_t z=0 ;   

#define LED_1 PORTD|=(1<<PD7);
#define LED_0 PORTD&=~(1<<PD7);
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
volatile uint8_t GRZALKA;


int main(void) //petla glowna
{
	
	DDRD=0xFF;
	PORTD=0x80;
		
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
	
	
	ADMUX=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);  // wybór kana³u ADC0
	ADMUX |= (1 << REFS0);  // ustaw zewnetrzne napiecie
	ADMUX &= ~(0 << REFS1); // odniesienia na AVCC

	ADCSRA=(1<<ADEN)			 //ustawienie bity ADEN=1 - wlaczenie
	|(1<<ADPS0)			 // ustawienie preskalera na 128 // ustawienie
	|(1<<ADPS1)			 // czestotliwosc taktowania przetwornika A/C, f=16Mhz/128
	|(1<<ADPS2);
	ADCSRA |= (1 << ADSC); // rozpocznij przetwarzanie
	
//-----------------------------------------------------------------------------------------------
// 	// /                  PWM Diody LED
TCCR0A |= (1<<WGM01) | (1<<WGM00); //tryb PWM
TCCR0A |= (1<<COM0A1)| (1<<COM0A0);
TCCR0A |= (1<<CS00)  | (1<<CS02); //preskaler 1024
OCR0A = 0;
OCR0A =255;
	
	float a=488.5/979;
	float b=511-a*1023;
	pomiar = a*ADC + b;
	pomiar_sr=pomiar;
	dtostrf(pomiar_sr, 8, 1, str);
	LCD_GoTo(0,1);
	LCD_WriteText(str);
	LCD_WriteText(" oC");
	 
	 
	      
	while(1)
	{
		if(!(PINB & (1<<SET_HOUR)))   // ! - negacja
		{
			hours++;                 // ++ - zwiêksza o 1
			if(hours > 23)
			hours = 0;
		}
		if(!(PINB & (1<<SET_MINUTE)))
		{
			minutes++;
			if(minutes > 59)
			minutes = 0;
		}
		_delay_ms(250);
		
		if(hours==7 & minutes==30)
		{
			SW1_1;
		}
		if(hours==19 & minutes==00)
		{
			SW1_0;
		}
		if(hours==8 & minutes==00)
		{
			SW2_1;
		}
		if(hours==19 & minutes==30)
		{
			SW2_0;
		}
		if(hours==8 & minutes==30)
		{
			SW3_1;
		}
		if(hours==20 & minutes==0)
		{
			SW3_0;			
		}
//-------------------------------------------------------------------------		
		//LED_1; // do sterowania ledami
	
//-------------------------------------------------------------------------
		if(pomiar_takt>=30)
		{
			
			pomiar_old=pomiar;			
			pomiar_sr=(pomiar+pomiar_old)/2;
			
					
			dtostrf(pomiar_sr, 8, 1, str);
			LCD_GoTo(0,1);
			LCD_WriteText(str);
			LCD_WriteText(" oC");
			
			pomiar_takt=0;
			
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

ISR(TIMER0_OVF_vect)
{
		 
		for(z = 0; z<255; z++)
		{
			OCR0A = z;
			
		}
		for(z = 255; z; z--)
		{
			OCR0A = z;
			
		}
}
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
	 
	pomiar_takt++;

	ADCSRA |= (1 << ADSC); // rozpocznij przetwarzanie
	float a=488.5/979;
	float b=511-a*1023;
	pomiar = a*ADC + b;
		
		
//----------------------------------------------------------------------------------------------------
//  obsluga grzalki 
	
	if(pomiar_sr<22.5 && GRZALKA==0)
	{
		czas_1++;
	}
	else if(pomiar_sr>22.5 && GRZALKA==1)
	{
		czas_1++;
	}
	else
	czas_1=0;
	
			if(czas_1>=30)
			{
				czas_1=0;
				if(GRZALKA==0)
				{
				GRZALKA_1;
				GRZALKA=1;
				}
				else
				{
				GRZALKA_0;
				GRZALKA=0;
				}
			}
}



//##############################################################################
