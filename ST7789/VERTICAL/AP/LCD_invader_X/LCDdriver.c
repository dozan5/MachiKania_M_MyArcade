/* 液晶モジュールドライバ for PIC32MX370F512H
   液晶モジュールの初期化はブートローダ側で行うため不要

 16bit mode接続
 D0   RB0
 D1   RB1
 D2   RB2
 D3   RB3
 D4   RB4
 D5   RB5
 D6   RB6
 D7   RB7
 D8   RB8
 D9   RB9
 D10  RB10
 D11  RB11
 D12  RB12
 D13  RB13
 D14  RB14
 D15  RB15
 RST  RD6
 CS   RD7
 WR   RD8
 RS   RD9
 RD   Vdd

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

void inline LCD_set_dat(unsigned short c){
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

void LCD_WriteData2(unsigned short data){
	LCD_set_dat(data>>8);
	LCD_set_dat(data);
}

void LCD_WriteData16(unsigned short data){
	LCD_set_dat(data);
}

unsigned short LCD_get_dat(void){
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
	TRISBCLR=LCD_DAT_MASK;
	return d;
}

void LCD_SetCursor(unsigned short x, unsigned short y){
    LCD_WriteIndex(0x2a);
    LCD_WriteData2(x);
    LCD_WriteIndex(0x2b);
    LCD_WriteData2(y);
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
	unsigned short d;
	LCD_SetCursor(x,y);
	LCD_WriteIndex(0x2e);
	LCD_ReadData(); //dummy read
	d=LCD_ReadData();
	return (d>>11)+(d&0x7e0)+((d&0x1f)<<11); //swap R and B
}