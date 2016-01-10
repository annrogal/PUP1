#ifndef _KS0108_H
#define _KS0108_H

#include <avr/pgmspace.h>

#define GFX_SCREEN_WIDTH	128
#define GFX_SCREEN_HEIGHT	64


#define DISPLAY_SET_Y       0x40
#define DISPLAY_SET_X       0xB8
#define DISPLAY_START_LINE  0xC0
#define DISPLAY_ON_CMD		0x3E
#define ON	0x01
#define OFF	0x00
#define DISPLAY_STATUS_BUSY	0x80


extern uint8_t GLCD_x;
extern uint8_t GLCD_y;
extern uint8_t color;

//Wewnêtrzne funkcje biblioteki
void GLCD_WriteData(uint8_t byte);
void GLCD_WriteCommand(uint8_t cmd, uint8_t controller);

//Funkcje zewnêtrzne
void GLCD_init();
void GLCD_cls();
void GLCD_goto(uint8_t x, uint8_t y);
uint8_t GLCD_ReadData();
void GLCD_SetPixel(uint8_t x, uint8_t y);
void GLCD_putchar(char c);

#endif
