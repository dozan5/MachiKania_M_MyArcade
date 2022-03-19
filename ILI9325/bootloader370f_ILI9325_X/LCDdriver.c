/* 液晶モジュールドライバ for PIC32MX370F512H
 * ILI9325 2.4インチ液晶モジュール M024C9325SLD
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

volatile unsigned short drawcount;
/**********************
*  Timer2 割り込み処理関数
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

	LCD_WriteReg(0x01, 0x0100); //Driver output control (1)
	LCD_WriteReg(0x03, 0x1030); //Entry mode
	LCD_WriteReg(0x02, 0x0700); //LCD driving control
	LCD_WriteReg(0x04, 0x0000); //Resize control
	LCD_WriteReg(0x08, 0x0202); //Display control (2)
	LCD_WriteReg(0x09, 0x0000); //Display control (3)
	LCD_WriteReg(0x10, 0x0000);
	LCD_WriteReg(0x11, 0x0007); //power control 2 DC, VC
	LCD_WriteReg(0x12, 0x0000); //power control 3 VRH
	LCD_WriteReg(0x13, 0x0000); //power control 4 VCOM amplitude
	delay_ms(200);
	LCD_WriteReg(0x10, 0x16B0);             //power control 1 BT,AP
	LCD_WriteReg(0x11, 0x0227);             //power control 2 DC,VC
	delay_ms(50);
	LCD_WriteReg(0x12, 0x001c);             //power control 3 VRH
	delay_ms(50);
	LCD_WriteReg(0x13, 0x0A00);             //power control 4 vcom amplitude
	LCD_WriteReg(0x29, 0x000F);             //power control 7 VCOMH
	LCD_WriteReg(0x2b, 0x000d);             //Frame rate
	delay_ms(50);
	LCD_WriteReg(0x20, 0x0000);              //Horizontal GRAM Address Set
	LCD_WriteReg(0x21, 0x0000);              //Vertical GRAM Address Set
	LCD_WriteReg(0x50, 0x0000);              //Horizontal Address Start Position
	LCD_WriteReg(0x51, 0x00EF);              //Horizontal Address end Position (239)
	LCD_WriteReg(0x52, 0x0000);              //Vertical Address Start Position
	LCD_WriteReg(0x53, 0x013F);              //Vertical Address end Position (319)
	LCD_WriteReg(0x60, 0xa700);              //Driver Output Control 2
	//  LCD_WriteReg(0x61, 0x0001);              //Base Image Display Control
	LCD_WriteReg(0x61, 0x0003);              //Base Image Display Control
	LCD_WriteReg(0x6a, 0x0000);              //Base Image Display Control
	LCD_WriteReg(0x90, 0x0010);              //Panel Interface Control 1
	LCD_WriteReg(0x92, 0x0000);              //Panel Interface Control 2
	LCD_WriteReg(0x93, 0x0000);              //Panel Interface Control 3
	LCD_WriteReg(0x30, 0x0007);
	LCD_WriteReg(0x31, 0x0403);
	LCD_WriteReg(0x32, 0x0404);
	LCD_WriteReg(0x35, 0x0002);
	LCD_WriteReg(0x36, 0x0707);
	LCD_WriteReg(0x37, 0x0606);
	LCD_WriteReg(0x38, 0x0106);
	LCD_WriteReg(0x39, 0x0007);
	LCD_WriteReg(0x3c, 0x0700);
	LCD_WriteReg(0x3d, 0x0707);
	LCD_WriteReg(0x07, 0x0173);

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
