#define F_PCU 16000000
#include <hd44780.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	
	// konfiguracja przycisku FLIP
	PORTE.DIRCLR = PIN5_bm;               // pin E5 jako wejœcie
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc;  // podci¹gniêcie do zasilania
	
	// Inicjalizacja wyœwietlacza
	LCD_Initalize();                     // inicjalizacja sterownika LCD
	LCD_WriteText("   HEJ");          // wyœwietlenie napisu
	LCD_GoTo(0,1);                          // przejœcie do drugiej linii
	LCD_WriteText("Licznik");       // wyœwietlenie napisu
	
	// zmienna s³u¿¹ca jako licznik
	uint8_t licznik = 0;
	
	while(1) {
		if(!(PORTE.IN & PIN5_bm)) {    // je¿eli przycisk FLIP jest wciœniêty
			licznik++;                    // zwiêksz licznik o 1
			LCD_Clear();                   // wyczyœæ wyœwietlacz
			LCD_WriteText("Licznik:");              // wyœwietlenie napisu
			LCD_GoTo(0,1);                         // przejœcie do drugiej linii
			LCD_DEC_bm(licznik);              // wyœwietlenie zmiennej liczbowej
			_delay_ms(100);               // czekanie 100ms
		}
	}
}