// TETRIS ブートローダ付き液晶ゲームシステム用
//をPIC32MX370F512H用に改編

#include <plib.h>
#include <stdlib.h>
#include "LCDdriver.h"
#include "tetris.h"

#define BOOTLOADER //ブートローダー用APにする場合有効にする

//ブートローダー使用時は共通ハイスコア書き込み領域を利用
//未使用時はフラッシュ最後のページにハイスコア記録
#ifndef BOOTLOADER
//ハイスコア書き込みページ（4096バイト単位で消去される）
#define FLASHPAGE (0x9d080000-0x1000)
#endif

#ifdef BOOTLOADER
#include "recscore.h"
#endif

//内蔵FRC 8MHz 1/2 * PLL 24 = 96MHz
// DEVCFG3
// USERID = No Setting
#pragma config FSRSSEL = PRIORITY_7     // Shadow Register Set Priority Select (SRS Priority 7)
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
//#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is Disabled)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// 入力ボタンのポート、ビット定義
#define KEYPORT PORTD
#define KEYDOWN 0x0001  // RD0
#define KEYLEFT 0x0002  // RD1
#define KEYUP 0x0004    // RD2
#define KEYRIGHT 0x0008 // RD3
#define KEYSTART 0x0010 // RD4
#define KEYFIRE 0x0020  // RD5

#define clearscreen() LCD_Clear(0)

unsigned char cursorx,cursory,cursorc;
unsigned char board[25][12]; //ブロックを配置する配列
unsigned char boardchange[25][12]; //board配列が変化したかを表す配列
unsigned int score,highscore; //得点、ハイスコア
unsigned int gcount=0; //カウンタ、乱数の種に使用
unsigned short keyold; //前回キー入力状態（リピート入力防止用）
char downkeyrepeat; //下キーのリピート制御
unsigned char next; //次のブロックの種類
unsigned char fallspeed,fallcount; //ブロックの落下速度、カウンタ
unsigned char level; //現在のレベル
unsigned char gamestatus;
// gamestatus
// 0:ゲームスタート
// 1:新ブロック出現
// 2:ブロック落下中
// 3:ステージクリア
// 4:ゲームオーバー

unsigned char lines;//消去したライン累積数
const unsigned int scorearray[]={40,100,300,1200}; //同時消去したライン数による得点

//ブロックの形状、色、向きの初期値定義
const _Block block[7]={
	{1, 0,-1, 0,-2, 0, COLOR_IBLOCK, 1},//I
	{1, 0,-1, 0,-1,-1, COLOR_JBLOCK, 3},//J
	{1, 0, 1,-1,-1, 0, COLOR_LBLOCK, 3},//L
	{1, 0, 0,-1,-1,-1, COLOR_ZBLOCK, 1},//Z
	{0,-1, 1,-1,-1, 0, COLOR_SBLOCK, 1},//S
	{0,-1,-1, 0,-1,-1, COLOR_OBLOCK, 0},//O
	{1, 0, 0,-1,-1, 0, COLOR_TBLOCK, 3} //T
};

_Block falling; //現在落下中のブロックの構造体
unsigned char blockx,blocky,blockangle,blockno; //現在落下中のブロックの座標、向き、種類

_Music music; //演奏中の音楽構造体
const unsigned short *sounddatap; //ブロック着地効果音配列の位置、演奏中の音楽よりこちらを優先

//sounddata配列　ド～上のド～その上のドの周期カウンタ値、PR3に書き込むと音程設定される
const unsigned short sounddata[]={27363,25828,24378,23010,21718,20499,19349,18263,17238,16270,15357,14495,
						13681,12914,12189,11505,10859,10249,9674,9131,8619,8135,7678,7247,6840};

//musicdata配列　音階,長さ,音階,長さ,・・・・・　最後に音階254で曲終了、253で最初へリピート
//				音階　0:ド～12:上のド～24:その上のド　　255:休符
//				長さ　60分のn秒

//コロブチカ
const unsigned char musicdata1[]={
	9,20,4,10,5,10,7,20,5,10,4,10,2,30,5,10,9,20,7,10,5,10,
	4,30,5,10,7,20,9,20,5,20,2,18,255,2,2,30,255,10,
	10,30,12,10,14,20,12,10,10,10,9,30,5,10,9,20,7,10,5,10,
	4,30,5,10,7,20,9,20,5,20,2,18,255,2,2,30,255,10,
	9,30,8,10,9,30,8,10,9,20,14,20,9,20,7,10,5,10,
	4,30,5,10,7,20,9,20,5,20,2,18,255,2,2,30,255,10,
	253
};

//カチューシャ
const unsigned char musicdata2[]={
	9,30,11,10,12,30,9,10,12,9,255,1,12,10,11,10,9,10,11,20,4,20,
	11,30,12,10,14,30,11,10,14,9,255,1,14,10,12,10,11,10,9,30,255,10,
	16,20,21,20,19,20,21,10,19,10,17,20,16,10,14,10,16,20,9,20,
	255,10,17,20,14,10,16,30,12,10,11,10,4,10,12,10,11,10,9,30,255,10,
	253
};

//トロイカ
const unsigned char musicdata3[]={
	9,10,
	14,29,255,1,14,9,255,1,14,9,255,1,14,10,13,10,14,10,16,30,13,10,9,20,255,10,9,10,
	17,20,14,20,5,20,7,9,255,1,7,10,9,60,255,10,9,10,
	14,30,16,10,17,10,16,10,14,10,9,10,7,30,10,10,14,20,16,10,14,10,
	9,30,10,10,9,10,7,10,4,10,5,10,2,60,255,10,
	253
};

//一週間
const unsigned char musicdata4[]={
	9,10,12,10,
	16,9,255,1,16,9,255,1,16,10,14,10,16,9,255,1,16,9,255,1,16,10,14,10,16,10,14,10,12,30,255,10,16,10,14,10,
	16,20,14,10,12,10,14,20,12,10,11,10,12,10,11,10,9,30,255,10,9,10,12,10,
	16,9,255,1,16,9,255,1,16,10,14,10,16,9,255,1,16,9,255,1,16,10,14,10,16,10,14,10,12,30,255,10,16,10,14,10,
	16,9,255,1,16,10,14,10,12,10,14,20,16,20,21,50,255,10,
	253
};

//カリンカ
const unsigned char musicdata5[]={
	9,20,
	7,20,4,10,5,10,7,20,4,10,5,10,7,20,5,10,4,10,2,20,9,9,255,1,9,10,
	7,20,4,10,5,10,7,20,4,10,5,10,7,20,5,10,4,10,2,20,9,9,255,1,9,10,
	7,20,4,10,5,10,7,20,4,10,5,10,7,20,5,10,4,10,2,20,9,9,255,1,9,10,
	7,20,4,10,5,10,7,20,4,10,5,10,7,20,5,10,4,10,2,20,9,9,255,1,9,10,
	7,20,4,10,5,10,7,20,4,10,5,10,7,20,5,10,4,10,2,20,9,9,255,1,9,10,
	7,20,4,10,5,10,7,20,4,10,5,10,7,20,5,10,4,10,2,20,14,60,255,20,12,60,255,20,
	9,20,12,20,10,20,9,10,7,10,5,40,0,40,9,20,12,20,10,20,9,10,7,10,5,40,0,40,
	2,19,255,1,2,19,255,1,2,20,4,20,7,20,5,20,4,20,2,20,0,39,255,1,0,39,255,1,0,40,255,40,
	9,20,12,20,10,20,9,10,7,10,5,40,0,40,9,20,12,20,7,20,9,20,5,40,0,40,
	2,19,255,1,2,19,255,1,2,20,4,20,7,20,5,20,4,20,2,20,12,40,10,40,9,40,255,20,
	253
};

//ПАРНАС
const unsigned char musicdata6[]={
	7,29,255,1,7,14,255,1,7,14,255,1,7,14,255,1,7,14,255,1,7,14,255,1,7,15,5,15,8,30,255,55,
	12,15,7,14,255,1,7,15,8,14,255,1,8,14,255,1,8,14,255,1,8,15,7,15,5,15,7,14,255,1,7,14,255,1,7,30,255,30,
	0,14,255,1,0,14,255,1,0,14,255,1,0,15,3,15,2,15,0,15,2,14,255,1,2,14,255,1,2,14,255,1,2,15,0,15,7,30,255,30,
	12,30,7,30,255,30,0,15,255,15,3,15,255,15,3,15,255,15,0,15,255,15,3,15,255,15,3,15,255,15,
	0,15,2,15,3,45,0,15,5,15,3,15,2,45,255,15,2,15,3,15,5,45,2,15,7,15,5,15,3,45,255,15,
	8,14,255,1,8,14,255,1,8,29,255,1,8,30,7,14,255,1,7,14,255,1,7,29,255,1,7,30,12,45,10,15,8,30,7,60,255,30,
	10,14,255,1,10,14,255,1,10,29,255,1,10,30,9,14,255,1,9,14,255,1,9,29,255,1,9,30,
	8,14,255,1,8,14,255,1,8,14,255,1,8,14,255,1,8,14,255,1,8,15,7,60,255,30,
	12,30,7,30,255,30,10,15,8,15,7,30,255,30,5,30,3,30,255,30,2,15,0,15,2,30,255,30,
	7,29,255,1,7,30,255,30,8,29,255,1,8,30,255,30,12,29,255,1,12,40,5,6,6,6,7,6,8,6,9,6,10,6,11,6,12,18,255,60,
	253
};

// 各レベルの曲指定
const unsigned char *musicdatap[]={
	musicdata1,musicdata2,musicdata3,musicdata4,musicdata5,musicdata6
};

//ブロック着地時の効果音（消去したブロック1行～4行用）
const unsigned short soundDong[5][SOUNDDONGLENGTH]={
	{40000,40000,40000,40000,40000,1,0}, //ブロック着地音
	{40000,40000,30000,30000,30000,1,0}, //1行クリアの音
	{30000,30000,30000,20000,20000,20000,0}, //2行クリアの音
	{30000,30000,30000,10000,10000,10000,0}, //3行クリアの音
	{16000,14000,12500,11000,9500,8000,0} //4行クリアの音
};

//背景画像コード 横24×縦14キャラクター
const unsigned char bitmap1[]={
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x62,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x63,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x64,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x65,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x66,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x67,0x68,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x6a,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x6b,0x6c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x6d,0x6e,0x6f,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x70,0x71,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,0x73,0x74,0x75,0x76,0x20,0x20,
	0x20,0x20,0x20,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x20,0x20,
	0x20,0x20,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x20,0x20,0x20,0x20,0x20,0x20,0x8b,0x8c,0x8d,0x20,0x8e,0x8f,0x90,0x20,
	0x20,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x20,0x20,0x20,0x20,0x20,0x20,0x9a,0x9b,0x9c,0x20,0x9d,0x9e,0x20,0x20,
	0x20,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0x20,0x20,0x20,0x20,0x20,0xa9,0xaa,0xab,0xac,0xad,0xae,0x20,0x20,
	0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0x20,0x20,0x20,0x20,0x20,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0x20,0x20,
	0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0x20,0x20,0x20,0x20,0x20,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0x20,
	0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0x20,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
	0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0x20,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff
};

unsigned short palette[256];

volatile unsigned short drawcount;
/**********************
*  Timer2 割り込み処理関数
***********************/
void __ISR(8, ipl4) T2Handler(void){
	drawcount++;
	IFS0bits.T2IF = 0;			// T2割り込みフラグクリア
}

#ifndef BOOTLOADER
void writehighscore(void){
	//ハイスコアのフラッシュ書き込み
	NVMErasePage((void *)FLASHPAGE);//ページ消去
	NVMWriteWord((void *)FLASHPAGE,highscore);//ワード書き込み
}
#endif

void set_palette(unsigned char n,unsigned char b,unsigned char r,unsigned char g){
//グラフィック用カラーパレット設定
	palette[n]=((r>>3)<<11)+((g>>2)<<5)+(b>>3);
}

void pset(int x,int y,unsigned char c){
// (x,y)の位置にカラーパレット番号cで点を描画
	if(x>=0 && x<LCD_X_RES && y>=0 && y<LCD_Y_RES)
		drawPixel(x,y,palette[c]);
}

void init_graphic(void){
	//グラフィックLCD使用開始
	int i;
	//カラーパレット初期化
	for(i=0;i<8;i++){
		set_palette(i,255*(i&1),255*((i>>1)&1),255*(i>>2));
	}
	for(i=0;i<8;i++){
		set_palette(i+8,128*(i&1),128*((i>>1)&1),128*(i>>2));
	}
	for(i=16;i<256;i++){
		set_palette(i,255,255,255);
	}
	LCD_Clear(0);
}

void wait60thsec(unsigned short n){
	// 60分のn秒ウェイト（ビデオ画面の最下行信号出力終了まで待つ）
	drawcount=0;
	while(drawcount<n) asm("wait");
}

unsigned char startkeycheck(unsigned short n){
	// 60分のn秒ウェイト
	// スタートボタンが押されればすぐ戻る
	//　戻り値　スタートボタン押されれば1、押されなければ0
	drawcount=0;

	while(drawcount<n){
		if(~KEYPORT & KEYSTART){
			return 1;
		}
		asm("wait");
	}

	return 0;
}

void playmusic1step(void){
	//演奏中の曲を1つ進める
	if(music.stop) return; //演奏終了済み
	music.count--;
	if(music.count>0){
		PR3=music.pr;
		return;
	}
	//次の音を鳴らす
	if(*music.p==254){ //曲終了
		music.stop=1;
		music.pr=0;
		PR3=0;
		return;
	}
	if(*music.p==253){ //曲の最初に戻る
		music.p=music.startp;
	}
	if(*music.p==255){
		music.pr=0;
		PR3=0; //休符
	}
	else{
		music.pr=sounddata[*music.p]; //周期データ
		PR3=music.pr;
	}
	music.p++;
	music.count=*music.p; //音符長さ
	music.p++;
}

void sound(void);//プロトタイプ宣言（この関数だけどうしても順番が逆転したので）
void playmusic60thsec(void){
	//60分の1秒ウェイト後、現在演奏中の曲を1つ進める
	wait60thsec(1); //60分の1秒ウェイト
//	playmusic1step();
	sound();
}

void wait60thsecmusic(unsigned short n){
	//60分のn秒間、曲を演奏しながらウェイト
	while(n--) playmusic60thsec();
}

void startmusic(unsigned char *m){
	music.p=m;
	music.startp=m;
	music.count=1;
	music.stop=0;
}

void stopmusic(void){
	music.stop=1;
	music.pr=0;
	PR3=0;
}

void putfont(int x,int y,unsigned char c,int bc,unsigned char n){
//8*8ドットのアルファベットフォント表示
//座標(x,y)、カラーパレット番号c
//bc:バックグランドカラー、負数の場合無視
//n:文字番号
	int i,j;
	unsigned char d;
	int skip;
	unsigned short c1;
	const unsigned char *p;
	if(x<=-8 || x>=LCD_X_RES || y<=-8 || y>=LCD_Y_RES) return; //画面外
	if(y<0){ //画面上部に切れる場合
		i=0;
		p=FontData+n*8-y;
	}
	else{
		i=y;
		p=FontData+n*8;
	}
	c1=palette[c];
	if(bc>=0) bc=palette[bc];
	for(;i<y+8;i++){
		if(i>=LCD_Y_RES) return; //画面下部に切れる場合
		d=*p++;
		if(x<0){ //画面左に切れる場合は残る部分のみ描画
			j=0;
			d<<=-x;
		}
		else{
			j=x;
		}
		if(bc<0){
			skip=1;
			for(;j<x+8;j++){
				if(j>=LCD_X_RES){ //画面右に切れる場合
					break;
				}
				if(d&0x80){
					if(skip){
						LCD_SetCursor(j,i);
						skip=0;
					}
					LCD_WriteData16(c1);
				}
				else skip=1;
				d<<=1;
			}
		}
		else{
			LCD_SetCursor(j,i);
			for(;j<x+8;j++){
				if(j>=LCD_X_RES){ //画面右に切れる場合
					break;
				}
				if(d&0x80){
					LCD_WriteData16(c1);
				}
				else{
					LCD_WriteData16(bc);
				}
				d<<=1;
			}
		}
	}
}
void locate(unsigned char x,unsigned char y,unsigned char c){
	//カーソルを座標(x,y)にカラー番号cに設定
	cursorx=x*8;
	cursory=y*8;
	cursorc=c;
}
void printchar2(unsigned char n){
	//カーソル位置、設定カラーでテキストコードnを1文字表示
	putfont(cursorx,cursory,cursorc,0,n);
	cursorx+=8;
	if(cursorx>=LCD_X_RES){
		cursorx=0;
		cursory+=8;
	}
	if(cursory>=LCD_Y_RES) cursory=0;
}
void printchar(unsigned char x,unsigned char y,unsigned char c,unsigned char n){
	//キャラクター座標(x,y)にカラー番号cでテキストコードnを1文字表示
	locate(x,y,c);
	printchar2(n);
}
void printstr(unsigned char x,unsigned char y,unsigned char c,unsigned char *s){
	//座標(x,y)からカラー番号cで文字列sを表示
	locate(x,y,c);
	while(*s){
		printchar2(*s++);
	}
}
void printnumber6(unsigned char x,unsigned char y,unsigned char c,unsigned int s){
	//座標(x,y)にカラー番号cで得点sを表示（6桁）
	x+=6;
	do{
		printchar(x,y,c,'0'+s%10);
		s/=10;
		x--;
	}while(s!=0);
}

void printnext(void){
//NEXTエリアに次のブロックを表示
	const _Block *bp;
	bp=&block[next];
	printstr(25,19,0,"    ");
	printstr(25,20,0,"    ");
	printstr(25,21,0,"    ");
	printstr(25,22,0,"    ");
	printchar(27,21,bp->color,CODE_BLOCK);
	printchar(27+bp->x1,21+bp->y1,bp->color,CODE_BLOCK);
	printchar(27+bp->x2,21+bp->y2,bp->color,CODE_BLOCK);
	printchar(27+bp->x3,21+bp->y3,bp->color,CODE_BLOCK);
}

void show(void){
//board配列の内容を画面に表示
	char x,y;
	for(y=1;y<=23;y++){
		for(x=1;x<=10;x++){
			if(boardchange[y][x]){
				printchar(11+x,y,board[y][x],CODE_BLOCK);
				boardchange[y][x]=0;
			}
		}
	}
}
void displayscore(void){
//得点表示
	printnumber6(0,16,7,score);
	printnumber6(0,19,7,highscore);
}
char check(_Block *bp,char x,char y){
//x,yの位置に_Block構造体bl（ポインタ渡し）をおけるかチェック
//戻り値　0:おける　-1:おけない
	if(board[y][x]) return -1;
	if(board[y+bp->y1][x+bp->x1]) return -1;
	if(board[y+bp->y2][x+bp->x2]) return -1;
	if(board[y+bp->y3][x+bp->x3]) return -1;
	return 0;
}
void putblock(void){
//board配列に落下中のブロックを書き込み
	_Block *bp;
	bp=&falling;
	board[blocky][blockx]=bp->color;
	board[blocky+bp->y1][blockx+bp->x1]=bp->color;
	board[blocky+bp->y2][blockx+bp->x2]=bp->color;
	board[blocky+bp->y3][blockx+bp->x3]=bp->color;

	boardchange[blocky][blockx]=1;
	boardchange[blocky+bp->y1][blockx+bp->x1]=1;
	boardchange[blocky+bp->y2][blockx+bp->x2]=1;
	boardchange[blocky+bp->y3][blockx+bp->x3]=1;
}
void eraseblock(void){
//board配列から落下中のブロックを消去
	_Block *bp;
	bp=&falling;
	board[blocky][blockx]=COLOR_SPACE;
	board[blocky+bp->y1][blockx+bp->x1]=COLOR_SPACE;
	board[blocky+bp->y2][blockx+bp->x2]=COLOR_SPACE;
	board[blocky+bp->y3][blockx+bp->x3]=COLOR_SPACE;

	boardchange[blocky][blockx]=1;
	boardchange[blocky+bp->y1][blockx+bp->x1]=1;
	boardchange[blocky+bp->y2][blockx+bp->x2]=1;
	boardchange[blocky+bp->y3][blockx+bp->x3]=1;
}
char newblock(void){
//次のブロック出現
//戻り値：通常0、置けなければ-1（ゲームオーバー）
	const _Block *blockp;

	blockp=&block[next];
	falling.x1=blockp->x1;
	falling.y1=blockp->y1;
	falling.x2=blockp->x2;
	falling.y2=blockp->y2;
	falling.x3=blockp->x3;
	falling.y3=blockp->y3;
	falling.color=blockp->color;
	falling.rot=blockp->rot;
	blockx=6;
	blocky=3;
	blockangle=0;
	blockno=next;
	fallcount=fallspeed;
	next=rand()%7;
	if(check(&falling,blockx,blocky)) return -1;
	printnext(); //NEXTの場所に次のブロック表示
	putblock(); //落下開始のブロック配置
	downkeyrepeat=0; //下キーのリピートを阻止
	return 0;
}
void displaylevel(void){
	//レベル表示
	printstr(13,13,7,"LEVEL");
	printnumber6(14,13,7,level);
	wait60thsecmusic(60*3);
	printstr(13,13,0,"        ");
	printstr(1,22,0,"     ");
	printnumber6(0,22,7,lines);
	printnumber6(0,25,7,level);
}

void moveblock(void){
//ブロックの落下、キー入力チェックで回転、移動
//落下できない場合はgamestatus=1とする

	unsigned short k;
	_Block tempblock;
	const _Block *blockp;
	char movedflag;

	movedflag=0;

	// ボタンチェック

	k=~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
	if(keyold!=KEYUP && k==KEYUP){	//上ボタン（回転）
		if(blockangle<falling.rot){ //軸中心に90度回転
			tempblock.x1=-falling.y1;
			tempblock.y1= falling.x1;
			tempblock.x2=-falling.y2;
			tempblock.y2= falling.x2;
			tempblock.x3=-falling.y3;
			tempblock.y3= falling.x3;
			if(check(&tempblock,blockx,blocky)==0){
				falling.x1=tempblock.x1;
				falling.y1=tempblock.y1;
				falling.x2=tempblock.x2;
				falling.y2=tempblock.y2;
				falling.x3=tempblock.x3;
				falling.y3=tempblock.y3;
				blockangle++;
				movedflag=-1;
			}
		}
		else{ //回転を初期位置に戻す場合
			blockp=&block[blockno];
			tempblock.x1=blockp->x1;
			tempblock.y1=blockp->y1;
			tempblock.x2=blockp->x2;
			tempblock.y2=blockp->y2;
			tempblock.x3=blockp->x3;
			tempblock.y3=blockp->y3;
			if(check(&tempblock,blockx,blocky)==0){
				falling.x1=tempblock.x1;
				falling.y1=tempblock.y1;
				falling.x2=tempblock.x2;
				falling.y2=tempblock.y2;
				falling.x3=tempblock.x3;
				falling.y3=tempblock.y3;
				blockangle=0;
				movedflag=-1;
			}
		}
	}
	else if(keyold!=KEYRIGHT && k==KEYRIGHT){	//右ボタン
		if(check(&falling,blockx+1,blocky)==0) blockx++;
		movedflag=-1;
	}
	else if(keyold!=KEYLEFT && k==KEYLEFT){	//左ボタン
		if(check(&falling,blockx-1,blocky)==0) blockx--;
		movedflag=-1;
	}
	else if(downkeyrepeat && k==KEYDOWN){	//下ボタン
		if(check(&falling,blockx,blocky+1)==0){
			blocky++;
			movedflag=-1;
			score++;
			if(score>highscore){
				highscore=score;
			}
		}
	}
	if((k&KEYDOWN)==0){
		downkeyrepeat=-1; // 新ブロック出現時の下キーリピート解除
	}
	keyold=k;

	fallcount--;
	if(fallcount==0){ //自然落下
		fallcount=fallspeed;
		if(check(&falling,blockx,blocky+1)) gamestatus=1; //着地完了（固定）
		else{
			blocky++;
			movedflag=-1;
		}
	}
	if(movedflag){
		if(check(&falling,blockx,blocky+1)){
			sounddatap=soundDong[0]; //着地音
		}
	}
}

void linecheck(void){
//完成ラインのチェックと消去、得点加算
	char x,y,x2,y2,c,cleared,cleared2;

	//消去するラインがあれば白いブロックに変更
	cleared=0;
	y=blocky+2;
	if(y>23) y=23;
	while(y>=blocky-2){
		c=0;
		for(x=1;x<=10;x++){
			if(board[y][x]) c++;
		}
		if(c==10){
			cleared++;
			locate(12,y,COLOR_CLEARBLOCK);
			for(x=1;x<=10;x++) printchar2(CODE_CLEARBLOCK);
		}
		y--;
	}
	if(cleared==0) return;

	wait60thsecmusic(15); //60分の15秒待ち

	//白いブロックの行を消去して、一番上に獲得した得点表示
	y=blocky+2;
	if(y>23) y=23;
	cleared2=cleared;
	while(cleared2>0){
		c=0;
		for(x=1;x<=10;x++){
			if(board[y][x]) c++;
		}
		if(c==10){
			printstr(12,y,0,"          ");
			cleared2--;
		}
		y--;
	}
	printnumber6(12,y+1,7,scorearray[cleared-1]);

	wait60thsecmusic(15); //60分の15秒待ち

	//消去した行の分、全体を落下させる
	cleared=0;
	y=blocky+2;
	if(y>23) y=23;
	while(y>=blocky-2 && cleared<4){
		c=0;
		for(x=1;x<=10;x++){
			if(board[y][x]) c++;
		}
		if(c==10){
			cleared++;
			for(y2=y;y2>0;y2--){
				for(x2=1;x2<=10;x2++){
					board[y2][x2]=board[y2-1][x2];
					boardchange[y2][x2]=1;
				}
			}
		}
		else y--;
	}

	//消去した行数に合わせて得点加算
	score+=scorearray[cleared-1];
	if(score>highscore){
		highscore=score;
	}
	lines+=cleared;
	sounddatap=soundDong[cleared]; //消去した行数に合わせた効果音を鳴らす
	printnumber6(0,22,7,lines);
}
void sound(void){
	//効果音と曲を出力（効果音優先）
	//60分の1秒ごとに呼び出し
	unsigned short pr;//タイマーカウンター値

	playmusic1step();
	pr=0;
	if(*sounddatap!=0){
		pr=*sounddatap++;
	}
	if(pr==1) PR3=0;
	else if(pr!=0) PR3=pr;
}

void gameinit(void){
//リセット後1回だけ呼ばれる初期化

	//カラーパレット定義
	set_palette(COLOR_WALL,200,200,200); //壁の色
	set_palette(COLOR_LBLOCK,0,255,165); //L形ブロック色
	set_palette(COLOR_BITMAP,0xde,0xb0,0xc4); //背景画像の色
	set_palette(COLOR_BRICK,0x22,0xb2,0x22); //背景画像の色

	highscore=read_recscore(0); //ブートローダ対応共通領域からレコードの1つ目の32ビットを読み出し
	score=0;
}
void gameinit2(void){
//ゲームスタートボタン押下後に呼ばれる初期化
	int i;
	unsigned char y;
	const unsigned char *p;
	score=0;
	level=0;
	fallspeed=60;
	clearscreen();
	locate(0,15,COLOR_BRICK);
	for(i=0;i<12*30;i++) printchar2(CODE_BRICK);

// 背景表示
	p=bitmap1;
	for(y=0;y<=13;y++){
		locate(0,y,COLOR_BITMAP);
		for(i=0;i<=10;i++) printchar2(*p++);
		p+=13;
	}
	p=bitmap1+16;
	for(y=0;y<=13;y++){
		locate(23,y,COLOR_BITMAP);
		for(i=23;i<=29;i++) printchar2(*p++);
		p+=17;
	}

// 得点表示枠を描画
	printchar(0,14,COLOR_FRAME,0x04);
	for(i=1;i<=8;i++) printchar2(0x07);
	printchar2(0x08);
	for(y=15;y<=25;y++){
		printchar(0,y,COLOR_FRAME,0x05);
		printstr(1,y,COLOR_FRAME,"        ");
		printchar2(0x05);
	}
	printchar(0,26,COLOR_FRAME,0x06);
	for(i=1;i<=8;i++) printchar2(0x07);
	printchar2(0x09);

	printstr(2,15,7,"SCORE");
	printstr(1,18,7,"HI-SCORE");
	printstr(2,21,7,"LINES");
	printstr(2,24,7,"LEVEL");

//NEXT表示枠を描画
	printchar(24,16,COLOR_FRAME,0x04);
	for(i=1;i<=4;i++) printchar2(0x07);
	printchar2(0x08);
	for(y=17;y<=22;y++){
		printchar(24,y,COLOR_FRAME,0x05);
		printstr(25,y,COLOR_FRAME,"    ");
		printchar2(0x05);
	}
	printchar(24,23,COLOR_FRAME,0x06);
	for(i=1;i<=4;i++) printchar2(0x07);
	printchar2(0x09);
	printstr(25,18,7,"NEXT");

	locate(11,0,COLOR_WALL);
	for(i=11;i<=22;i++) printchar2(CODE_WALL);
	for(y=1;y<=23;y++){
		printchar(11,y,COLOR_WALL,CODE_WALL);
		for(i=12;i<=21;i++) printchar2(' ');
		printchar(22,y,COLOR_WALL,CODE_WALL);
	}
	locate(11,24,COLOR_WALL);
	for(i=11;i<=22;i++) printchar2(CODE_WALL);
	displayscore();
	next=rand()%7;
	printnext(); //NEXTの場所に次のブロック表示
	sounddatap=soundDong[0]+SOUNDDONGLENGTH-1;

	//ゲームエリアの初期化
	for(i=0;i<12;i++) {
		for(y=0;y<25;y++) {
			if(i==0 || i==11 || y==24) {
				board[y][i]=COLOR_WALL;
			} else {
				board[y][i]=COLOR_SPACE;
				boardchange[y][i]=1;
			}
		}
	}
}

void gameinit3(void){
//各レベルごとに呼ばれる初期化
	char x,y;
	lines=0;			//消去ライン数クリア
	level++;

	//落下速度更新
	if(fallspeed<=8) fallspeed=3;
	else fallspeed-=5;

	//ブロック再描画用処理
	for(x=1;x<11;x++) {
		for(y=0;y<24;y++) {
			boardchange[y][x]=1;
		}
	}

 	startmusic((unsigned char *)musicdatap[(level-1)%(sizeof musicdatap/sizeof musicdatap[0])]);//各レベルの音楽開始
	keyold=~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
	srand(gcount);
}
void gameover(void){
//ゲームオーバー
	printstr(13,13,7,"GAME OVER");
	wait60thsecmusic(60*5); //5秒ウェイト
	stopmusic();
#ifdef BOOTLOADER
	if(highscore>read_recscore(0)){
		write_recscore(highscore,0);//ブートローダ対応共通領域のレコードにハイスコア書き込み
	}
#else
	if(highscore>*(unsigned int *)FLASHPAGE){
		writehighscore();//ハイスコア書き込み
	}
#endif
}
void title(void){
	//タイトル画面表示
	unsigned char x,y,c;
	const unsigned char *p;
	clearscreen();

	//背景画像表示
	p=bitmap1;
	for(y=0;y<14;y++){
		locate(3,y+3,COLOR_BITMAP);
		for(x=0;x<24;x++){
			printchar2(*p++);
		}
	}

	//タイトルロゴ表示
	c=0x0a;
	locate(10,4,4);
	for(x=1;x<=10;x++) printchar2(c++);
	locate(10,5,4);
	for(x=1;x<=10;x++) printchar2(c++);

	printstr(5,0,5,"SCORE");
	printstr(18,0,5,"HI-SCORE");
	printnumber6(5,1,7,score);
	printnumber6(20,1,7,highscore);

	printstr(3,19,5,"LEFT   \x5c");
	printstr(3,20,5,"RIGHT  \x5b");
	printstr(3,21,5,"ROTATE \x1e");
	printstr(3,22,5,"DOWN   \x1f");
	printstr(17,19,7,"TETRIS");
	printstr(15,21,7,"FOR PIC32MX");
	printstr(17,22,7,"\x5eKENKEN");

	printstr(6,25,6,"PUSH START BUTTON");
	while(1){
		if(startkeycheck(600)) return;
	}
}

void game(void){
// gamestatus
// 0:ゲームスタート
// 1:新ブロック出現
// 2:ブロック落下中
// 3:ステージクリア
// 4:ゲームオーバー
	gameinit2();
	gamestatus=0;
	while(gamestatus<4){
		gameinit3();
		displaylevel();//レベル表示
		gamestatus=1;
		while(gamestatus<3){
			if(newblock()) gamestatus=4;//新ブロック出現、ゲームオーバーチェック
			else gamestatus=2;
			drawcount=0;
			while(gamestatus==2){
				while(drawcount==0) asm("wait");//アイドルにして割り込み待ち
				drawcount=0;
				sound();		//効果音出力
				eraseblock();	//ブロック消去
				moveblock();	//ブロック移動、着地完了チェック
				putblock();		//ブロック配置
				if(gamestatus==1){	//ブロック着地完了の場合
					linecheck();//ライン完成チェック、完成ライン消去
					if(lines>=SCENECLEARLINE) gamestatus=3;
				}
				displayscore();
				show();			//board配列の内容を画面出力
				gcount++;
			}
		}
	}
	gameover();
}

int main(void){
	/* ポートの初期設定 */
	TRISB = 0x0000;						// 全て出力
	TRISC = 0x0000;						// 全て出力
	TRISD = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// ボタン接続ポート入力設定
	TRISE = 0x0000;						// 全て出力
	TRISF = 0x0003;						// RF0,1は入力
	TRISG = 0x0080;						// RG7は入力
	ANSELB = 0x0000;                    // 全てデジタル
	ANSELD = 0x0000;                    // 全てデジタル
	ANSELG = 0x0000;                    // 全てデジタル
	CNPUDSET= KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// プルアップ設定

	// 周辺機能ピン割り当て
	SDI2R = 1;							//RPG7にSDI2を割り当て
	RPG8R = 6;							//RPG8にSDO2を割り当て
    
	OSCCONCLR=0x10; // WAIT命令はアイドルモード
	INTEnableSystemMultiVectoredInt();
	SYSTEMConfig(96000000,SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE); //キャッシュ有効化（周辺クロックには適用しない）
	BMXCONCLR=0x40;	// RAMアクセスウェイト0

    //サウンド関係初期設定(R)
//    RPF4R=0x0B;    //RPF4にOC3を割り当て
//    RPD10R=0x0B;   // RPD10にOC3を割り当て
//    OC3R=0;
//    OC3CON=0x800B; // Tiemr3ベース、Toggleモードで有効化
    //サウンド関係初期設定(L)
    RPF5R = 0x0B;  //RPF5にOC4を割り当て
    RPD11R=0x0B;   // RPD11にOC4を割り当て
    OC4R=0;
    OC4CON=0x800B; // Tiemr3ベース、Toggleモードで有効化
    PR3=0;// Timer3周期
    T3CON=0x8040; // プリスケーラ1:16でTimer3開始

#ifdef BOOTLOADER
	init_recscore();//ブートローダ対応の共有ハイスコア記録領域を検索し、レコードがなければ新規作成
	if(read_recscore(0)==0xffffffff){
		highscore=1000;
		write_recscore(highscore,0);//ハイスコア初期値10000点
	}
	else highscore=read_recscore(0); //ブートローダ対応共通領域からレコードの1つ目の32ビットを読み出し
#else
	if(*(unsigned int *)FLASHPAGE==-1){ //フラッシュメモリにハイスコアが書き込まれているかチェック
		highscore=1000;
		writehighscore();
	}
	else highscore=*(unsigned int *)FLASHPAGE;
#endif

	// Timer2初期設定（60分の1秒カウンタ）
	T2CON = 0x0050;				// プリスケーラ1:32、タイマ2停止状態
	IPC2bits.T2IP = 4;			// 割り込みレベル4
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;			// タイマ2割り込み有効化
	PR2=50000;			    	// 60分の1秒
	T2CONSET = 0x8000;			// タイマ2スタート

	init_graphic();
	gameinit(); //ゲーム全体初期化
	while(1){
		title();//タイトル画面、スタートボタンで戻る
		game();//ゲームメインループ
	}
}
