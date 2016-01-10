#include "KS0108lib.h"
#include "font5x8.h"
#include "defines.h"

uint8_t GLCD_x;
uint8_t GLCD_y;
uint8_t color=1;

#define GLUE(a, b)     a##b

/* single-bit macros, used for control bits */
#define SET_(what, p, m) GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m) GLUE(what, p) &= ~(1 << (m))
#define GET_(/* PIN, */ p, m) GLUE(PIN, p) & (1 << (m))
#define SET(what, x) SET_(what, x)
#define CLR(what, x) CLR_(what, x)

/*makra operuj¹ce na bajtach*/
#define DATA(what, b)	GLUE(what, b)

#define DISPLAY_STATUS_BUSY	0x80

//W³¹cz wybrany kontroler poprzez zmianê linii CS
void GLCD_EnableController(uint8_t controller)
{
 if(controller==0)
  {
   CLR(PORT, KS0108_CS1); SET(PORT, KS0108_CS2);
  } else
  {
   CLR(PORT, KS0108_CS2); SET(PORT, KS0108_CS1);
  }
}

uint8_t GLCD_ReadStatus(uint8_t controller)
{
 uint8_t status;
 DATA(DDR, KS0108_DATA)=0x00;
 SET(PORT, KS0108_RW);
 CLR(PORT, KS0108_RS);
 GLCD_EnableController(controller);
 SET(PORT, KS0108_E);
 asm volatile ("nop");
 status=DATA(PIN, KS0108_DATA);
 CLR(PORT, KS0108_E);
 return status;
}

//Czekaj a¿ flaga BUSY zostanie skasowana
static inline void GLCD_WaitUntillBusy(uint8_t controller)
{
 while(GLCD_ReadStatus(controller) & DISPLAY_STATUS_BUSY);
}

void GLCD_WriteCommand(uint8_t cmd, uint8_t controller)
{
 GLCD_WaitUntillBusy(controller);
 DATA(DDR, KS0108_DATA)=0xFF;
 CLR(PORT, KS0108_RW);
 CLR(PORT, KS0108_RS);
 GLCD_EnableController(controller);
 DATA(PORT, KS0108_DATA)=cmd;
 SET(PORT, KS0108_E);
 asm volatile ("nop");
 CLR(PORT, KS0108_E);
}

unsigned char GLCD_ReadData()
{
 uint8_t data;
 uint8_t CS=GLCD_x/64;
 GLCD_WaitUntillBusy(CS);
 DATA(DDR, KS0108_DATA)=0x00;
 SET(PORT, KS0108_RW);
 SET(PORT, KS0108_RS);
 GLCD_EnableController(CS);
 SET(PORT, KS0108_E);
 asm volatile ("nop");
 data=DATA(PIN, KS0108_DATA);
 CLR(PORT, KS0108_E);
 GLCD_x++;
 return data;
}

void GLCD_WriteData(uint8_t byte)
{
 uint8_t CS=GLCD_x/64;
 GLCD_WaitUntillBusy(CS);
 DATA(DDR, KS0108_DATA)=0xFF;
 CLR(PORT, KS0108_RW);
 SET(PORT, KS0108_RS);
 DATA(PORT, KS0108_DATA)=byte;
 GLCD_EnableController(CS);
 SET(PORT, KS0108_E);
 asm volatile ("nop");
 CLR(PORT, KS0108_E);
 GLCD_x++;
}

void GLCD_init()
{
 SET(DDR, KS0108_E);
 SET(DDR, KS0108_RS);
 SET(DDR, KS0108_RW);
 SET(DDR, KS0108_CS1);
 SET(DDR, KS0108_CS2);
 SET(PORT, KS0108_CS1);
 SET(PORT, KS0108_CS2);

#ifdef KS0108_RESET
 SET(DDR, KS0108_RESET);
 SET(PORT, KS0108_RESET);
#endif

 for(uint8_t i=0; i<GFX_SCREEN_WIDTH/64; i++)
 {
  GLCD_WriteCommand((DISPLAY_ON_CMD | ON), i);
  GLCD_WriteCommand(DISPLAY_START_LINE | 0,i);
 }
}

void GLCD_goto(uint8_t x, uint8_t y)
{
 uint8_t i;
 GLCD_x = x;
 GLCD_y = y;

 for(i=0; i<GFX_SCREEN_WIDTH/64; i++)
  {
   GLCD_WriteCommand(DISPLAY_SET_Y, i);
   GLCD_WriteCommand(DISPLAY_SET_X | y, i);
  }
 GLCD_WriteCommand(DISPLAY_SET_Y | (x%64), (x/64));
 GLCD_WriteCommand(DISPLAY_SET_X | y, (x/64));
}

void GLCD_cls()
{
 uint8_t tc=255;
 if(color) tc=0;
 for(uint8_t j=0; j<GFX_SCREEN_HEIGHT/8; j++)
  {
   GLCD_goto(0,j);
   for(uint8_t i=0; i<GFX_SCREEN_WIDTH; i++) GLCD_WriteData(tc);
  }
 GLCD_x=0; GLCD_y=0;
}

#if SLOW_TEXT==0
void GLCD_putchar(char c)
{
 const char __flash *ptr=font5x8+(5*(c-32));
 for(uint8_t i=0; i<5; i++)
  {
   uint8_t px=*ptr++;
   if(color==0) px^=0xFF;
   GLCD_WriteData(px);
  }
 if(color==0) GLCD_WriteData(0xFF); else GLCD_WriteData(0x00);
}

#else
void GLCD_putchar(char c)
{
 uint8_t orig_y=GLCD_y;
 const char __flash *ptr=font5x8+(5*(c-32));
 uint8_t tmp_x=GLCD_x;
 uint8_t tmpcolor=color;

 for(uint8_t i=0; i<6; i++)
  {
   uint8_t tmp_y=orig_y;
   uint8_t tmp;
   if(i!=5) tmp=*ptr++; else tmp=0;
   for(uint8_t y=0;y<8;y++)
    {
	 if(tmp & 0x01) color=1; else color=0;
	 if(tmpcolor==0) color^=0x01;
	 GLCD_SetPixel(tmp_x, tmp_y);
	 tmp_y++;
	 tmp>>=1;
	}
   tmp_x++;
  }
  color=tmpcolor;
  GLCD_y=orig_y;
}
#endif

void GLCD_SetPixel(uint8_t x, uint8_t y)
{
 int8_t tmpy=y/8;
 uint8_t tmp;
 GLCD_goto(x, tmpy);
 tmp=GLCD_ReadData();
 tmp=GLCD_ReadData();
 GLCD_goto(x, tmpy);
 if(color) tmp|=_BV(y%8); else tmp&=~(_BV(y%8));
 GLCD_WriteData(tmp);
}





