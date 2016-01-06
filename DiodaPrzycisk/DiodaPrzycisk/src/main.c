#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define LED1 PD0
#define LED2 PD1
#define przycisk PC5

int main(void)
{
	DDRD|=(1<<LED1) | (1<<LED2);//Ustawienie pinów steruj¹cych diodami
								// jako wyjœcia
	PORTD |=(1<<LED1);//Ustawienie stanu wysokiego na wyjœciu steruj¹cym LED1 
	
	DDRC&=~(1<<przycisk);//Ustawienie pinu klawisza jako wejœcia
	PORTC|=(1<<przycisk); //w³¹czenie rezystora podci¹gaj¹cego tzw. Pull_up
	
	 while (1) //Pêtla g³ówna
	 {
		 if(PINC & (1<<przycisk))//jeœli klawisz nie wciœniety
		 {
			 PORTD ^=(1<<LED1); //suma modulo 2 (XOR) stanu poprzedniego
			 //na porcie i pinu LED1 (zmiana stanu na przeciwny)
			 
			 PORTD ^=(1<<LED2); //suma modulo 2 (XOR) stanu poprzedniego na porcie
			 //pinu LED2 (zmiana stanu na przeciwny)
			 
			 _delay_ms(1000); //opóŸnienie 1 s
		 }
	
	 }
}
