#define F_CPU 16000000
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include <hd44780.h>

int main(void)
{

	float pomiar;
	char str[8];
	float a=488.5/979;
	float b=511-a*1023;
	LCD_Initalize();

	ADMUX=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);  // wybór kana³u ADC0
	

	ADMUX |= (1 << REFS0);  // ustaw zewnetrzne napiecie
	ADMUX &= ~(0 << REFS1); // odniesienia na AVCC

	ADCSRA=(1<<ADEN)			 //ustawienie bity ADEN=1 - wlaczenie
	|(1<<ADPS0)			 // ustawienie preskalera na 128 // ustawienie
	|(1<<ADPS1)			 // czestotliwosc taktowania przetwornika A/C, f=16Mhz/128
	|(1<<ADPS2);

	while(1)
	{

		ADCSRA |= (1 << ADSC); // rozpocznij przetwarzanie
		while(bit_is_set(ADCSRA, ADSC))
		{};                 // czekaj na koniec przetwarzania
		
				
		pomiar = a*ADCW + b;  // bity na volty
		
		//pomiar 
		//pomiar 

		LCD_Clear();
		dtostrf(pomiar, 8, 1, str);
		LCD_WriteText("Temperatura");
		LCD_GoTo(3,1);
		LCD_WriteText(str);
		LCD_WriteText(" oC");

		_delay_ms(100);
	}

}