#ifndef _GRAPHICS_H
#define _GRAPHICS_H

void GLCD_puttext(const char __memx *txt);
void GLCD_Circle(uint8_t cx, uint8_t cy ,uint8_t radius);
void GLCD_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

#endif
