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
	delay_ms(2);
	LCD_RESET_HI;
	delay_ms(2);
	LCD_CS_LO; // Enable LCD

	LCD_WriteIndex(0xCB);
	LCD_WriteData(0x39);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x00);
	LCD_WriteData(0x34);
	LCD_WriteData(0x02);

	LCD_WriteIndex(0xCF);
	LCD_WriteData(0x00);
	LCD_WriteData(0XC1);
	LCD_WriteData(0X30);

	LCD_WriteIndex(0xE8);
	LCD_WriteData(0x85);
	LCD_WriteData(0x00);
	LCD_WriteData(0x78);

	LCD_WriteIndex(0xEA);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	LCD_WriteIndex(0xED);
	LCD_WriteData(0x64);
	LCD_WriteData(0x03);
	LCD_WriteData(0X12);
	LCD_WriteData(0X81);

	LCD_WriteIndex(0xF7);
	LCD_WriteData(0x20);
    
	LCD_WriteIndex(0xC0);  //Power control 
	LCD_WriteData(0x23);   //VRH[5:0] 

	LCD_WriteIndex(0xC1);  //Power control 
	LCD_WriteData(0x10);   //SAP[2:0];BT[3:0] 

	LCD_WriteIndex(0xC5);  //VCM control 
	LCD_WriteData(0x3e);
	LCD_WriteData(0x28);

	LCD_WriteIndex(0xC7);  //VCM control2 
	LCD_WriteData(0x86);

	LCD_WriteIndex(0x36);  // Memory Access Control 
	LCD_WriteData(0x48);
//	LCD_WriteData(0x88);

	LCD_WriteIndex(0x37);
	LCD_WriteData2(0x0000);

	LCD_WriteIndex(0x3A);
	LCD_WriteData(0x55);

	LCD_WriteIndex(0xB1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x18);

	LCD_WriteIndex(0xB6);  // Display Function Control 
	LCD_WriteData(0x08);
	LCD_WriteData(0x82);
	LCD_WriteData(0x27);

	LCD_WriteIndex(0xF2);  // 3Gamma Function Disable 
	LCD_WriteData(0x00);

	LCD_WriteIndex(0x26);  //Gamma curve selected 
	LCD_WriteData(0x01);

	LCD_WriteIndex(0xE0);  //Set Gamma 
	LCD_WriteData(0x0F);
	LCD_WriteData(0x31);
	LCD_WriteData(0x2B);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x08);
	LCD_WriteData(0x4E);
	LCD_WriteData(0xF1);
	LCD_WriteData(0x37);
	LCD_WriteData(0x07);
	LCD_WriteData(0x10);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x09);
	LCD_WriteData(0x00);

	LCD_WriteIndex(0XE1);  //Set Gamma 
	LCD_WriteData(0x00);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x14);
	LCD_WriteData(0x03);
	LCD_WriteData(0x11);
	LCD_WriteData(0x07);
	LCD_WriteData(0x31);
	LCD_WriteData(0xC1);
	LCD_WriteData(0x48);
	LCD_WriteData(0x08);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x31);
	LCD_WriteData(0x36);
	LCD_WriteData(0x0F);

	LCD_WriteIndex(0x11);  //Exit Sleep 
	delay_ms(10);

	LCD_WriteIndex(0x29);  //Display on 
	LCD_WriteIndex(0x2c);

	LCD_CS_HI;
	delay_ms(30);
	LCD_CS_LO;
}

void LCD_SetCursor(unsigned short x, unsigned short y){
#if LCD_ALIGNMENT == VERTICAL
    LCD_WriteIndex(0x2a);
	LCD_WriteData2(x);
	LCD_WriteIndex(0x2b);
	LCD_WriteData2(y);
	LCD_WriteIndex(0x2c);
#elif LCD_ALIGNMENT == HORIZONTAL
    LCD_WriteIndex(0x2a);
	LCD_WriteData2(y);
	LCD_WriteIndex(0x2b);
	LCD_WriteData2(x);
	LCD_WriteIndex(0x2c);
#endif
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
