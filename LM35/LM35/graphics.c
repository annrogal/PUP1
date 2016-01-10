#include <stdlib.h>
#include "KS0108lib.h"
#include "graphics.h"

void GLCD_puttext(const char __memx *txt)
{
 while(*txt) GLCD_putchar(*txt++);
}

void GLCD_Circle(uint8_t cx, uint8_t cy ,uint8_t radius)
{
 int8_t x, y, xchange, ychange, radiusError;
 x=radius;
 y=0;
 xchange=1-2*radius;
 ychange=1;
 radiusError=0;
 while(x>=y)
  {
   GLCD_SetPixel(cx+x, cy+y); 
   GLCD_SetPixel(cx-x, cy+y); 
   GLCD_SetPixel(cx-x, cy-y);
   GLCD_SetPixel(cx+x, cy-y); 
   GLCD_SetPixel(cx+y, cy+x); 
   GLCD_SetPixel(cx-y, cy+x); 
   GLCD_SetPixel(cx-y, cy-x); 
   GLCD_SetPixel(cx+y, cy-x); 
   y++;
   radiusError+=ychange;
   ychange+=2;
   if(2*radiusError+xchange>0)
    {
     x--;
	 radiusError+=xchange;
	 xchange+=2;
	}
  }
}

//Kreœlenie linii algorytmem Bresenhama
void GLCD_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
 int8_t err, e2, sx, sy;
 int8_t dx=abs(x1-x0);
 int8_t dy=abs(y1-y0); 
 if(x0<x1) sx=1; else sx=-1;
 if(y0<y1) sy=1; else sy=-1;
 if(dx>dy) err=dx/2; else err=-dy/2;
 
 while(1)
  {
   GLCD_SetPixel(x0, y0);
   if((x0==x1) && (y0==y1)) return;
   e2=err;
   if(e2>-dx)
    { 
     err=err-dy;
     x0=x0+sx;
	} 
   if(e2<dy)
    { 
     err=err+dx;
     y0=y0+sy;
    }
  }
}

