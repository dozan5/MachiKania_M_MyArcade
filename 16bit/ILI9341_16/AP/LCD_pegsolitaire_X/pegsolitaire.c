//ペグソリテア for PIC32MX250F128B / Color LCD Game System w/bootloader by K.Tanaka
//をPIC32MX370F512H用に改編

#include <plib.h>
#include "LCDdriver.h"
#include "graphlib.h"

// ブートローダ使用の場合、ここでのconfig設定は無効
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
#define BOARDXSIZE 7
#define BOARDYSIZE 7
#define BALLXSIZE 32
#define BALLYSIZE 32

// グローバル変数定義
unsigned short keystatus,keystatus2,oldkey; //最新のボタン状態と前回のボタン状態
volatile unsigned short drawcount;
unsigned char board[BOARDYSIZE][BOARDXSIZE]; //盤の状態
unsigned char undob[BOARDXSIZE*BOARDYSIZE][BOARDYSIZE][BOARDXSIZE]; //盤の状態
int balls; //ボール残数
int step; //現在の手数
int cursorx1,cursory1,cursorx2,cursory2; //カーソル位置

// 盤初期データ
// 0：穴なし　1：穴（ボールあり）　2：穴（ボールなし）
const unsigned char BOARD[]={
	0,0,1,1,1,0,0,
	0,0,1,1,1,0,0,
	1,1,1,1,1,1,1,
	1,1,1,2,1,1,1,
	1,1,1,1,1,1,1,
	0,0,1,1,1,0,0,
	0,0,1,1,1,0,0
};

// カラーパレットデータ（8～15）
const unsigned char PALDAT[]={
	185,122,87,
	34,177,76,
	136,0,21,
	255,255,255,
	128,0,0,
	0,0,0,
	0,0,0,
	0,0,0
};

// 画像ビットマップ
// BMP1：ボールあり画像　BMP2：穴（ボールなし）画像
const unsigned char BMP1[]={
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,
	8,8,8,8,8,8,9,9,9,9,9,9,9,9,11,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,
	8,8,8,8,8,8,9,9,9,9,9,9,11,11,11,11,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,
	8,8,8,8,8,9,9,9,9,9,9,11,11,11,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,
	8,8,8,8,9,9,9,9,9,11,11,11,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,
	8,8,8,8,9,9,9,9,9,11,11,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,
	8,8,8,8,9,9,9,9,11,11,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,
	8,8,8,9,9,9,9,9,11,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,8,8,
	8,8,8,9,9,9,9,11,11,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,8,8,
	8,8,8,9,9,9,9,11,11,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,8,8,
	8,8,8,9,9,9,9,9,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,8,8,
	8,8,8,9,9,9,9,9,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,8,8,
	8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,8,8,
	8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,8,8,
	8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,12,8,8,
	8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,12,8,8,
	8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,12,8,8,8,
	8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,12,12,8,8,8,
	8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,12,8,8,8,8,
	8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,12,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,12,12,12,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,12,12,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,12,12,12,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,12,12,12,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};
const unsigned char BMP2[]={
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,10,10,10,10,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,10,10,10,10,10,10,10,10,10,10,10,10,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,8,8,8,10,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,8,8,8,8,8,8,10,10,8,8,8,8,8,8,8,
	8,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,8,8,8,
	8,8,8,8,8,8,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,8,8,8,
	8,8,8,8,8,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,8,8,
	8,8,8,8,10,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,8,
	8,8,8,8,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,8,
	8,8,8,8,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,
	8,8,8,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,
	8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,
	8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,
	8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,11,8,8,8,10,8,8,8,
	8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,11,8,8,8,10,8,8,8,
	8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,11,11,8,8,8,10,8,8,8,
	8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,11,8,8,8,8,10,8,8,8,
	8,8,8,8,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,11,11,8,8,8,10,8,8,8,8,
	8,8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,11,11,8,8,8,8,10,8,8,8,8,
	8,8,8,8,8,10,8,8,8,8,8,8,8,8,8,8,8,8,11,11,11,11,8,8,8,8,10,8,8,8,8,8,
	8,8,8,8,8,8,10,8,8,8,8,8,8,8,8,8,8,11,11,8,8,8,8,8,8,10,8,8,8,8,8,8,
	8,8,8,8,8,8,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,8,8,8,
	8,8,8,8,8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,10,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,10,8,8,8,8,8,8,8,8,8,8,8,8,10,10,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,10,10,8,8,8,8,8,8,8,8,10,10,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,10,10,10,10,10,10,10,10,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};
const unsigned int SOUND1[]={
	0x64000,1
};
const unsigned int SOUND2[]={
	0x60800,0x60C00,0x61000,0x61800,0x62000,0x61800,0x61000,0x60C00,0x60800,0x60400,1
};
const unsigned int SOUND3[]={
	0x306E0,0x30700,1
};
const unsigned int SOUND4[]={
	0x30B80,0x30C00,1
};
const unsigned int SOUND5[]={
	0x30400,1
};
const unsigned int SOUND6[]={
	0x30400,0x20000,0x30400,1
};

/**********************
*  Timer2 割り込み処理関数
***********************/
void __ISR(8, ipl4) T2Handler(void){
	drawcount++;
	IFS0bits.T2IF = 0;			// T2割り込みフラグクリア
}

void wait_video_end(unsigned short n){
//ビデオ信号出力n回終了するまでのウェイト（60分のn秒ウェイト）
//ビデオ信号出力割り込み処理中以外はアイドル状態とする
	drawcount=0;
	while(drawcount<n) asm("wait");
}

void keycheck(void){
//ボタン状態読み取り
//keystatus :現在押されているボタンに対応するビットを1にする
//keystatus2:前回押されていなくて、今回押されたボタンに対応するビットを1にする
	oldkey=keystatus;
	keystatus=~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT | KEYSTART | KEYFIRE);
	keystatus2=keystatus & ~oldkey; //ボタンから手を離したかチェック
}

void sound(const unsigned int *s){
//効果音出力
//以下の32bit列を与える
//上位16bit：音程、2048が440Hz、大きいほど低音
//下位16bit：音の長さ、60分の1秒単位
//最後に上位16bitが0で繰り返し回数を指定
	unsigned int pr,t;
	int n;
	const unsigned int *p;
	p=s;
	while(*p>=0x10000) p++;
	wait_video_end(1);
	for(n=*p;n>0;n--){
		p=s;
		while(*p>=0x10000){
			pr=*p & 0xffff;
			t=*p>>16;
			pr*=54545; //2048で440Hzとなるように補正
			PR3=pr>>16; //サウンド周波数設定
			wait_video_end(t); //60分のt秒ウェイト
			p++;
		}
	}
	PR3=0; //サウンド停止
}
void score(void){
// ボール残り数表示
	printnum2(184,30,7,8,balls,2);
}
void getundob(void){
//アンドゥバッファからコピー
	int i,j;
	for(i=0;i<BOARDYSIZE;i++){
		for(j=0;j<BOARDXSIZE;j++){
			board[i][j]=undob[step][i][j];
		}
	}
}
void setundob(void){
//アンドゥバッファにコピー
	int i,j;
	for(i=0;i<BOARDYSIZE;i++){
		for(j=0;j<BOARDXSIZE;j++){
			undob[step][i][j]=board[i][j];
		}
	}
}
void gameinit2(void){
	step=0;
	cursorx1=3;
	cursory1=3;
}
void gameinit(void){
//起動時1回だけ呼ばれる初期化
	int i,j;
	unsigned char r,g,b;
	const unsigned char *p;
	//カラーパレット初期化
	p=PALDAT;
	for(i=8;i<16;i++){
		r=*p++;
		g=*p++;
		b=*p++;
		set_palette(i,b,r,g);
	}
	clearscreen();
	boxfill(0,0,BALLXSIZE*BOARDXSIZE-1,BALLYSIZE*BOARDYSIZE-1,8);
	printstr(172,20,7,-1,"BALLS");
	printstr(160,162,7,-1,"FIRE:");
	printstr(176,172,7,-1,"SELECT");
	printstr(160,182,7,-1,"START:");
	printstr(176,192,7,-1,"BACK");
	printstr(160,202,7,-1,"START 2s");
	printstr(176,212,7,-1,"RESET");
	p=BOARD;
	for(i=0;i<BOARDYSIZE;i++){
		for(j=0;j<BOARDXSIZE;j++){
			board[i][j]=*p++;
		}
	}
	setundob(); //アンドゥバッファにコピー
	gameinit2();
	//ボタン連続押し防止の初期設定
	keystatus=KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT | KEYSTART | KEYFIRE;
}
void putboard(void){
//盤全体を再描画
	int i,j;
	balls=0;
	for(i=0;i<BOARDYSIZE;i++){
		for(j=0;j<BOARDXSIZE;j++){
			if(board[i][j]==1){
				putbmpmn(j*BALLXSIZE,i*BALLYSIZE,BALLXSIZE,BALLYSIZE,BMP1);
				balls++;
			}
			else if(board[i][j]==2){
				putbmpmn(j*BALLXSIZE,i*BALLYSIZE,BALLXSIZE,BALLYSIZE,BMP2);
			}
		}
	}
	score();
}
void putcursor(int x,int y,unsigned char c){
// カーソル枠を表示
	boxfill(x*BALLXSIZE,y*BALLYSIZE,x*BALLXSIZE+1,y*BALLYSIZE+BALLYSIZE-1,c);
	boxfill(x*BALLXSIZE,y*BALLYSIZE,x*BALLXSIZE+BALLXSIZE-1,y*BALLYSIZE+1,c);
	boxfill(x*BALLXSIZE+BALLXSIZE-2,y*BALLYSIZE,x*BALLXSIZE+BALLXSIZE-1,y*BALLYSIZE+BALLYSIZE-1,c);
	boxfill(x*BALLXSIZE,y*BALLYSIZE+BALLYSIZE-2,x*BALLXSIZE+BALLXSIZE-1,y*BALLYSIZE+BALLYSIZE-1,c);
}
void undocheck(void){
// STARTボタンをチェックし、アンドゥを行う
// 長押しした場合は、初期状態に戻す
	int t=0;
	while(t<120){
		wait_video_end(1);
		keycheck();
		if(keystatus!=KEYSTART) break;
		t++;
	}
	if(t==120){
		//初期状態に戻す
		gameinit2();
		sound(SOUND6);
	}
	else{
		//アンドゥ（1つ前に戻す）
		if(step>0){
			step--;
			sound(SOUND5);
		}
	}
}
int move1(void){
// 移動するボールを選択
// 戻り値　1：STARTボタン押下　0：その他
	while(1){
		putcursor(cursorx1,cursory1,2); //赤カーソル表示
		do{
			wait_video_end(1);
			keycheck();
		} while(keystatus2==0);
		putcursor(cursorx1,cursory1,8); //カーソル消去
		if(keystatus2==KEYSTART){
			// アンドゥまたは初期状態に戻す
			undocheck();
			return 1;
		}
		if(keystatus2==KEYFIRE && board[cursory1][cursorx1]==1) break; //ボールのあるところでFIREボタン
		//上下左右ボタンでカーソル移動
		if(keystatus2==KEYDOWN && cursory1<BOARDYSIZE-1 && board[cursory1+1][cursorx1]) cursory1++;
		if(keystatus2==KEYUP && cursory1>0 && board[cursory1-1][cursorx1]) cursory1--;
		if(keystatus2==KEYLEFT && cursorx1>0 && board[cursory1][cursorx1-1]) cursorx1--;
		if(keystatus2==KEYRIGHT && cursorx1<BOARDXSIZE-1 && board[cursory1][cursorx1+1]) cursorx1++;
	}
	sound(SOUND3);
	return 0;
}
int move2(void){
// ボールの移動先を選択
// 戻り値　1：STARTボタン押下　0：その他
	cursorx2=cursorx1;
	cursory2=cursory1;
	while(1){
		putcursor(cursorx1,cursory1,2); //赤カーソル表示
		putcursor(cursorx2,cursory2,4); //緑カーソル表示
		do{
			wait_video_end(1);
			keycheck();
		} while(keystatus2==0);
		putcursor(cursorx1,cursory1,8); //カーソル消去
		putcursor(cursorx2,cursory2,8); //カーソル消去
		if(keystatus2==KEYSTART){
			// アンドゥまたは初期状態に戻す
			undocheck();
			return 1;
		}
		if(keystatus2==KEYFIRE) break; //FIREボタン

		//上下左右ボタンでカーソル移動
		if(keystatus2==KEYDOWN && cursory2<BOARDYSIZE-1 && board[cursory2+1][cursorx2]) cursory2++;
		if(keystatus2==KEYUP && cursory2>0 && board[cursory2-1][cursorx2]) cursory2--;
		if(keystatus2==KEYLEFT && cursorx2>0 && board[cursory2][cursorx2-1]) cursorx2--;
		if(keystatus2==KEYRIGHT && cursorx2<BOARDXSIZE-1 && board[cursory2][cursorx2+1]) cursorx2++;
	}
	return 0;
}
int movecheck(void){
// ボールが移動できるかチェック
// 戻り値　0：移動できる　1：移動できない
	if(board[cursory2][cursorx2]!=2) return 1; //移動先が開いていない
	if(cursorx1==cursorx2){
		if(cursory1-cursory2!=2 && cursory1-cursory2!=-2) return 1; //距離が2でない
		if(board[(cursory1+cursory2)/2][cursorx1]==1) return 0; //間にボールがある
	}
	else if(cursory1==cursory2){
		if(cursorx1-cursorx2!=2 && cursorx1-cursorx2!=-2) return 1; //距離が2でない
		if(board[cursory1][(cursorx1+cursorx2)/2]==1) return 0; //間にボールがある
	}
	return 1; //筋が違う
}
void move3(void){
// ボールを移動させ、飛び越えたボールを取り除く
	putbmpmn(cursorx2*BALLXSIZE,cursory2*BALLYSIZE,BALLXSIZE,BALLYSIZE,BMP1);
	putbmpmn(cursorx1*BALLXSIZE,cursory1*BALLYSIZE,BALLXSIZE,BALLYSIZE,BMP2);
	putbmpmn((cursorx1+cursorx2)/2*BALLXSIZE,(cursory1+cursory2)/2*BALLYSIZE,BALLXSIZE,BALLYSIZE,BMP2);
	board[cursory2][cursorx2]=1;
	board[cursory1][cursorx1]=2;
	board[(cursory1+cursory2)/2][(cursorx1+cursorx2)/2]=2;
	balls--;
	cursorx1=cursorx2;
	cursory1=cursory2;
	step++;
}
int goalcheck(void){
//パズル完成チェック
//完成の場合、画面表示してSTARTボタン待ちする
//戻り値　未完成:0、完成:1
	if(balls>1) return 0;
	printstr(48,108,6,-1,"CONGRATULATIONS!");
	printstr(72,130,7,-1,"PUSH START");
	printstr(60,140,7,-1,"TO PLAY AGAIN");
	sound(SOUND2);
	while(keystatus2!=KEYSTART){
		wait_video_end(1);
		keycheck();
	}
	gameinit2();//ゲーム初期状態に戻す
	return 1;
}

void main(void){
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

	// Timer2初期設定（60分の1秒カウンタ）
	T2CON = 0x0050;				// プリスケーラ1:32、タイマ2停止状態
	IPC2bits.T2IP = 4;			// 割り込みレベル4
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;			// タイマ2割り込み有効化
	PR2=50000;			    	// 60分の1秒
	T2CONSET = 0x8000;			// タイマ2スタート

	init_graphic();

	//ここからゲーム処理
	gameinit();//最初の1回だけの初期化
	while(1){
		getundob(); //アンドゥバッファから戻す
		putboard(); //盤全体の描画

		//メインループ
		while(1){
			if(move1()) break; //移動するボール選択、STARTボタンでアンドゥまたは初期状態に戻る
			if(move2()) break; //移動先選択、STARTボタンで初期状態に戻る
			if(movecheck()){ //移動できるかチェック、不可なら選択からやり直し
				sound(SOUND1);
				continue;
			}
			move3(); //ボールを移動させる
			setundob(); //アンドゥバッファにコピー
			score(); //ボール残り数表示
			if(goalcheck()) break; //完成チェック、完成の場合最初に戻る
			sound(SOUND4);
		}
	}
}
