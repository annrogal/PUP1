// this code sets up timer1 for a 1s  @ 16Mhz Clock (mode 4)

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>


int main(void)
{
	
	DDRD=0x01; // Ustawienie bitu LED jako wyjœcia
	PORTD=0x01;
	
	OCR1A = 15625;

	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A

	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt on compare match

	TCCR1B |= (1 << CS12) | (1 << CS10);
	// set prescaler to 1024 and start the timer


	sei();
	// enable interrupts


	while (1)
	{
		// we have a working Timer
	}
}

ISR (TIMER1_COMPA_vect)
{
	PORTD ^= (1 << 0); // zmiana stanu diody Led
}