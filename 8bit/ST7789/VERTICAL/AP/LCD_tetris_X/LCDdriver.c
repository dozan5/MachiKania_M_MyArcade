/* 液晶モジュールドライバ for PIC32MX370F512H
   液晶モジュールの初期化はブートローダ側で行うため不要

8bit mode接続
 D0  RB0
 D1  RB1
 D2  RB2
 D3  RB3
 D4  RB4
 D5  RB5
 D6  RB6
 D7  RB7
 RST  RD6
 CS   RD7
 WR   RD8
 RS   RD9
 RD   RG2

 SDカード接続ポート
 SPI DO RG8  (O)
 SPI DI RG7  (I)
 SPI CLKO RG6  (O)
 CS  RF3  (O)
 WE  RF1  0
 CD  RF0  0
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
	unsigned char d;
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
	d=d | LCD_get_dat()<<8;
	LCD_RD_HI;
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	TRISBCLR=LCD_DAT_MASK;
	return d;
}

void LCD_SetCursor(unsigned short x, unsigned short y){
    LCD_WriteIndex(0x2A);
    LCD_WriteData16(x);
    LCD_WriteIndex(0x2B);
    LCD_WriteData16(y);
    LCD_WriteIndex(0x2C);
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
	unsigned short d, temp;
	LCD_SetCursor(x,y);
	LCD_WriteIndex(0x2E);
	LCD_ReadData(); //dummy read
	temp=LCD_ReadData();
    d=(temp & 0xF800) | (temp & 0x00FC) << 3;
    d=d | (LCD_ReadData() & 0x00F8) >> 3;
	return d;
}
