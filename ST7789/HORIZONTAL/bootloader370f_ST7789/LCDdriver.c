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

void LCD_Init(){
	LCD_CS_HI;                  // Disable LCD
	LCD_RESET_LO;               // Hold in reset
	LCD_RS_LO;
	LCD_WR_HI;
	LCD_RD_HI;

	// Reset controller
	LCD_RESET_HI;
	delay_ms(120);
	LCD_RESET_LO;
	delay_ms(10);
	LCD_RESET_HI;
	delay_ms(120);

	LCD_CS_LO; // Enable LCD

    LCD_WriteIndex(0x11);		// Sleep OUT
    delay_ms(50);

    LCD_WriteIndex(0x20);		// Display Inversion OFF
	LCD_WriteData(0x00);

    LCD_WriteIndex(0x36);  // Memory Access Control 
//	LCD_WriteData(0x00);
	LCD_WriteData(0x60);

    LCD_WriteIndex(0x3A);		// Interface Pixel Format
   	LCD_WriteData(0x55);

    LCD_WriteIndex(0x2A);		// Column Addess Set
    LCD_WriteData2(0x0000);
    LCD_WriteData2(LCD_X_RES-1);

    LCD_WriteIndex(0x2B);		// Page Address Set
    LCD_WriteData2(0x0000);
    LCD_WriteData2(LCD_Y_RES-1);
	delay_ms(50);

    LCD_WriteIndex(0x29);		// Display ON
    LCD_WriteIndex(0x2C);		// Memory Write

	LCD_CS_HI;
	delay_ms(30);
	LCD_CS_LO;
}

void LCD_SetCursor(unsigned short x, unsigned short y)
{
    LCD_WriteIndex(0x2a);
    LCD_WriteData2(x);
    LCD_WriteIndex(0x2b);
    LCD_WriteData2(y);
	LCD_WriteIndex(0x2c);
}

void LCD_continuous_output(unsigned short x,unsigned short y,unsigned short color,int n)
{
	unsigned int i ;
	LCD_SetCursor(x,y);
	for (i=0; i < n ; i++){
       	LCD_WriteData2(color);
    }
}

void LCD_Clear(unsigned short color)
{
	LCD_continuous_output(0,0,color,LCD_X_RES*LCD_Y_RES);
}

void drawPixel(unsigned short x, unsigned short y, unsigned short color)
{
	LCD_SetCursor(x,y);
	LCD_WriteData2(color);
}
