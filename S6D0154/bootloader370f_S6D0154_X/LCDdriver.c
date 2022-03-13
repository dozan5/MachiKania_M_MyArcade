/* 液晶モジュールドライバ for PIC32MX370F512H
 * S6D0154 2.4インチ液晶モジュール UL024C0154D8
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

volatile unsigned short drawcount;

/**********************
*  Timer2 割り込み処理関数
***********************/
void __ISR(16, ipl4) T4Handler(void){
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

void LCD_WriteReg(unsigned char index, unsigned short data){
// Write Index
	LCD_WriteIndex(index);
// Write Data
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
	delay_ms(100);
	LCD_RESET_HI;
	delay_ms(2);
	LCD_CS_LO; // Enable LCD

    LCD_WriteReg(0x11,0x001A);
    LCD_WriteReg(0x12,0x3121);
    LCD_WriteReg(0x13,0x006C);
    LCD_WriteReg(0x14,0x4249);
    LCD_WriteReg(0x10,0x0800);
	delay_ms(10);
    LCD_WriteReg(0x11,0x011A);
	delay_ms(10);
    LCD_WriteReg(0x11,0x031A);
	delay_ms(10);
    LCD_WriteReg(0x11,0x071A);
	delay_ms(10);
    LCD_WriteReg(0x11,0x0F1A);
	delay_ms(20);
    LCD_WriteReg(0x11,0x0F3A);
	delay_ms(30);
    LCD_WriteReg(0x01, 0x0128); //Driver output control (1)
    LCD_WriteReg(0x03, 0x1030); //Entry mode
    LCD_WriteReg(0x02,0x0100);
    LCD_WriteReg(0x07,0x1012);
    LCD_WriteReg(0x08,0x0303);
    LCD_WriteReg(0x0B,0x1100);
    LCD_WriteReg(0x0C,0x0000);
    LCD_WriteReg(0x0F,0x1801);
    LCD_WriteReg(0x15,0x0020);
    LCD_WriteReg(0x50,0x0101);
    LCD_WriteReg(0x51,0x0603);
    LCD_WriteReg(0x52,0x0408);
    LCD_WriteReg(0x53,0x0000);
    LCD_WriteReg(0x54,0x0605);
    LCD_WriteReg(0x55,0x0406);
    LCD_WriteReg(0x56,0x0303);
    LCD_WriteReg(0x57,0x0303);
    LCD_WriteReg(0x58,0x0010);
    LCD_WriteReg(0x59,0x1000);
    LCD_WriteReg(0x07,0x0012);
	delay_ms(40);
    LCD_WriteReg(0x07,0x0013);
    LCD_WriteReg(0x07,0x0017);  //Display Control  DISPLAY ON
    LCD_WriteReg(0x37, 0x0000); //HorizontalStartAddress
    LCD_WriteReg(0x36, 0x00ef); //HorizontalEndAddress
    LCD_WriteReg(0x39, 0x0000); //VerticalStartAddress
    LCD_WriteReg(0x38, 0x013f); //VertocalEndAddress
    LCD_WriteReg(0x20, 0x000);  //GRAM Address Set
    LCD_WriteReg(0x21, 0x000);

	LCD_CS_HI;
	delay_ms(30);
	LCD_CS_LO;
}

void LCD_SetCursor(unsigned short x, unsigned short y){
#if LCD_ALIGNMENT == VERTICAL
	LCD_WriteReg(0x20, x);
	LCD_WriteReg(0x21, y);
#elif LCD_ALIGNMENT == HORIZONTAL
	LCD_WriteReg(0x20, y);
	LCD_WriteReg(0x21, x);
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
