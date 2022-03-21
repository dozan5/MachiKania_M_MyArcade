#define VERTICAL 0
#define HORIZONTAL 1
//#define LCD_ALIGNMENT VERTICAL
#define LCD_ALIGNMENT HORIZONTAL

#if LCD_ALIGNMENT == VERTICAL
	#define LCD_X_RES 240 // 横方向解像度
	#define LCD_Y_RES 320 // 縦方向解像度
#else
	#define LCD_X_RES 320 // 横方向解像度
	#define LCD_Y_RES 240 // 縦方向解像度
#endif

#define LCD_DAT_MASK 0xFFFF
#define LCD_DAT LATB
#define LCD_DATSET LATBSET
#define LCD_DATCLR LATBCLR
#define LCD_WR_LO LATDCLR=0x0100
#define LCD_WR_HI LATDSET=0x0100
#define LCD_RS_LO LATDCLR=0x0200
#define LCD_RS_HI LATDSET=0x0200
#define LCD_CS_LO LATDCLR=0x0080
#define LCD_CS_HI LATDSET=0x0080
#define LCD_RESET_LO LATDCLR=0x0040
#define LCD_RESET_HI LATDSET=0x0040
#define LCD_RD_LO LATGCLR=0x0004
#define LCD_RD_HI LATGSET=0x0004

void LCD_WriteIndex(unsigned char index);
void LCD_WriteData(unsigned short data);
unsigned short LCD_ReadData(void);

void LCD_Init(void);
void LCD_SetCursor(unsigned short x, unsigned short y);
void LCD_Clear(unsigned short color);
void drawPixel(unsigned short x, unsigned short y, unsigned short color);

void LCD_set_Vertical(void);
void LCD_set_Horizontal(void);
