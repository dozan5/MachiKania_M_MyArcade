#include <plib.h>
#define Fosc 60
#define RGB(r,g,b) ((((b>>3)<<11) | ((g>>2)<<5) | (r>>3)))

#define ENDCOL     320	// X
#define ENDROW     480	// Y
#define XChar		  (short)((ENDCOL) / 12)
#define YLine		  (short)((ENDROW) / 14)

/*******************************************
*  16bit Color Difinition
*******************************************/
#define WHITE	RGB(255,255,255)
#define BLACK	RGB(0,0,0)
#define RED     RGB(255,0,0)
#define GREEN	RGB(0,255,0)
#define BLUE	RGB(0,0,255)
#define BLUE2	RGB(0,0,64)
#define CYAN	RGB(0,255,255)
#define MAGENTA	RGB(255,0,255)
#define YELLOW	RGB(255,255,0)
#define YELLOW2	RGB(90,90,0)
#define BROWN	RGB(64,64,64)
#define BROWN2	RGB(32,32,32)
#define ORANGE	RGB(128,32,0)
#define ORANGE2	RGB(64,8,0)
#define PINK	RGB(255,128,128)
#define PINK2	RGB(128,64,64)

unsigned short keystatus,keystatus2,oldkey; //最新のボタン状態と前回のボタン状態

//void lcd_cmd(unsigned short cmnd);
//void lcd_data(unsigned short data);
//void lcd_out(unsigned short index, unsigned short data);
//void lcd_Init(void);
void lcd_vClear(unsigned char Color);
void lcd_vChar(char colum, char line, unsigned char letter, unsigned char Color1,unsigned char Color2);
void vcip(short cx,short cy, unsigned char pat);
void vcips(short cx,short cy, unsigned char pat);
void vmap(void);
void vmaps(void);
void mramset(unsigned char stg);
void lcd_vramset2(short cx,short cy,unsigned char dat1,unsigned char dat2);
void lcd_vramset(short cx,short cy, unsigned char dat1,unsigned char dat2);
void lcd_disp(void);
void lcd_vStr(char line, char colum, char *s, unsigned char Color1, unsigned char Color2);
void move(void);
void scr(void);
void opening(void);
void stage(void);
void stdemo(void);
void stsel(void);
void escset(void);
void mcls(void);
void itostring(char digit, unsigned int data, char *buffer);
void sund(unsigned short hz);
void sund2(unsigned short hz);

void psave(unsigned char n);
void pload(unsigned char n);
//void delay_us(unsigned short time);
//void delay_ms(unsigned short time);
