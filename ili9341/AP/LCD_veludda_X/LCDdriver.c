/* 液晶モジュールドライバ for PIC32MX370F512H
 * ILI9341 3.2インチ液晶モジュール M032C9341B3
 * 液晶モジュールの初期化はブートローダ側で行うため不要
 *
 * 8bit mode接続
 * D8  RB0
 * D9  RB1
 * D10 RB2
 * D11 RB3
 * D12 RB4
 * D13 RB5
 * D14 RB6
 * D15 RB7
 * RD  RB8
 * WR  RB9
 * RS  RB10
 * CS  RB11
 * RESET RB12
 *
 * SDカード接続ポート
 * SPI DO RG8  (O)
 * SPI DI RG7  (I)
 * SPI CLKO RG6  (O)
 * CS  RF3  (O)
 * WE  RF1  0
 * CD  RF0  0
*/

#include <plib.h>
#include "LCDdriver.h"

void inline LCD_set_dat(unsigned char c){
	LCD_DATSET=c;
	LCD_DATCLR=~c & LCD_DAT_MASK;
	LCD_WR_LO;
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_WR_HI;
}

void LCD_WriteIndex(unsigned char index){
	LCD_RS_LO;
	LCD_set_dat(index);
    LCD_RS_HI;
}

void LCD_WriteData(unsigned char data){
	LCD_set_dat(data);
}

void LCD_WriteData16(unsigned short data){
	LCD_set_dat(data>>8);
	LCD_set_dat(data);
}

unsigned char LCD_get_dat(void){
	unsigned short d;
	d=PORTB & LCD_DAT_MASK;
	return d;
}

unsigned short LCD_ReadData(void){
// Read Data
	unsigned short d;
	TRISBSET=LCD_DAT_MASK;
	LCD_RD_LO;
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	d=LCD_get_dat();
	LCD_RD_HI;
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	LCD_RD_LO;
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	d=(d<<8)+LCD_get_dat();
	LCD_RD_HI;
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	TRISBCLR=LCD_DAT_MASK;
	return d;
}

unsigned char LCD_ReadData8(void){
// Read Data
	unsigned char d;
	TRISBSET = LCD_DAT_MASK;
	LCD_RD_LO;
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	d=LCD_get_dat();
	LCD_RD_HI;
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	TRISBCLR = LCD_DAT_MASK;
	return d;
}

void LCD_SetCursor(unsigned short x, unsigned short y){
#if LCD_ALIGNMENT == VERTICAL
    LCD_WriteIndex(0x2a);
	LCD_WriteData16(x);
	LCD_WriteIndex(0x2b);
	LCD_WriteData16(y);
#elif LCD_ALIGNMENT == HORIZONTAL
    LCD_WriteIndex(0x2a);
	LCD_WriteData16(y);
	LCD_WriteIndex(0x2b);
	LCD_WriteData16(x);
#endif
	LCD_WriteIndex(0x2c);
}

void LCD_continuous_output(unsigned short x,unsigned short y,unsigned short color,int n){
	unsigned int i ;
	LCD_SetCursor(x,y);
	for (i=0; i < n ; i++){
       	LCD_WriteData16(color);
    }
}

void LCD_Clear(unsigned short color){
	LCD_continuous_output(0,0,color,LCD_X_RES*LCD_Y_RES);
}

void drawPixel(unsigned short x, unsigned short y, unsigned short color){
	LCD_SetCursor(x,y);
	LCD_WriteData16(color);
}

unsigned short getColor(unsigned short x, unsigned short y){
	unsigned short r, g, b;
	LCD_SetCursor(x,y);
	LCD_WriteIndex(0x2e);

	LCD_ReadData8(); //dummy read
    r =	LCD_ReadData8();
    g =	LCD_ReadData8();
    b =	LCD_ReadData8();

	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

void LCD_set_Vertical(void){
	LCD_WriteIndex(0x36);  // Memory Access Control 
   	LCD_WriteData(0x48);   // VERTICAL 
}

void LCD_set_Horizontal(void){
	LCD_WriteIndex(0x36);  // Memory Access Control 
   	LCD_WriteData(0x18);   // HORIZONTAL
}
