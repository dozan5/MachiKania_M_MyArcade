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

volatile unsigned short delaytimerflag;

/**********************
*  Timer5 割り込み処理関数
***********************/
void __ISR(20, ipl4) T5Handler(){
	delaytimerflag=1;
	IFS0bits.T5IF=0;    // T5割り込みフラグクリア
}

void delay_us(int t){
	delaytimerflag=0;
	PR5=48*t;   // tマイクロ秒
	T5CONSET=0x8000;
	while(delaytimerflag==0) asm("wait");	// sleepで待機
	T5CONCLR=0x8000;
}

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

    LCD_WriteIndex(0x36);  // Memory Access Control 
	LCD_WriteData(0x00);
//	LCD_WriteData(0x60);

    LCD_WriteIndex(0x3A);		// Interface Pixel Format
   	LCD_WriteData(0x55);

    LCD_WriteIndex(0x2A);		// Column Addess Set
    LCD_WriteData16(0x0000);
    LCD_WriteData16(LCD_X_RES-1);

    LCD_WriteIndex(0x2B);		// Page Address Set
    LCD_WriteData16(0x0000);
    LCD_WriteData16(LCD_Y_RES-1);
	delay_ms(50);

    LCD_WriteIndex(0x29);		// Display ON
    LCD_WriteIndex(0x2C);		// Memory Write

	LCD_CS_HI;
	delay_ms(30);
	LCD_CS_LO;
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
