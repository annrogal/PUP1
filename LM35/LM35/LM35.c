/*
 * LM35.c
 *
 * Created: 2014-08-21 17:18:13
 *  Author: tmf
 */ 


#include <avr\io.h>
#include <stdio.h>
#include <avr\interrupt.h>
#include <util\atomic.h>
#include <string.h>
#include <util\delay.h>
#include "KS0108lib.h"
#include "graphics.h"

#define NOOFSAMPLES	128

volatile uint32_t ADCVal;

ISR(ADC_vect)
{
	static uint32_t ADCaccum;
	static uint8_t sampleno;

	ADCaccum+=ADC;
	sampleno++;
	if(sampleno==NOOFSAMPLES)
	{
		ADCVal=ADCaccum;
		ADCaccum=0;
		sampleno=0;
	}
}

void ADC_init()
{
	ADMUX=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);  // wybór kana³u ADC0
	

	ADMUX |= (1 << REFS0);  // ustaw zewnetrzne napiecie
	ADMUX &= ~(0 << REFS1); // odniesienia na AVCC

	ADCSRA=(1<<ADEN)			 //ustawienie bity ADEN=1 - wlaczenie
	|(1<<ADPS0)			 // ustawienie preskalera na 128 // ustawienie
	|(1<<ADPS1)			 // czestotliwosc taktowania przetwornika A/C, f=16Mhz/128
	|(1<<ADPS2);
}
uint16_t GetTemperature()
{
	uint32_t adc;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		adc=ADCVal;
	}

	return (adc*11000UL)/(1024UL*NOOFSAMPLES);
}

int main()
{
	char wynik[7];

	LCD_Initalize();
	ADC_init();
	LCD_Clear();
	LCD_GoTo(0,0);

	LCD_WriteText("Temperatura:");
	while(1)
	{
		sprintf(wynik, "%5d", GetTemperature());
		uint8_t len=strlen(wynik);
		memmove(&wynik[len-1], &wynik[len-2], 3);
		wynik[len-2]=',';
		LCD_GoTo(0, 10);
		LCD_WriteText(wynik);
		_delay_ms(500);
	}
}
