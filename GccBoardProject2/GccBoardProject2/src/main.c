#define F_PCU 16000000
#include <hd44780.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	
	// konfiguracja przycisku FLIP
	PORTE.DIRCLR = PIN5_bm;               // pin E5 jako wej�cie
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc;  // podci�gni�cie do zasilania
	
	// Inicjalizacja wy�wietlacza
	LCD_Initalize();                     // inicjalizacja sterownika LCD
	LCD_WriteText("   HEJ");          // wy�wietlenie napisu
	LCD_GoTo(0,1);                          // przej�cie do drugiej linii
	LCD_WriteText("Licznik");       // wy�wietlenie napisu
	
	// zmienna s�u��ca jako licznik
	uint8_t licznik = 0;
	
	while(1) {
		if(!(PORTE.IN & PIN5_bm)) {    // je�eli przycisk FLIP jest wci�ni�ty
			licznik++;                    // zwi�ksz licznik o 1
			LCD_Clear();                   // wyczy�� wy�wietlacz
			LCD_WriteText("Licznik:");              // wy�wietlenie napisu
			LCD_GoTo(0,1);                         // przej�cie do drugiej linii
			LCD_DEC_bm(licznik);              // wy�wietlenie zmiennej liczbowej
			_delay_ms(100);               // czekanie 100ms
		}
	}
}