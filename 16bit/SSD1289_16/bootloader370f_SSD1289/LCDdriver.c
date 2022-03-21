/*
 液晶モジュールドライバ for PIC32MX370F512H

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

volatile unsigned short drawcount;
/**********************
*  Timer4 割り込み処理関数
***********************/
void __ISR(16, ipl4) T4Handler(void)
{
	drawcount++;
	IFS0bits.T4IF = 0;			// T2割り込みフラグクリア
}

void delay_us(unsigned int us);

void delay_ms(int t){
	while(t>0){
		delay_us(1000);
		t--;
	}
}

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

void LCD_WriteData(unsigned short data){
	LCD_set_dat(data);
}

void LCD_WriteReg(unsigned char index, unsigned short data){
	LCD_WriteIndex(index);
	LCD_WriteData(data);
}

void LCD_Init(){
	LCD_CS_HI;                  // Disable LCD
	LCD_RESET_LO;               // Hold in reset
	LCD_RS_LO;
	LCD_RD_HI;
	LCD_WR_HI;

	// Reset controller
	LCD_RESET_LO;
	delay_ms(2);
	LCD_RESET_HI;
	delay_ms(2);
	LCD_CS_LO; // Enable LCD

	LCD_WriteReg(0x00,0x0001);
	LCD_WriteReg(0x03,0xA8A4);
	LCD_WriteReg(0x0C,0x0000);
	LCD_WriteReg(0x0D,0x080C);
	LCD_WriteReg(0x0E,0x2B00);
	LCD_WriteReg(0x1E,0x00B7);

	LCD_WriteReg(0x01, 0x2B3F);
//	LCD_WriteReg(0x01, 0x693F);

	LCD_WriteReg(0x02,0x0600);
	LCD_WriteReg(0x10,0x0000);
	LCD_WriteReg(0x11,0x6070);
	LCD_WriteReg(0x05,0x0000);
	LCD_WriteReg(0x06,0x0000);
	LCD_WriteReg(0x16,0xEF1C);
	LCD_WriteReg(0x17,0x0003);
	LCD_WriteReg(0x07,0x0233);
	LCD_WriteReg(0x0B,0x0000);
	LCD_WriteReg(0x0F,0x0000);
	LCD_WriteReg(0x41,0x0000);
	LCD_WriteReg(0x42,0x0000);
	LCD_WriteReg(0x48,0x0000);
	LCD_WriteReg(0x49,0x013F);
	LCD_WriteReg(0x4A,0x0000);
	LCD_WriteReg(0x4B,0x0000);
	LCD_WriteReg(0x44,0xEF00);
	LCD_WriteReg(0x45,0x0000);
	LCD_WriteReg(0x46,0x013F);
	LCD_WriteReg(0x30,0x0707);
	LCD_WriteReg(0x31,0x0204);
	LCD_WriteReg(0x32,0x0204);
	LCD_WriteReg(0x33,0x0502);
	LCD_WriteReg(0x34,0x0507);
	LCD_WriteReg(0x35,0x0204);
	LCD_WriteReg(0x36,0x0204);
	LCD_WriteReg(0x37,0x0502);
	LCD_WriteReg(0x3A,0x0302);
	LCD_WriteReg(0x3B,0x0302);
	LCD_WriteReg(0x23,0x0000);
	LCD_WriteReg(0x24,0x0000);
	LCD_WriteReg(0x25,0x8000);
	LCD_WriteReg(0x4f,0x0000);
	LCD_WriteReg(0x4e,0x0000);
	LCD_WriteIndex(0x22);

	LCD_CS_HI;
	delay_ms(30);
	LCD_CS_LO;
}

void LCD_SetCursor(unsigned short x, unsigned short y){
#if LCD_ALIGNMENT == VERTICAL
	LCD_WriteReg(0x4E, x);
	LCD_WriteReg(0x4F, y);
	LCD_WriteIndex(0x22);
#elif LCD_ALIGNMENT == HORIZONTAL
	LCD_WriteReg(0x4E, );
	LCD_WriteReg(0x4F, x);
	LCD_WriteIndex(0x22);
#endif
}

void LCD_continuous_output(unsigned short x,unsigned short y,unsigned short color,int n){
	unsigned int i ;
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
