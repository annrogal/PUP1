#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define LED1 PD0
#define LED2 PD1
#define przycisk PC5

int main(void)
{
	DDRD|=(1<<LED1) | (1<<LED2);//Ustawienie pin�w steruj�cych diodami
								// jako wyj�cia
	PORTD |=(1<<LED1);//Ustawienie stanu wysokiego na wyj�ciu steruj�cym LED1 
	
	DDRC&=~(1<<przycisk);//Ustawienie pinu klawisza jako wej�cia
	PORTC|=(1<<przycisk); //w��czenie rezystora podci�gaj�cego tzw. Pull_up
	
	 while (1) //P�tla g��wna
	 {
		 if(PINC & (1<<przycisk))//je�li klawisz nie wci�niety
		 {
			 PORTD ^=(1<<LED1); //suma modulo 2 (XOR) stanu poprzedniego
			 //na porcie i pinu LED1 (zmiana stanu na przeciwny)
			 
			 PORTD ^=(1<<LED2); //suma modulo 2 (XOR) stanu poprzedniego na porcie
			 //pinu LED2 (zmiana stanu na przeciwny)
			 
			 _delay_ms(1000); //op�nienie 1 s
		 }
	
	 }
}
