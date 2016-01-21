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
float pomiar;			   // float - rzeczywisty, do 6 miejsce precyzji
float pomiar_old;
float pomiar_sr;
char str[8];			   // typ znakowy
char time[] = "00:00:00";
int czas_1;
int czas_2=0;			 // int - liczby ca³kowite
int pomiar_takt=0;
int timer0=0;
bool rozjasnianie=true;
volatile uint8_t d=255;
volatile uint8_t GRZALKA;

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
	
	DDRD=0xFF;
	PORTD=0x40;
	
	
	LCD_Initalize();
	LCD_GoTo(4,0);
	LCD_WriteText(time);
	
	//-----------------------------------TIMER1---------------------------------------------------------
	PINB = (1<<SET_HOUR | 1<<SET_MINUTE);
	TCCR1B |= (1 << WGM12);								//ustawienie timera w tryb CTC,
	//CTC-Clear On Timer Compare - automatyczne wyzerowanie po osi¹gniêciu po¿¹danej wartoœci
	OCR1A =625;								    //ustawienie wartoœci po¿¹danej na 1Hz 15625-1
	TIMSK1 |= (1 << OCIE1A);							//Zezwolenie na przerwania dla CTC
	TCCR1B |= (1 << CS12) | (1 << CS10);			    // ustawienie preskalera na 1024
	
	
	
	//-----------------------------------ADC-----------------------------------------------------------
	
	ADMUX=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);      //wybór kana³u ADC0
	ADMUX |= (1 << REFS0);                              //ustaw zewnetrzne napiecie
	ADMUX &= ~(0 << REFS1);                             //odniesienia na AVCC
	ADCSRA=(1<<ADEN)			                        //ustawienie bity ADEN=1 - wlaczenie
	|(1<<ADPS0)										    //ustawienie preskalera na 128
	// ustawienie
	|(1<<ADPS1)											//czestotliwosc taktowania przetwornika A/C, f=16Mhz/128
	|(1<<ADPS2);
	ADCSRA |= (1 << ADSC);								// rozpocznij przetwarzanie
	
	//----------------------------------------TIMER0-PWM Dioda------------------------------------------------------
	DDRD |= (1 << DDD6);
	OCR0A = 255;											//wartoœæ pocz¹tkowa PWM
	TCCR0A |= (1 << COM0A1)|(1<<COM0A0);					//none-inverting mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);					//ustawienie trybu PWM
	TCCR0B |= (1 << CS01);									//ustawienie preskalera na 8
	
	sei();													//Zezwolenie globalne na przerwania

	//----------------------------------------pierwszy pomiar temperatury------------------------------------------------------
	_delay_ms(1000);
	
	float a=488.5/979;
	float b=511-a*1023;
	pomiar = a*ADC + b;
	pomiar_sr=pomiar;
	dtostrf(pomiar_sr, 8, 1, str);
	LCD_GoTo(0,1);
	LCD_WriteText("temp");
	LCD_WriteText(str);
	LCD_WriteText(" C");
	//----------------------------------------pêtla-----------------------------------------------------
	while(1)
	{
		if(rozjasnianie)
		{			
			if(hours==7 && minutes<=30)
			{
				czas_2++;
				if(czas_2>=7 && OCR0A>=6)
				{
				OCR0A--;
				czas_2=0;
				}
				if(OCR0A<=7)
				{
					OCR0A=0;
				}
			}
			if(hours==20 && minutes<=30)
			{
				czas_2++;
				if(czas_2>=7 && OCR0A<=249)
				{
					OCR0A++;
					czas_2=0;
				}
				if(OCR0A>=250)
				{
					OCR0A=255;
				}
			}
			rozjasnianie=false;
		}
		//STEROWANIE PRZYCISKAMI
		if(!(PINB & (1<<SET_HOUR)))   // ! - negacja
		{
			hours++;                 // ++ - zwiêksza o 1
			if(hours > 23)
			hours = 0;
			_delay_ms(300);
		}
		if(!(PINB & (1<<SET_MINUTE)))
		{
			minutes++;
			if(minutes > 59)
			minutes = 0;
			_delay_ms(300);
		}
		
		//STEROWANIE ŒWIAT£EM
		if(hours==7 && minutes==30)
		{
			SW1_1;
		}
		if(hours==19 && minutes==0)
		{
			SW1_0;
		}
		if(hours==8 && minutes==0)
		{
			SW2_1;
		}
		if(hours==19 && minutes==30)
		{
			SW2_0;
		}
		if(hours==8 && minutes==30)
		{
			SW3_1;
		}
		if(hours==20 && minutes==0)
		{
			SW3_0;
		}
		//UŒREDNIENIE WYNIKÓW POMIARU TEMPERATURY
		if(pomiar_takt>=30)
		{
			pomiar_old=pomiar;
			pomiar_sr=(pomiar+pomiar_old)/2;
			
			dtostrf(pomiar_sr, 8, 1, str);
			LCD_GoTo(0,1);
			LCD_WriteText("temp");
			LCD_WriteText(str);
			LCD_WriteText(" C");
			
			pomiar_takt=0;
		}
		
	}
}

void LCD_update_time()
{
	unsigned char temp[8];
	
	LCD_GoTo(4,0);
	
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

//-----------------------------Procedura obs³ugi przerwania od przepe³nienia timera-----------------------------------------
ISR(TIMER1_COMPA_vect)
{
	//POMIAR CZASU
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
	
	//POMIAR TEMPERATURY - ODLICZANIE
	pomiar_takt++;
	ADCSRA |= (1 << ADSC); // rozpocznij przetwarzanie
	float a=488.5/979;
	float b=511-a*1023;
	pomiar = a*ADC + b;
	
	//OBS£UGA GRZA£KI
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
	//STEROWANIE JASNOŒCI¥ LED
	rozjasnianie=true;
}



