/* 液晶モジュールドライバ for PIC32MX370F512H
 *  HX8347-D 2.8インチ液晶モジュール  M028C8347D8
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

void LCD_WriteData8(unsigned char data){
// Write Data
	LCD_set_dat(data);
}

void LCD_WriteReg(unsigned char index, unsigned short data){
// Write Index
	LCD_WriteIndex(index);
// Write Data
	LCD_WriteData8(data);
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

	LCD_WriteReg(0xEA,0x00); //PTBA[15:8]
	LCD_WriteReg(0xEB,0x20); //PTBA[7:0]
	LCD_WriteReg(0xEC,0x0C); //STBA[15:8]
	LCD_WriteReg(0xED,0xC4); //STBA[7:0]
	LCD_WriteReg(0xE8,0x38); //OPON[7:0]
	LCD_WriteReg(0xE9,0x10); //OPON1[7:0]
	LCD_WriteReg(0xF1,0x01); //OTPS1B
	LCD_WriteReg(0xF2,0x10); //GEN

	//Gamma 2.2 Setting
	LCD_WriteReg(0x40,0x01); //
	LCD_WriteReg(0x41,0x00); //
	LCD_WriteReg(0x42,0x00); //
	LCD_WriteReg(0x43,0x10); //
	LCD_WriteReg(0x44,0x0E); //
	LCD_WriteReg(0x45,0x24); //
	LCD_WriteReg(0x46,0x04); //
	LCD_WriteReg(0x47,0x50); //
	LCD_WriteReg(0x48,0x02); //
	LCD_WriteReg(0x49,0x13); //
	LCD_WriteReg(0x4A,0x19); //
	LCD_WriteReg(0x4B,0x19); //
	LCD_WriteReg(0x4C,0x16); //
	LCD_WriteReg(0x50,0x1B); //
	LCD_WriteReg(0x51,0x31); //
	LCD_WriteReg(0x52,0x2F); //
	LCD_WriteReg(0x53,0x3F); //
	LCD_WriteReg(0x54,0x3F); //
	LCD_WriteReg(0x55,0x3E); //
	LCD_WriteReg(0x56,0x2F); //
	LCD_WriteReg(0x57,0x7B); //
	LCD_WriteReg(0x58,0x09); //
	LCD_WriteReg(0x59,0x06); //
	LCD_WriteReg(0x5A,0x06); //
	LCD_WriteReg(0x5B,0x0C); //
	LCD_WriteReg(0x5C,0x1D); //
	LCD_WriteReg(0x5D,0xCC); //

	//Power Voltage Setting
	LCD_WriteReg(0x1B,0x1B); //VRH=4.65V
	LCD_WriteReg(0x1A,0x01); //BT (VGH~15V,VGL~-10V,DDVDH~5V)
	LCD_WriteReg(0x24,0x2F); //VMH(VCOM High voltage ~3.2V)
	LCD_WriteReg(0x25,0x57); //VML(VCOM Low voltage -1.2V)

	//****VCOM offset**///
	LCD_WriteReg(0x23,0x88); //for Flicker adjust //can reload from OTP

	//Power on Setting
	LCD_WriteReg(0x18,0x34); //I/P_RADJ,N/P_RADJ, Normal mode 60Hz
	LCD_WriteReg(0x19,0x01); //OSC_EN='1', start Osc
	LCD_WriteReg(0x01,0x08); //DP_STB='0', out deep sleep
	LCD_WriteReg(0x1F,0x88);// GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
	delay_ms(5);
	LCD_WriteReg(0x1F,0x80);// GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
	delay_ms(5);
	LCD_WriteReg(0x1F,0x90);// GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
	delay_ms(5);
	LCD_WriteReg(0x1F,0xD0);// GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	delay_ms(5);

	//262k/65k color selection
	LCD_WriteReg(0x17,0x05); //default 0x06 262k color // 0x05 65k color

	//SET PANEL
	LCD_WriteReg(0x36,0x00); //SS_P, GS_P,REV_P,BGR_P

	//Display ON Setting
	LCD_WriteReg(0x28,0x38); //GON=1, DTE=1, D=1000
	delay_ms(40);
	LCD_WriteReg(0x28,0x3F); //GON=1, DTE=1, D=1100

    LCD_WriteIndex(0x16);  // Memory Access Control 
   	LCD_WriteData(0x18);
//   	LCD_WriteData(0xD8);

	//Set GRAM Area
	LCD_WriteReg(0x02,0x00);
	LCD_WriteReg(0x03,0x00); //Column Start
	LCD_WriteReg(0x04,0x00);
	LCD_WriteReg(0x05,0xEF); //Column End
	LCD_WriteReg(0x06,0x00);
	LCD_WriteReg(0x07,0x00); //Row Start
	LCD_WriteReg(0x08,0x01);
	LCD_WriteReg(0x09,0x3F); //Row End

	LCD_CS_HI;
	delay_ms(30);
	LCD_CS_LO;
}

void LCD_SetCursor(unsigned short x, unsigned short y){
#if LCD_ALIGNMENT == VERTICAL
	LCD_WriteReg(0x02, x >> 8);
	LCD_WriteReg(0x03, x & 0xFF); //Column Start
	LCD_WriteReg(0x06, y >> 8);
	LCD_WriteReg(0x07, y & 0xFF); //Row Start
#elif LCD_ALIGNMENT == HORIZONTAL
	LCD_WriteReg(0x02, y >> 8);
	LCD_WriteReg(0x03, y & 0xFF); //Column Start
	LCD_WriteReg(0x06, x >> 8);
	LCD_WriteReg(0x07, x & 0xFF); //Row Start
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
