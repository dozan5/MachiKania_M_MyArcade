/* 液晶モジュールドライバ for PIC32MX370F512H
 *  SSD1289 3.2インチ液晶モジュール LCD032-2P
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
// Write Index
	LCD_RS_LO;
	LCD_set_dat(index);
   	LCD_RS_HI;
}

void LCD_WriteData(unsigned short data){
// Write Data
	LCD_set_dat(data>>8);
	LCD_set_dat((unsigned char)data);
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

void LCD_WriteReg(unsigned char index, unsigned short data){
// Write Index
	LCD_WriteIndex(index);
// Write Data
	LCD_WriteData(data);
}

unsigned short LCD_ReadReg(unsigned char index){
// Write Index
	LCD_WriteIndex(index);
// Read Data
	return LCD_ReadData();
}

void LCD_SetCursor(unsigned short x, unsigned short y){
#if LCD_ALIGNMENT == VERTICAL
	LCD_WriteReg(0x4E, x);
	LCD_WriteReg(0x4F, y);
#elif LCD_ALIGNMENT == HORIZONTAL
	LCD_WriteReg(0x4E, y);
	LCD_WriteReg(0x4F, x);
#endif
	LCD_WriteIndex(0x22);
}

void LCD_continuous_output(unsigned short x,unsigned short y,unsigned short color,int n){
	//High speed continuous output
	int i;
	LCD_SetCursor(x,y);
	for (i=0; i < n ; i++){
    	LCD_WriteData(color);
	}
}

void LCD_Clear(unsigned short color){
	LCD_continuous_output(0,0,color,LCD_X_RES*LCD_Y_RES);
}

void drawPixel(unsigned short x, unsigned short y, unsigned short color){
	LCD_SetCursor(x,y);
	LCD_WriteData(color);
}

unsigned short getColor(unsigned short x, unsigned short y){
	unsigned short d;
	LCD_SetCursor(x,y);
	LCD_ReadData(); //dummy read
	d=LCD_ReadData();
	return (d>>11)+(d&0x7e0)+((d&0x1f)<<11); //swap R and B
}

void LCD_set_Vertical(void){
	LCD_WriteReg(0x01, 0x2B3F);
}

void LCD_set_Horizontal(void){
	LCD_WriteReg(0x01, 0x6B3F);
}
