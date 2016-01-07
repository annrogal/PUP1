#define F_CPU	4000000UL
#include <avr/delay.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>

/*Global Variables Declarations*/
unsigned char hours = 0;
unsigned char minutes = 0;
unsigned char seconds = 0;
char time[] = "00:00:00";

/*LCD function declarations */
void LCD_send_command(unsigned char cmnd);
void LCD_send_data(unsigned char data);
void LCD_init();
void LCD_goto(unsigned char y, unsigned char x);
void LCD_print(char *string);
void LCD_update_time(void);

/*Timer Counter 1 Compare Match A Interrupt Service Routine/Interrupt Handler*/
ISR(TIMER1_COMPA_vect);

#define LCD_DATA_PORT	PORTC
#define LCD_DATA_DDR	DDRC
#define LCD_DATA_PIN	PINC

#define LCD_CNTRL_PORT	PORTD
#define LCD_CNTRL_DDR	DDRD
#define LCD_CNTRL_PIN	PIND

#define LCD_RS_PIN		PD4
#define LCD_ENABLE_PIN	PD5
#define SET_HOUR		PB0
#define SET_MINUTE		PB1

int main(void)
{
	unsigned char i;
	
	LCD_init();
	LCD_print("czas");
	LCD_goto(3,1);
	LCD_print(time);
	
	LCD_CNTRL_PORT = (1<<SET_HOUR | 1<<SET_MINUTE);
	
	TCCR1B = (1<<CS12|1<<WGM12);
	OCR1A = 15625;
	TIMSK0 = 1<<OCIE1A;
	sei();
	
	while(1)
	{
		if(!(LCD_CNTRL_PIN & (1<<SET_HOUR)))
		{
			hours++;
			if(hours > 23)
			hours = 0;
		}
		if(!(LCD_CNTRL_PIN & (1<<SET_MINUTE)))
		{
			minutes++;
			if(minutes > 59)
			minutes = 0;
		}
		
	}
}

/* This function sends a command 'cmnd' to the LCD module*/
void LCD_send_command(unsigned char cmnd)
{
	LCD_DATA_PORT = cmnd;
	
	LCD_CNTRL_PORT &= ~(1<<LCD_RS_PIN);
	
	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
	_delay_us(100);
}

/* This function sends the data 'data' to the LCD module*/
void LCD_send_data(unsigned char data)
{
	LCD_DATA_PORT = data;
	
	LCD_CNTRL_PORT |= (1<<LCD_RS_PIN);
	
	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
	_delay_us(100);
}

void LCD_init()
{
	LCD_CNTRL_DDR = 0x30;
	LCD_CNTRL_PORT = 0xCF;
	LCD_DATA_DDR = 0x1E;
	LCD_DATA_PORT = 0xE1;
	
	_delay_ms(10);
	LCD_send_command(0x38);
	LCD_send_command(0x0C);
	LCD_send_command(0x01);
	_delay_ms(10);
	LCD_send_command(0x06);
}

/* This function moves the cursor the line y column x on the LCD module*/
void LCD_goto(unsigned char y, unsigned char x)
{
	unsigned char firstAddress[] = {0x80,0xC0,0x94,0xD4};
	
	LCD_send_command(firstAddress[y-1] + x-1);
	_delay_ms(10);
}

void LCD_print(char *string)
{
	unsigned char i;
	
	while(string[i]!=0)
	{
		LCD_send_data(string[i]);
		i++;
	}
}
void LCD_update_time()
{
	unsigned char temp;
	
	LCD_goto(2,4);
	
	itoa(hours/10,temp,10);
	LCD_print(temp);
	itoa(hours%10,temp,10);
	LCD_print(temp);
	LCD_print(":");
	
	itoa(minutes/10,temp,10);
	LCD_print(temp);
	itoa((minutes%10),temp,10);
	LCD_print(temp);
	LCD_print(":");
	
	itoa(seconds/10,temp,10);
	LCD_print(temp);
	itoa(seconds%10,temp,10);
	LCD_print(temp);
}
/*Timer Counter 1 Compare Match A Interrupt Service Routine/Interrupt Handler*/
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
}