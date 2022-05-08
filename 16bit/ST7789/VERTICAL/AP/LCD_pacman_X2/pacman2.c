// パックマン ブートローダ付き液晶ゲームシステム by K.Tanaka
//をPIC32MX370F512H用に改編

#include <plib.h>
#include "pacman2.h"
#include "LCDdriver.h"

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

_Character pacman,akabei,pinky,aosuke,guzuta; //各キャラクターの構造体
_Music music; //演奏中の音楽構造体
unsigned int score,highscore; //得点、ハイスコア
unsigned char player; //パックマン残数
unsigned char stage; //現在のステージ数
unsigned char gamecount; //全体カウンター
unsigned char gamestatus; //0:ゲームスタート、1:ゲーム中、2:プレイヤー1減、3:ステージクリア、4:ゲームオーバー
unsigned short pacmanspeed,monsterspeed,ijikespeed,medamaspeed;//現在のステージの各キャラの移動速度（カウンタ）
unsigned short ijiketime; //現在のステージのイジケ時間
unsigned char fruitno; //現在のステージのフルーツ番号
unsigned char upflag; //10000点越えのプレイヤー増チェック
unsigned char fruitflag1,fruitflag2; //現在のステージでフルーツを出したかのフラグ
unsigned short fruitcount; //フルーツ表示時間カウンター
unsigned char cookie; //えさ残数
unsigned char huntedmonster; //イジケを捕まえた数（得点計算用）
unsigned short monsterhuntedtimer; //イジケを捕まえている間の停止カウンター
unsigned short fruitscoretimer; //フルーツ獲得時の得点表示カウンター
unsigned char monstersoundcount; //モンスター効果音持続時間
unsigned char cookiesoundcount,fruitsoundcount,over10000soundcount;//各種効果音の持続時間
unsigned short monstersound,monstersounddif; //モンスター効果音の値と増減値（モードにより異なる）
unsigned short monsterhuntedsound; //イジケ捕獲効果音の値
unsigned short fruitsound; //フルーツ獲得効果音
unsigned short firekeyold; //一時停止キー状態
unsigned char map[MAPXSIZE*MAPYSIZE]; // 通路、壁、えさ、パワーえさ、フルーツ、ドアがあることを表す
unsigned char fruit[]={0,1,2,2,3,3,4,4,5,5,6,6,7}; //面ごとのフルーツ番号
unsigned short fruitscore[]={10,30,50,70,100,200,300,500}; //フルーツの得点
unsigned short pacmansp[]= {135,150,150,150,160,160,160,170,170,170,190,190,190,210,210,210,210,256,256,256,256}; //面ごとのパックマン速度
unsigned short monstersp[]={135,150,150,150,160,160,160,170,170,170,190,190,190,210,210,210,210,256,256,256,256}; //面ごとのモンスター速度
unsigned short ijikesp[]=  { 50, 60, 60, 60, 65, 65, 65, 70, 70, 70, 75, 75, 75, 80, 80, 80, 80, 90, 90, 90, 90}; //面ごとのイジケ速度
unsigned short ijike[]=    {550,520,450,350,130,300,130,120, 60,300,100, 60, 60,180, 60, 60,  0 ,60,  0,  0,  0}; //面ごとのイジケ時間
unsigned short medamasp[]= {512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512}; //面ごとの目玉速度

//sounddata配列　ド～上のド～その上のドの周期カウンタ値、PR3に書き込むと音程設定される
unsigned short sounddata[]={27363,25828,24378,23010,21718,20499,19349,18263,17238,16270,15357,14495,
						13681,12914,12189,11505,10859,10249,9674,9131,8619,8135,7678,7247,6840};
//musicdata配列　音階,長さ,音階,長さ,・・・・・　最後に音階254で曲終了
//				音階　0:ド～12:上のド～24:その上のド　　255:休符
//				長さ　60分のn秒
unsigned char musicdata1[]={0,3,12,5,19,8,16,8,0,3,12,8,0,3,19,5,16,8,12,8,255,8,
						1,3,13,5,20,8,17,8,1,3,13,8,1,3,20,5,17,8,13,8,255,8,
						0,3,12,5,19,8,16,8,0,3,12,8,0,3,19,5,16,8,12,8,255,8,
						7,3,17,5,18,8,9,3,19,5,20,8,11,3,21,5,23,8,12,3,24,5,254};
unsigned char musicdata2[]={6,6,255,2,6,6,255,2,6,8,3,4,1,4,6,2,255,2,6,12,10,16,
						6,6,255,2,6,6,255,2,6,8,3,4,1,4,6,2,255,2,6,12,3,16,
						6,6,255,2,6,6,255,2,6,8,3,4,1,4,6,2,255,2,6,12,9,8,11,8,
						12,8,11,8,9,8,6,8,9,8,6,16,255,8,
						6,6,255,2,6,6,255,2,6,8,3,4,1,4,6,2,255,2,6,12,10,16,
						6,6,255,2,6,6,255,2,6,8,3,4,1,4,6,2,255,2,6,12,3,16,
						6,6,255,2,6,6,255,2,6,8,3,4,1,4,6,2,255,2,6,12,9,8,11,8,
						12,8,11,8,9,8,6,8,9,8,6,16,254};
//
//マップ定義
unsigned char scenedata[MAPXSIZE*MAPYSIZE]={
	//壁とえさの配置 0x80～0x8e:壁、0x8f:何もなし（通路）、0x90:えさ、0x91:パワーえさ
	0x80,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x8a,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x81,
	0x85,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x85,
	0x85,0x90,0x80,0x84,0x81,0x90,0x80,0x84,0x81,0x90,0x85,0x90,0x80,0x84,0x81,0x90,0x80,0x84,0x81,0x90,0x85,
	0x85,0x91,0x85,0x8f,0x85,0x90,0x85,0x8f,0x85,0x90,0x85,0x90,0x85,0x8f,0x85,0x90,0x85,0x8f,0x85,0x91,0x85,
	0x85,0x90,0x82,0x84,0x83,0x90,0x82,0x84,0x83,0x90,0x86,0x90,0x82,0x84,0x83,0x90,0x82,0x84,0x83,0x90,0x85,
	0x85,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x85,
	0x85,0x90,0x80,0x84,0x81,0x90,0x87,0x90,0x80,0x84,0x84,0x84,0x81,0x90,0x87,0x90,0x80,0x84,0x81,0x90,0x85,
	0x85,0x90,0x82,0x84,0x83,0x90,0x85,0x90,0x82,0x84,0x8a,0x84,0x83,0x90,0x85,0x90,0x82,0x84,0x83,0x90,0x85,
	0x85,0x90,0x90,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x90,0x90,0x85,
	0x82,0x84,0x84,0x84,0x81,0x90,0x8d,0x84,0x89,0x90,0x86,0x90,0x88,0x84,0x8c,0x90,0x80,0x84,0x84,0x84,0x83,
	0x8f,0x8f,0x8f,0x8f,0x85,0x90,0x85,0x8f,0x8f,0x8f,0x8f,0x8f,0x8f,0x8f,0x85,0x90,0x85,0x8f,0x8f,0x8f,0x8f,
	0x8f,0x8f,0x8f,0x8f,0x85,0x90,0x85,0x8f,0x80,0x84,0x8e,0x84,0x81,0x8f,0x85,0x90,0x85,0x8f,0x8f,0x8f,0x8f,
	0x88,0x84,0x84,0x84,0x83,0x90,0x86,0x8f,0x85,0x8f,0x8f,0x8f,0x85,0x8f,0x86,0x90,0x82,0x84,0x84,0x84,0x89,
	0x8f,0x8f,0x8f,0x8f,0x8f,0x90,0x8f,0x8f,0x85,0x8f,0x8f,0x8f,0x85,0x8f,0x8f,0x90,0x8f,0x8f,0x8f,0x8f,0x8f,
	0x88,0x84,0x84,0x84,0x81,0x90,0x87,0x8f,0x82,0x84,0x84,0x84,0x83,0x8f,0x87,0x90,0x80,0x84,0x84,0x84,0x89,
	0x8f,0x8f,0x8f,0x8f,0x85,0x90,0x85,0x8f,0x8f,0x8f,0x8f,0x8f,0x8f,0x8f,0x85,0x90,0x85,0x8f,0x8f,0x8f,0x8f,
	0x8f,0x8f,0x8f,0x8f,0x85,0x90,0x85,0x8f,0x80,0x84,0x84,0x84,0x81,0x8f,0x85,0x90,0x85,0x8f,0x8f,0x8f,0x8f,
	0x80,0x84,0x84,0x84,0x83,0x90,0x86,0x8f,0x82,0x84,0x8a,0x84,0x83,0x8f,0x86,0x90,0x82,0x84,0x84,0x84,0x81,
	0x85,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x85,
	0x85,0x90,0x88,0x84,0x81,0x90,0x88,0x84,0x89,0x90,0x86,0x90,0x88,0x84,0x89,0x90,0x80,0x84,0x89,0x90,0x85,
	0x85,0x91,0x90,0x90,0x85,0x90,0x90,0x90,0x90,0x90,0x8f,0x90,0x90,0x90,0x90,0x90,0x85,0x90,0x90,0x91,0x85,
	0x8d,0x84,0x81,0x90,0x85,0x90,0x87,0x90,0x80,0x84,0x84,0x84,0x81,0x90,0x87,0x90,0x85,0x90,0x80,0x84,0x8c,
	0x8d,0x84,0x83,0x90,0x86,0x90,0x85,0x90,0x82,0x84,0x8a,0x84,0x83,0x90,0x85,0x90,0x86,0x90,0x82,0x84,0x8c,
	0x85,0x90,0x90,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x85,0x90,0x90,0x90,0x90,0x90,0x85,
	0x85,0x90,0x88,0x84,0x84,0x84,0x8b,0x84,0x89,0x90,0x86,0x90,0x88,0x84,0x8b,0x84,0x84,0x84,0x89,0x90,0x85,
	0x85,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x85,
	0x82,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x83
};

//マクロ設定　仮想マップの読み出し、書き込み
#define GETMAP(x,y) map[(y)*MAPXSIZE+(x)]
#define SETMAP(x,y,d) map[(y)*MAPXSIZE+(x)]=(d)

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

	// Timer5初期設定（マイクロ秒ディレイ用）
	T5CON=0x0010;				// プリスケーラ1:2、タイマ5停止状態
	IPC5bits.T5IP=4;			// 割り込みレベル4
	IFS0bits.T5IF=0;
	IEC0bits.T5IE=1;			// タイマ5割り込み有効化

	LCD_Init();
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
void playmusic60thsec(void){
	//60分の1秒ウェイト後、現在演奏中の曲を1つ進める
	wait60thsec(1); //60分の1秒ウェイト
	if(music.stop) return; //演奏終了済み
	music.count--;
	if(music.count>0) return;

	//次の音を鳴らす
	if(*music.p==254){ //曲終了
		music.stop=1;
		PR3=0;
		return;
	}
	if(*music.p==255) PR3=0; //休符
	else PR3=sounddata[*music.p]; //周期データ
	music.p++;
	music.count=*music.p; //音符長さ
	music.p++;
}
void startmusic(unsigned char *m){
	music.p=m;
	music.count=1;
	music.stop=0;
}
/*
//　単に曲演奏　（今回は未使用）
void playmusic(unsigned char *m){
	startmusic(m);
	while(music.stop==0){
		playmusic60thsec();
	}
}	
*/
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
void printchar(unsigned char x,unsigned char y,unsigned char c,unsigned char n){
	//キャラクター座標(x,y)にカラー番号cでテキストコードnを1文字表示
	putfont(x*8,y*8,c,0,n);
}
void printstr(unsigned char x,unsigned char y,unsigned char c,unsigned char *s){
	//座標(x,y)からカラー番号cで文字列sを表示
	while(*s){
		printchar(x++,y,c,*s++);
	}
}
void printscore(unsigned char x,unsigned char y,unsigned char c,unsigned int s){
	//座標(x,y)にカラー番号cで得点sを表示（5桁）
	x+=5;
	do{
		printchar(x,y,c,'0'+s%10);
		s/=10;
		x--;
	}while(s!=0);
}

void putbmpmn2(int x,int y,unsigned char m,unsigned char n,const unsigned char bmp[]){
// 横m*縦nドットのキャラクターを座標x,yに表示
// ゲームのマップ内だけに描画の場合はこちら（マップ外にはみ出した部分は表示しない）
// unsigned char bmp[m*n]配列に、単純にカラー番号を並べる
// カラー番号が0の部分は透明色として扱う
	int i,j;
	int skip;
	const unsigned char *p;
	if(x<=-m || x>=MAPXSIZE*8 || y<=-n || y>=MAPYSIZE*8) return; //画面外
	if(y<0){ //画面上部に切れる場合
		i=0;
		p=bmp-y*m;
	}
	else{
		i=y;
		p=bmp;
	}
	for(;i<y+n;i++){
		if(i>=MAPYSIZE*8) return; //画面下部に切れる場合
		if(x<0){ //画面左に切れる場合は残る部分のみ描画
			j=0;
			p+=-x;
		}
		else{
			j=x;
		}
		skip=1;
		for(;j<x+m;j++){
			if(j>=MAPXSIZE*8){ //画面右に切れる場合
				p+=x+m-j;
				break;
			}
			if(*p!=0){ //カラー番号が0の場合、透明として処理
				if(skip){
					LCD_SetCursor(j,i);
					skip=0;
				}
				LCD_WriteData16(palette[*p]);
			}
			else skip=1;
			p++;
		}
	}
}
void putbmp(int x,int y,unsigned char m,unsigned char n,const unsigned char bmp[]){
// 横m*縦nドットのキャラクターを座標x,yに表示(画面全体が描画対象の場合)
// unsigned char bmp[m*n]配列に、単純にカラー番号を並べる
// カラー番号が0の部分は透明色として扱う
	int i,j;
	int skip;
	const unsigned char *p;
	if(x<=-m || x>LCD_X_RES || y<=-n || y>=LCD_Y_RES) return; //画面外
	if(y<0){ //画面上部に切れる場合
		i=0;
		p=bmp-y*m;
	}
	else{
		i=y;
		p=bmp;
	}
	for(;i<y+n;i++){
		if(i>=LCD_Y_RES) return; //画面下部に切れる場合
		if(x<0){ //画面左に切れる場合は残る部分のみ描画
			j=0;
			p+=-x;
		}
		else{
			j=x;
		}
		skip=1;
		for(;j<x+m;j++){
			if(j>=LCD_X_RES){ //画面右に切れる場合
				p+=x+m-j;
				break;
			}
			if(*p!=0){ //カラー番号が0の場合、透明として処理
				if(skip){
					LCD_SetCursor(j,i);
					skip=0;
				}
				LCD_WriteData16(palette[*p]);
			}
			else skip=1;
			p++;
		}
	}
}

// 縦m*横nドットのキャラクター消去
// カラー0で塗りつぶし
void clrbmpmn(int x,int y,unsigned char m,unsigned char n){
	int i,j,k;
	if(x<=-m || x>=LCD_X_RES || y<=-n || y>=LCD_Y_RES) return; //画面外
	if(y<0) i=0; //画面上部に切れる場合
	else i=y;
	if(x<0) j=0; //画面左に切れる場合は残る部分のみ描画
	else j=x;
	if(x+m>=LCD_X_RES) k=LCD_X_RES-j; //画面右に切れる場合
	else k=x+m-j;
	for(;i<y+n;i++){
		if(i>=LCD_Y_RES) return; //画面下部に切れる場合
		LCD_continuous_output(j,i,0,k);
	}
}
void putpacman(void){
	//パックマンの表示
	unsigned char a;
	pacman.animcount--;
	if(pacman.animcount==0){
		pacman.animcount=pacman.animcount0;
		pacman.animvalue++;
		if(pacman.animvalue==6) pacman.animvalue=0;
	}
	if(pacman.animvalue==0) a=0;
	else if(pacman.animvalue<=3) a=pacman.dir*3+pacman.animvalue;
	else a=pacman.dir*3+6-pacman.animvalue;
	putbmp((int)(pacman.x/256)-3,(int)(pacman.y/256)-3,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[a]);
}
void putmonster(_Character *p){
	//モンスター表示　p:キャラクターのポインタ指定
	unsigned char i;//モンスターの足のパターン（2種類）
	if(gamecount & 4) i=0;
	else i=1;
	switch(p->status){
		case IJIKE:
			if(p->modecount>180 || gamecount & 8)
				putbmp((int)(p->x/256)-3,(int)(p->y/256)-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Ijikebmp[i]);
			else
				//白で点滅
				putbmp((int)(p->x/256)-3,(int)(p->y/256)-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Ijikebmp[2+i]);
			break;
		case MEDAMA:
			putbmp((int)(p->x/256)-3,(int)(p->y/256)-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Medamabmp[p->dir]);
			break;
		default:
			putbmp((int)(p->x/256)-3,(int)(p->y/256)-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[p->no*8+p->dir*2+i]);
	}
}
void blinkpowercookie(){
	//パワーえさの点滅
	if(gamecount & 0x10){
		set_palette(COLOR_POWERCOOKIE,0,50,50);
	}
	else{
		set_palette(COLOR_POWERCOOKIE,0,255,255);
	}
}
unsigned char getfruitno(unsigned char s){
	// s面のフルーツ番号を返す
	if(s<13) return fruit[s-1];
	else return fruit[12];
}
void putfruit(void){
	// フルーツ表示
	putbmp(FRUITX*8-2,FRUITY*8-3,12,14,Fruitbmp[fruitno]);
}
void putmapchar(unsigned char x,unsigned char y){
	//マップ上のコードに応じたものを表示
	unsigned char d;

	d=GETMAP(x,y);
	if(d==MAP_COOKIE) printchar(x,y,COLOR_COOKIE,CODE_COOKIE);
	else if(d==MAP_POWERCOOKIE) printchar(x,y,COLOR_POWERCOOKIE,CODE_POWERCOOKIE);
	else if(d==MAP_DOOR) printchar(x,y,COLOR_DOOR,CODE_DOOR);
	else if(d==MAP_WALL) printchar(x,y,COLOR_WALL,scenedata[y*MAPXSIZE+x]);
	else printchar(x,y,0,' ');
}
void setfruit(unsigned char f){
	// f 0:フルーツ削除&表示消去、1:フルーツ発生&表示
	if(f){
		SETMAP(FRUITX,FRUITY,MAP_FRUIT);
	}
	else{
		SETMAP(FRUITX,FRUITY,MAP_NONE);
		putmapchar(FRUITX-1,FRUITY-1);
		putmapchar(FRUITX  ,FRUITY-1);
		putmapchar(FRUITX+1,FRUITY-1);
		putmapchar(FRUITX-1,FRUITY  );
		putmapchar(FRUITX  ,FRUITY  );
		putmapchar(FRUITX+1,FRUITY  );
		putmapchar(FRUITX-1,FRUITY+1);
		putmapchar(FRUITX  ,FRUITY+1);
		putmapchar(FRUITX+1,FRUITY+1);
	}
}
void getfruit(){
	// フルーツ食べた
	score+=fruitscore[fruitno];
	fruitcount=0;
	setfruit(0);
	fruitsound=22000;
	fruitsoundcount=20;
	fruitscoretimer=TIMER_FRUITSCORE;
}

void displayscore(){
	if(score>highscore) highscore=score;
	printscore(21,3,7,highscore);
	printscore(21,7,7,score);
}
void displayplayers(){
	//プレイヤー残数表示
	unsigned char i;
	for(i=0;i<player && i<5;i++) putbmp(22*8+i*16,23*8,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[11]);
	for(;i<4;i++) clrbmpmn(22*8+i*16,23*8,XWIDTH_PACMAN,YWIDTH_PACMAN);
}
void displayfruits(){
	//画面右側に8ステージ分のフルーツを表示
	unsigned char i,no;
	for(i=0;i<8 && i<stage;i++){
		no=getfruitno(stage-i);
		clrbmpmn (22*8+(i%4)*16,15*8+(i/4)*16,12,14);
		putbmp(22*8+(i%4)*16,15*8+(i/4)*16,12,14,Fruitbmp[no]);
	}
}
void putmap(void){
	//通路、えさ、パワーえさの表示
	unsigned char *p,*mapp;
	int i,j;
	clearscreen();
	p=scenedata;
	mapp=map;
	cookie=0;
	for(i=0;i<MAPYSIZE;i++){
		for(j=0;j<MAPXSIZE;j++){
			if(*p<=0x8d){
				*mapp=MAP_WALL; //壁
				printchar(j,i,COLOR_WALL,*p);
			}
			else if(*p==CODE_DOOR){
				*mapp=MAP_DOOR; //ドア
				printchar(j,i,COLOR_DOOR,CODE_DOOR);
			}
			else if(*p==CODE_COOKIE){
				*mapp=MAP_COOKIE; //えさ
				printchar(j,i,COLOR_COOKIE,CODE_COOKIE);
				cookie++;
			}
			else if(*p==CODE_POWERCOOKIE){
				*mapp=MAP_POWERCOOKIE; //パワーえさ
				printchar(j,i,COLOR_POWERCOOKIE,CODE_POWERCOOKIE);
				cookie++;
			}
			else *mapp=0;
			p++;
			mapp++;
		}
	}
	printstr(21,1,7,"HI-SCORE");
	printchar(27,3,7,'0');
	printstr(22,5,7,"1UP");
	printchar(27,7,7,'0');
	displayscore();
	displayplayers();
	displayfruits();
}
void initcharacter(_Character *p,unsigned char s,unsigned short x,unsigned short y,
					unsigned char dir,unsigned short speed,unsigned char ac,unsigned short modec){
	//キャラクター初期化
	//p:キャラクターのポインタ、s:モード（status）、（x,y):初期位置、dir:初期方向
	//speed:移動速度、ac:アニメーションカウンタ、modec：モードカウンター
	p->status=s;
	p->x=x;
	p->y=y;
	p->oldx=x;
	p->oldy=y;
	p->dir=dir;
	p->turn=0;
	p->speed=speed;
	p->animcount=ac;
	p->animcount0=ac;
	p->animvalue=0;
	p->modecount=modec;
}
void gameinit(void){
// ゲーム全体の初期化。起動後1回だけ
	set_palette(COLOR_GUZUTA,70,200,128); //グズタの色
	set_palette(COLOR_PINKY,180,230,128); //ピンキーの色
	set_palette(COLOR_WALL,250,20,40); //壁の色
	set_palette(COLOR_POWERCOOKIE,0,255,255); //パワーえさの色
	set_palette(COLOR_ORANGE,0,255,165); //みかんの色
	set_palette(COLOR_MONSTERNUDE,180,245,220); //裸のモンスターの色
	akabei.no=AKABEI;
	aosuke.no=AOSUKE;
	guzuta.no=GUZUTA;
	pinky.no=PINKY;
	highscore=read_recscore(0); //ブートローダ対応共通領域からレコードの1つ目の32ビットを読み出し
	score=0;
}
void gameinit2(void){
// ゲーム初期化。プレイボタンで毎回呼び出し
	score=0;
	player=3;
	upflag=0;//1000点越えでプレイヤー+1のチェック用
	stage=0;
	firekeyold=0;
}
void gameinit3(void){
// シーンクリアごとに呼び出し
	if(stage<STAGEMAX-1){
		ijiketime=ijike[stage];
		pacmanspeed=pacmansp[stage];
		monsterspeed=monstersp[stage];
		ijikespeed=ijikesp[stage];
		medamaspeed=medamasp[stage];
	}
	else{
		ijiketime=ijike[STAGEMAX-1];
		pacmanspeed=pacmansp[STAGEMAX-1];
		monsterspeed=monstersp[STAGEMAX-1];
		ijikespeed=ijikesp[STAGEMAX-1];
		medamaspeed=medamasp[STAGEMAX-1];
	}
	stage++;
	fruitflag1=1;//面前半のフルーツが残っている
	fruitflag2=1;//面後半のフルーツが残っている
	fruitno=getfruitno(stage);
	putmap();//通路等の表示
}
void gameinit4(void){
// パックマンがやられるごと、およびシーンクリアごとに呼び出し
	fruitcount=0;
	fruitscoretimer=0;
	huntedmonster=0;
	monsterhuntedtimer=0;
	monstersoundcount=0;
	cookiesoundcount=0;
	fruitsoundcount=0;
	over10000soundcount=0;
	setfruit(0);//フルーツ削除、表示消去
	initcharacter(&pacman,0,10*8*256,20*8*256,DIR_LEFT,pacmanspeed,5,0);
	initcharacter(&akabei,NAWABARI,MONSTERHOUSEX*8*256,(MONSTERHOUSEY-2)*8*256,DIR_LEFT,monsterspeed,0,550);
	initcharacter(&pinky ,NAWABARI,MONSTERHOUSEX*8*256,(MONSTERHOUSEY+1)*8*256,DIR_UP,monsterspeed,0,0);
	initcharacter(&aosuke,TAIKI2,(MONSTERHOUSEX-1)*8*256,(MONSTERHOUSEY+1)*8*256,DIR_DOWN,monsterspeed,0,375);
	initcharacter(&guzuta,TAIKI2,(MONSTERHOUSEX+1)*8*256,(MONSTERHOUSEY+1)*8*256,DIR_DOWN,monsterspeed,0,600);
}

void keycheck(){
	// ボタンチェックし、壁でなければパックマンの向き変更
	unsigned short k;
	unsigned char d;
	unsigned short x,y;

	x=pacman.x/256;
	y=pacman.y/256;
	k=~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
	if(k==KEYUP && (x%8)==0){	//上ボタン
		if(pacman.dir!=DIR_UP){
			if(y>=8){
				if(GETMAP(x/8,y/8-1)!=MAP_WALL) pacman.dir=DIR_UP;
			}
			else pacman.dir=DIR_UP;
		}
	}
	else if(k==KEYRIGHT && (y%8)==0){	//右ボタン
		if(pacman.dir!=DIR_RIGHT){
			if(x<=(MAPXSIZE-2)*8){
				if(GETMAP(x/8+1,y/8)!=MAP_WALL) pacman.dir=DIR_RIGHT;
			}
			else pacman.dir=DIR_RIGHT;
		}
	}
	else if(k==KEYDOWN && (x%8)==0){	//下ボタン
		if(pacman.dir!=DIR_DOWN){
			if(y<=(MAPYSIZE-2)*8){
				d=GETMAP(x/8,y/8+1);
				if(d!=MAP_WALL && d!=MAP_DOOR) pacman.dir=DIR_DOWN;
			}
			else pacman.dir=DIR_DOWN;
		}
	}
	else if(k==KEYLEFT && (y%8)==0){	//左ボタン
		if(pacman.dir!=DIR_LEFT){
			if(x>=8){
				if(GETMAP(x/8-1,y/8)!=MAP_WALL) pacman.dir=DIR_LEFT;
			}
			else pacman.dir=DIR_LEFT;
		}
	}
}
void pausecheck(void){
	//FIREボタンで一時停止
	//firekeyoldでキーリピートチェック
	unsigned short k;

	k=~KEYPORT & KEYFIRE;
	if(firekeyold==KEYFIRE || k!=KEYFIRE){
		firekeyold=k;
		return;
	}
	firekeyold=k;
	PR3=0; //サウンド停止
	while(1){
		//もう一度FIREボタンが押されるまでループ
		drawcount=0;
		while(drawcount==0) asm("wait");
		k=~KEYPORT & KEYFIRE;
		if(firekeyold!=KEYFIRE && k==KEYFIRE){
			firekeyold=k;
			return;
		}
		firekeyold=k;
	}
}

void movepacman(){
	//パックマン移動チェック
	unsigned char t;
	unsigned short x,y;

	if(monsterhuntedtimer>0) return; //モンスター捕獲中は停止
	x=pacman.x/256;
	y=pacman.y/256;
	switch(pacman.dir){
		case DIR_UP: //上
			if(y!=0){
				if((y%8)!=0 || GETMAP(x/8,y/8-1)!=MAP_WALL) pacman.y-=pacman.speed;
			}
			else pacman.y=(MAPYSIZE-1)*8*256;
			break;
		case DIR_RIGHT: //右
			if(x!=(MAPXSIZE-1)*8){
				if((x%8)!=0 || GETMAP(x/8+1,y/8)!=MAP_WALL) pacman.x+=pacman.speed;
			}
			else pacman.x=0;
			break;
		case DIR_DOWN: //下。モンスターハウスのドアも壁とみなす
			if(y!=(MAPYSIZE-1)*8){
				t=GETMAP(x/8,y/8+1);
				if((y%8)!=0 || t!=MAP_WALL && t!=MAP_DOOR) pacman.y+=pacman.speed;
			}
			else pacman.y=0;
			break;
		case DIR_LEFT: //左
			if(x!=0){
				if((x%8)!=0 || GETMAP(x/8-1,y/8)!=MAP_WALL) pacman.x-=pacman.speed;
			}
			else pacman.x=(MAPXSIZE-1)*8*256;
	}
}
void movemonster(_Character *p,unsigned short tx,unsigned short ty){
	//モンスターの移動方向決定し、移動させる
	//p:モンスターのポインタ、tx:目標x座標、ty:目標y座標
	unsigned char cUP,cRIGHT,cDOWN,cLEFT;
	unsigned char t;
	unsigned char olddir;
	unsigned short x,y,x1,y1,oldx,oldy;

	x=p->x/256;
	y=p->y/256;
	x1=x/8;
	y1=y/8;
	oldx=p->oldx;
	oldy=p->oldy;
	p->oldx=p->x;
	p->oldy=p->y;
	olddir=p->dir;

	switch(p->dir){
	//方向変更後、小数点以上移動するまで直進
	//また、直進中も小数点以下移動の場合は直進する
		case DIR_UP: //上
			if(p->turn && y==oldy/256){
				p->y-=p->speed;
				return;
			}
			p->turn=0;
			if(y==(unsigned short)((p->y-p->speed))/256){
				p->y-=p->speed;
				return;
			}
			break;
		case DIR_RIGHT: //右
			if(p->turn && x==oldx/256){
				p->x+=p->speed;
				return;
			}
			p->turn=0;
			if(x==(p->x+p->speed)/256){
				p->x+=p->speed;
				return;
			}
			break;
		case DIR_DOWN: //下
			if(p->turn && y==oldy/256){
				p->y+=p->speed;
				return;
			}
			p->turn=0;
			if(y==(p->y+p->speed)/256){
				p->y+=p->speed;
				return;
			}
			break;
		case DIR_LEFT: //左
			if(p->turn && x==oldx/256){
				p->x-=p->speed;
				return;
			}
			p->turn=0;
			if(x==(unsigned short)((p->x-p->speed))/256){
				p->x-=p->speed;
				return;
			}
	}

	//cUP、cRIGHT、cDOWN、cLEFT:それぞれの方向に通れる場合1、通れない場合0とする
	cUP=1;
	cRIGHT=1;
	cDOWN=1;
	cLEFT=1;
	if((x%8)!=0){
		cUP=0;
		cDOWN=0;
	}
	else if((y%8)==0){
		if(y1!=0 && GETMAP(x1,y1-1)==MAP_WALL) cUP=0;
		//目玉のとき以外は一方通行チェック
		else if(p->status!=MEDAMA &&
		  (x1==ONEWAY1X && y1==ONEWAY1Y+1 || x1==ONEWAY2X && y1==ONEWAY2Y+1 ||
		   x1==ONEWAY3X && y1==ONEWAY3Y+1 || x1==ONEWAY4X && y1==ONEWAY4Y+1)) cUP=0;
		if(y1!=MAPYSIZE-1){
			//目玉のとき以外はドアも壁とみなす
			t=GETMAP(x1,y1+1);
			if(t==MAP_WALL || (t==MAP_DOOR && p->status!=MEDAMA)) cDOWN=0;
		}
	}

	if((y%8)!=0){
		cRIGHT=0;
		cLEFT=0;
	}
	else if((x%8)==0){
		if(x1!=MAPXSIZE-1 && GETMAP(x1+1,y1)==MAP_WALL) cRIGHT=0;
		if(x1!=0 && GETMAP(x1-1,y1)==MAP_WALL) cLEFT=0;
	}

	tx>>=8;
	ty>>=8;
	if(p->status==TAIKI2){
		if(y==MONSTERHOUSEY*8) p->dir=DIR_DOWN;
		else if(y==(MONSTERHOUSEY+1)*8) p->dir=DIR_UP;
	}
	else switch(p->dir){
		case DIR_UP: //進行方向=上・・・優先順位　右→上→左
			if(x<tx && cRIGHT) p->dir=DIR_RIGHT;
			else if(y>ty && cUP) p->dir=DIR_UP;
			else if(x>tx && cLEFT) p->dir=DIR_LEFT;
			else if(cRIGHT) p->dir=DIR_RIGHT;
			else if(cUP) p->dir=DIR_UP;
			else p->dir=DIR_LEFT;
			break;
		case DIR_RIGHT: //進行方向=右・・・優先順位　下→右→上
			if(y<ty && cDOWN) p->dir=DIR_DOWN;
			else if(x<tx && cRIGHT) p->dir=DIR_RIGHT;
			else if(y>ty && cUP) p->dir=DIR_UP;
			else if(cDOWN) p->dir=DIR_DOWN;
			else if(cRIGHT) p->dir=DIR_RIGHT;
			else p->dir=DIR_UP;
			break;
		case DIR_DOWN: //進行方向=下・・・優先順位　左→下→右
			if(x>tx && cLEFT) p->dir=DIR_LEFT;
			else if(y<ty && cDOWN) p->dir=DIR_DOWN;
			else if(x<tx && cRIGHT) p->dir=DIR_RIGHT;
			else if(cLEFT) p->dir=DIR_LEFT;
			else if(cDOWN) p->dir=DIR_DOWN;
			else p->dir=DIR_RIGHT;
			break;
		case DIR_LEFT: //進行方向=左・・・優先順位　上→左→下
			if(y>ty && cUP) p->dir=DIR_UP;
			else if(x>tx && cLEFT) p->dir=DIR_LEFT;
			else if(y<ty && cDOWN) p->dir=DIR_DOWN;
			else if(cUP) p->dir=DIR_UP;
			else if(cLEFT) p->dir=DIR_LEFT;
			else p->dir=DIR_DOWN;
	}
	if(p->dir != olddir) p->turn=1;
	switch(p->dir){
		case DIR_UP: //上
			if(y!=0) p->y-=p->speed;
			else p->y=(MAPYSIZE-1)*8*256;
			break;
		case DIR_RIGHT: //右
			if(x!=(MAPXSIZE-1)*8){
				//ワープゾーンでスピードを落とす
				if(y1==WARPY && (x1<WARPX1 || x1>=WARPX2) && p->status!=MEDAMA) p->x+=p->speed/2;
				else p->x+=p->speed;
			}
			else p->x=0;
			break;
		case DIR_DOWN: //下
			if(y!=(MAPYSIZE-1)*8) p->y+=p->speed;
			else p->y=0;
			break;
		case DIR_LEFT: //左
			if(x!=0){
				//ワープゾーンでスピードを落とす
				if(y1==WARPY && (x1<WARPX1 || x1>=WARPX2) && p->status!=MEDAMA) p->x-=p->speed/2;
				else p->x-=p->speed;
			}
			else p->x=(MAPXSIZE-1)*8*256;
	}
}

void moveakabei(){
	//アカベイ移動
	unsigned short targetx,targety;

	if(monsterhuntedtimer>0 && akabei.status!=MEDAMA) return; //誰かが捕獲されているとき目玉以外は停止
	switch (akabei.status){
		case NAWABARI:
			akabei.modecount--;
			if(akabei.modecount==0){
				akabei.modecount=TIMER_OIKAKE;
				akabei.status=OIKAKE;
				akabei.dir=(akabei.dir+2)&3; //向き反転
				akabei.turn=0;
				//他のモンスターも同時に変更する
				if(pinky.status==NAWABARI){
					pinky.status=OIKAKE;
					pinky.dir=(pinky.dir+2)&3;
					pinky.turn=0;
				}
				if(aosuke.status==NAWABARI){
					aosuke.status=OIKAKE;
					aosuke.dir=(aosuke.dir+2)&3;
					aosuke.turn=0;
				}
				if(guzuta.status==NAWABARI){
					guzuta.status=OIKAKE;
					guzuta.dir=(guzuta.dir+2)&3;
					guzuta.turn=0;
				}
			}
			break;
		case OIKAKE:
			akabei.modecount--;
			if(akabei.modecount==0){
				akabei.modecount=TIMER_NAWABARI;
				akabei.status=NAWABARI;
				akabei.dir=(akabei.dir+2)&3; //向き反転
				akabei.turn=0;
				if(pinky.status==OIKAKE){
					pinky.status=NAWABARI;
					pinky.dir=(pinky.dir+2)&3;
					pinky.turn=0;
				}
				if(aosuke.status==OIKAKE){
					aosuke.status=NAWABARI;
					aosuke.dir=(aosuke.dir+2)&3;
					aosuke.turn=0;
				}
				if(guzuta.status==OIKAKE){
					guzuta.status=NAWABARI;
					guzuta.dir=(guzuta.dir+2)&3;
					guzuta.turn=0;
				}
			}
			break;
		case IJIKE:
			akabei.modecount--;
			if(akabei.modecount==0){
				akabei.status=OIKAKE;
				akabei.modecount=TIMER_OIKAKE;
				akabei.speed=monsterspeed;
			}
			break;
		case MEDAMA:
			if((akabei.x/256)==(MONSTERHOUSEX*8) && (akabei.y/256)==(MONSTERHOUSEY*8)){ //モンスターハウス到着
				akabei.status=TAIKI2;
				akabei.modecount=1;
				akabei.dir=DIR_UP;
				akabei.speed=monsterspeed;
			}
			break;
		case TAIKI:
			if((akabei.x/256)==(MONSTERHOUSEX*8) && (akabei.y/256)==((MONSTERHOUSEY-2)*8)){ //モンスターハウスから出た
				akabei.status=OIKAKE;
				akabei.modecount=TIMER_OIKAKE;
				akabei.dir=DIR_LEFT;
			}
			break;
		case TAIKI2:
			akabei.modecount--;
			if(akabei.modecount==0){
				akabei.status=TAIKI;
			}
	}
	switch (akabei.status){
		case NAWABARI:
		case IJIKE:
			targetx=NAWABARIAKABEIX*8*256;
			targety=NAWABARIAKABEIY*8*256;
			break;
		case OIKAKE:
			targetx=pacman.x;
			targety=pacman.y;
			break;
		case MEDAMA:
			targetx=MONSTERHOUSEX*8*256;
			targety=MONSTERHOUSEY*8*256;
			break;
		case TAIKI:
			targetx=MONSTERHOUSEX*8*256;
			targety=(MONSTERHOUSEY-2)*8*256;
	}
	movemonster(&akabei,targetx,targety);
}

void movepinky(){
	//ピンキー移動
	unsigned short targetx,targety;

	if(monsterhuntedtimer>0 && pinky.status!=MEDAMA) return; //誰かが捕獲されているとき目玉以外は停止
	switch (pinky.status){
		case NAWABARI:
		case OIKAKE:
			break;
		case IJIKE:
			pinky.modecount--;
			if(pinky.modecount==0){
				pinky.status=OIKAKE;
				pinky.speed=monsterspeed;
			}
			break;
		case MEDAMA:
			if((pinky.x/256)==(MONSTERHOUSEX*8) && (pinky.y/256)==(MONSTERHOUSEY*8)){ //モンスターハウス到着
				pinky.status=TAIKI2;
				pinky.modecount=1;
				pinky.dir=DIR_UP;
				pinky.speed=monsterspeed;
			}
			else pinky.speed=medamaspeed;
			break;
		case TAIKI:
			if((pinky.x/256)==(MONSTERHOUSEX*8) && (pinky.y/256)==((MONSTERHOUSEY-2)*8)){ //モンスターハウスから出た
				pinky.status=OIKAKE;
				pinky.dir=DIR_LEFT;
			}
			break;
		case TAIKI2:
			pinky.modecount--;
			if(pinky.modecount==0){
				pinky.status=TAIKI;
			}
	}
	switch (pinky.status){
		case NAWABARI:
			targetx=NAWABARIPINKYX*8*256;
			targety=NAWABARIPINKYY*8*256;
			break;
		case IJIKE:
			targetx=MONSTERHOUSEX*8*256;
			targety=MONSTERHOUSEY*8*256;
			break;
		case OIKAKE: //パックマンの進行方向+3をターゲット座標とする
			if(pacman.dir==DIR_UP){
				targetx=pacman.x;
				if(pacman.y>=4*8*256) targety=pacman.y-3*8*256;
				else if(pacman.y/256/8==0) targety=(MAPYSIZE-3)*8*256;
				else targety=1*8*256;
			}
			else if(pacman.dir==DIR_RIGHT){
				targety=pacman.y;
				if(pacman.x<=(MAPXSIZE-5)*8*256) targetx=pacman.x+3*8*256;
				else if(pacman.x/256/8==(MAPXSIZE-1)*8) targetx=2*8*256;
				else targetx=(MAPXSIZE-2)*8*256;
			}
			else if(pacman.dir==DIR_DOWN){
				targetx=pacman.x;
				if(pacman.y<=(MAPYSIZE-5)*8*256) targety=pacman.y+3*8*256;
				else if(pacman.y/256/8==(MAPYSIZE-1)*8) targety=2*8*256;
				else targety=(MAPYSIZE-2)*8*256;
			}
			else if(pacman.dir==DIR_LEFT){
				targety=pacman.y;
				if(pacman.x>=4*8*256) targetx=pacman.x-3*8*256;
				else if(pacman.x/256/8==0) targetx=(MAPXSIZE-3)*8*256;
				else targetx=1*8*256;
			}
			break;
		case MEDAMA:
			targetx=MONSTERHOUSEX*8*256;
			targety=MONSTERHOUSEY*8*256;
			break;
		case TAIKI:
			targetx=MONSTERHOUSEX*8*256;
			targety=(MONSTERHOUSEY-2)*8*256;
	}
	movemonster(&pinky,targetx,targety);
}

void moveaosuke(){
	//アオスケ移動
	unsigned short targetx,targety;

	if(monsterhuntedtimer>0 && aosuke.status!=MEDAMA) return; //誰かが捕獲されているとき目玉以外は停止
	switch (aosuke.status){
		case NAWABARI:
		case OIKAKE:
			break;
		case IJIKE:
			aosuke.modecount--;
			if(aosuke.modecount==0){
				aosuke.status=OIKAKE;
				aosuke.speed=monsterspeed;
			}
			break;
		case MEDAMA:
			if((aosuke.x/256)==(MONSTERHOUSEX*8) && (aosuke.y/256)==(MONSTERHOUSEY*8)){ //モンスターハウス到着
				aosuke.status=TAIKI2;
				aosuke.modecount=1;
				aosuke.dir=DIR_UP;
				aosuke.speed=monsterspeed;
			}
			break;
		case TAIKI:
			if((aosuke.x/256)==(MONSTERHOUSEX*8) && (aosuke.y/256)==((MONSTERHOUSEY-2)*8)){ //モンスターハウスから出た
				aosuke.status=OIKAKE;
				aosuke.dir=DIR_LEFT;
			}
			break;
		case TAIKI2:
			aosuke.modecount--;
			if(aosuke.modecount==0){
				aosuke.status=TAIKI;
			}
	}
	switch (aosuke.status){
		case NAWABARI:
			targetx=NAWABARIAOSUKEX*8*256;
			targety=NAWABARIAOSUKEY*8*256;
			break;
		case IJIKE:
			targetx=MONSTERHOUSEX*8*256;
			targety=MONSTERHOUSEY*8*256;
			break;
		case OIKAKE: //パックマン中心にアカベイと点対称の位置をターゲット座標とする
			targetx=pacman.x/2-akabei.x/4;
			targety=pacman.y/2-akabei.y/4;
			if(targetx>=32768) targetx=0;
			else if(targetx>=16384) targetx=16383;
			if(targety>=32768) targety=0;
			else if(targety>=16384) targety=16383;
			targetx<<=2;
			targety<<=2;
			break;
		case MEDAMA:
			targetx=MONSTERHOUSEX*8*256;
			targety=MONSTERHOUSEY*8*256;
			break;
		case TAIKI:
			targetx=MONSTERHOUSEX*8*256;
			targety=(MONSTERHOUSEY-2)*8*256;
	}
	movemonster(&aosuke,targetx,targety);
}

void moveguzuta(){
	//グズタ移動
	unsigned short targetx,targety;
	short dx,dy;

	if(monsterhuntedtimer>0 && guzuta.status!=MEDAMA) return; //誰かが捕獲されているとき目玉以外は停止
	switch (guzuta.status){
		case NAWABARI:
		case OIKAKE:
			break;
		case IJIKE:
			guzuta.modecount--;
			if(guzuta.modecount==0){
				guzuta.status=OIKAKE;
				guzuta.speed=monsterspeed;
			}
			break;
		case MEDAMA:
			if((guzuta.x/256)==(MONSTERHOUSEX*8) && (guzuta.y/256)==(MONSTERHOUSEY*8)){ //モンスターハウス到着
				guzuta.status=TAIKI2;
				guzuta.modecount=1;
				guzuta.dir=DIR_UP;
				guzuta.speed=monsterspeed;
			}
			break;
		case TAIKI:
			if((guzuta.x/256)==(MONSTERHOUSEX*8) && (guzuta.y/256)==((MONSTERHOUSEY-2)*8)){ //モンスターハウスから出た
				guzuta.status=OIKAKE;
				guzuta.dir=DIR_LEFT;
			}
			break;
		case TAIKI2:
			guzuta.modecount--;
			if(guzuta.modecount==0){
				guzuta.status=TAIKI;
			}
	}
	switch (guzuta.status){
		case NAWABARI:
			targetx=NAWABARIGUZUTAX*8*256;
			targety=NAWABARIGUZUTAY*8*256;
			break;
		case IJIKE:
			targetx=MONSTERHOUSEX*8*256;
			targety=MONSTERHOUSEY*8*256;
			break;
		case OIKAKE: //パックマンから遠くにいるときは近づいてくるが、近づくと別を目指す
			dx=(short)(guzuta.x/256/8)-(short)(pacman.x/256/8);
			dy=(short)(guzuta.y/256/8)-(short)(pacman.y/256/8);
			if(dx*dx+dy*dy>10){
				targetx=pacman.x;
				targety=pacman.y;
			}
			else{
				targetx=NAWABARIGUZUTAX*8*256;
				targety=NAWABARIGUZUTAY*8*256;
			}
			break;
		case MEDAMA:
			targetx=MONSTERHOUSEX*8*256;
			targety=MONSTERHOUSEY*8*256;
			break;
		case TAIKI:
			targetx=MONSTERHOUSEX*8*256;
			targety=(MONSTERHOUSEY-2)*8*256;
	}
	movemonster(&guzuta,targetx,targety);
}

void sound(void){
	//効果音出力
	//60分の1秒ごとに呼び出し
	//後ろのほうで処理するものほど優先的に鳴らす
	unsigned short pr;//タイマーカウンター値
	unsigned short monsterspeed2;
	pr=0;
	monsterspeed2=2048/monsterspeed;

//モンスターの音程
	if(akabei.status==MEDAMA || pinky.status==MEDAMA || aosuke.status==MEDAMA || guzuta.status==MEDAMA){
	//目玉・・・prは2200～3000で連続変化、スピードが速いと変化も速い
		if(monstersoundcount==0){
			monstersound=2200;
			monstersounddif=800/monsterspeed2;
		}
		else monstersound+=monstersounddif;
		pr=monstersound;
		monstersoundcount++;
		if(monstersoundcount>monsterspeed2) monstersoundcount=0;
	}
	else if(akabei.status==IJIKE || pinky.status==IJIKE || aosuke.status==IJIKE || guzuta.status==IJIKE){
	//イジケ・・・prは7600～3800で連続変化、スピードが速いと変化も速い
		if(monstersoundcount==0){
			monstersounddif=7600/monsterspeed2;
			monstersound=3800+(monsterspeed2>>1)*monstersounddif;
		}
		else monstersound-=monstersounddif;
		pr=monstersound;
		monstersoundcount++;
		if(monstersoundcount*2>monsterspeed2) monstersoundcount=0;
	}
	else{
	//通常
		if(monstersoundcount==0){
			monstersound=2500+monsterspeed2*60;
			monstersounddif=monstersound/monsterspeed2;
		}
		else if(monstersoundcount<monsterspeed2) monstersound+=monstersounddif;
		else monstersound-=monstersounddif;
		pr=monstersound;
		monstersoundcount++;
		if(monstersoundcount>monsterspeed2*2) monstersoundcount=0;
	}

//えさを食べる音
	if(cookiesoundcount!=0){
		if((cookie&1)==0){
			if(cookiesoundcount>3) pr=32000;
			else pr=16000;
		}
		else{
			if(cookiesoundcount>3) pr=16000;
			else pr=32000;
		}
		cookiesoundcount--;
	}
//フルーツを食べる音0.3秒
	if(fruitsoundcount!=0){
		pr=fruitsound;
		fruitsound-=670;
		fruitsoundcount--;
	}
//イジケを食べる音60/60秒、8回でF→上のA、2回に1回音を鳴らし音程変更
	if(monsterhuntedtimer>0){
		if(monsterhuntedtimer&1){
			pr=monsterhuntedsound;
			monsterhuntedsound-=50;
		}
	}
//10000点越え、パックマン+1 Gを8回鳴らす
	if(over10000soundcount!=0){
		if((over10000soundcount&7)<6) pr=4565;
		over10000soundcount--;
	}

	PR3=pr; //実際に周期変更
}

void erasechars2(_Character *p){
// 1キャラクターを表示した場所の周囲含めて書き直す
	unsigned short x,y;
	unsigned char x2,y2;

	x=p->x/256;
	y=p->y/256;
	x2=(unsigned char)(x/8);
	y2=(unsigned char)(y/8);
	if((y%8)<3 && y2>0){
		if((x%8)<4 && x2>0) putmapchar(x2-1,y2-1);
		putmapchar(x2,y2-1);
		if(x2<MAPXSIZE-1) putmapchar(x2+1,y2-1);
		if((x%8)>=5 && x2<MAPXSIZE-2) putmapchar(x2+2,y2-1);
	}
	if((x%8)<4 && x2>0) putmapchar(x2-1,y2);
	putmapchar(x2,y2);
	if(x2<MAPXSIZE-1) putmapchar(x2+1,y2);
	if((x%8)>=5 && x2<MAPXSIZE-2) putmapchar(x2+2,y2);
	if(y2<MAPYSIZE-1){
		if((x%8)<4 && x2>0) putmapchar(x2-1,y2+1);
		putmapchar(x2,y2+1);
		if(x2<MAPXSIZE-1) putmapchar(x2+1,y2+1);
		if((x%8)>=5 && x2<MAPXSIZE-2) putmapchar(x2+2,y2+1);
	}
	if((y%8)>=6 && y2<MAPYSIZE-2){
		if((x%8)<4 && x2>0) putmapchar(x2-1,y2+2);
		putmapchar(x2,y2+2);
		if(x2<MAPXSIZE-1) putmapchar(x2+1,y2+2);
		if((x%8)>=5 && x2<MAPXSIZE-2) putmapchar(x2+2,y2+2);
	}
}
void erasechars(){
	// 表示物の消去（元の表示物に戻す）
	erasechars2(&pacman);
	erasechars2(&akabei);
	erasechars2(&pinky);
	erasechars2(&aosuke);
	erasechars2(&guzuta);
}

void putpowercookies(){
	// パワーえさのカラーパレット変更時の再表示用
	putmapchar(POWERCOOKIEX1,POWERCOOKIEY1);
	putmapchar(POWERCOOKIEX2,POWERCOOKIEY2);
	putmapchar(POWERCOOKIEX3,POWERCOOKIEY3);
	putmapchar(POWERCOOKIEX4,POWERCOOKIEY4);
}
void displaychars(){
	// フルーツ、イジケ、パックマン、イジケ以外のモンスターの順に表示
	blinkpowercookie(); //パワーえさの点滅
	putpowercookies();
	if(fruitcount>0) putfruit();
	else if(fruitscoretimer>0) putbmp(FRUITX*8-4,FRUITY*8,XWIDTH_SCORE,YWIDTH_SCORE,Scorebmp[4+fruitno]);
	if(akabei.status==IJIKE) putmonster(&akabei);
	if(aosuke.status==IJIKE) putmonster(&aosuke);
	if(guzuta.status==IJIKE) putmonster(&guzuta);
	if(pinky.status==IJIKE) putmonster(&pinky);
	if(monsterhuntedtimer!=0)//イジケを食べたときの得点表示
		putbmp((int)(pacman.x/256)-4,(int)(pacman.y/256),XWIDTH_SCORE,YWIDTH_SCORE,Scorebmp[huntedmonster-1]);
	else putpacman();
	if(akabei.status!=IJIKE) putmonster(&akabei);
	if(aosuke.status!=IJIKE) putmonster(&aosuke);
	if(guzuta.status!=IJIKE) putmonster(&guzuta);
	if(pinky.status!=IJIKE) putmonster(&pinky);
}
void fruitcheck(){
	//フルーツ出現、消滅チェック、
	if(fruitflag1 && cookie==FRUITTIME1){
		fruitflag1=0;
		setfruit(1);
		fruitcount=TIMER_FRUIT;
	}
	else if(fruitflag2 && cookie==FRUITTIME2){
		fruitflag2=0;
		setfruit(1);
		fruitcount=TIMER_FRUIT;
	}
	else if(fruitcount>0){
		fruitcount--;
		if(fruitcount==0) setfruit(0);//時間切れでフルーツ消去
	}
	else if(fruitscoretimer>0){ //フルーツを食べたときのスコア消去
		fruitscoretimer--;
		if(fruitscoretimer==0){
			putmapchar(FRUITX-1,FRUITY);
			putmapchar(FRUITX  ,FRUITY);
			putmapchar(FRUITX+1,FRUITY);
		}
	}
}

void movechars(){
	//各キャラクターの移動
	movepacman();
	moveakabei();
	movepinky();
	moveaosuke();
	moveguzuta();
	if(monsterhuntedtimer>0) monsterhuntedtimer--;
}

void setmonsterijike(_Character *p){
	// モンスターをいじけ状態にする
	if(p->status==MEDAMA) return;
//	if(p->status==TAIKI || p->status==TAIKI2) return;
	p->dir=(p->dir+2)&3; //向き反転
	p->turn=0;
	if(ijiketime!=0){
		p->status=IJIKE;
		p->modecount=ijiketime;
		p->speed=ijikespeed;
	}
}

unsigned char monsterhuntcheck(_Character *p){
	//　モンスター捕獲チェック
	//　戻り値　0:捕獲なし、1:イジケを捕獲、2:パックマンがモンスターにやられた
#define HANTEI 7 // 座標がどれだけ重なった時に捕獲と判定するかの捕獲判定値

	short dx,dy;

	if(p->status==MEDAMA) return 0;
	dx=(short)(p->x/256)-(short)(pacman.x/256);
	dy=(short)(p->y/256)-(short)(pacman.y/256);
	if(dx!=0 && dy!=0) return 0;
	if(dx<=-(XWIDTH_MONSTER-HANTEI) || dx>=XWIDTH_PACMAN-HANTEI ||
	   dy<=-(YWIDTH_MONSTER-HANTEI) || dy>=YWIDTH_PACMAN-HANTEI) return 0;
	if(p->status==IJIKE){
		p->status=MEDAMA;
		p->x&=0xfeff; //目玉の移動速度が1ドット飛ばしのため、強制的に偶数座標にする
		p->y&=0xfeff; //
		huntedmonster++;
		p->speed=medamaspeed;
		score+=(1<<huntedmonster)*10;
		monsterhuntedtimer=TIMER_HUNTEDSTOP;//停止時間
		monsterhuntedsound=5100;
		return 1;
	}
	gamestatus=2;
	return 2;
}
void huntedcheck(void){
	//　各種の捕獲、衝突チェック

	unsigned char d,x,y;

	if(monsterhuntedtimer>0) return; //目玉以外停止中
	x=pacman.x/256;
	y=pacman.y/256;
	if((x%8)==4 || (y%8)==4){
		x/=8;
		y/=8;
		if(pacman.dir==DIR_RIGHT) x++;
		else if(pacman.dir==DIR_DOWN) y++;
		d=GETMAP(x,y);
		if(d==MAP_COOKIE){
			//えさ食べた
			SETMAP(x,y,MAP_NONE);
			score++;
			cookie--;
			cookiesoundcount=4;
		}
		else if(d==MAP_POWERCOOKIE){
			//パワーえさ食べた
			SETMAP(x,y,MAP_NONE);
			score+=5;
			cookie--;
			cookiesoundcount=4;
			huntedmonster=0;
			setmonsterijike(&akabei);
			setmonsterijike(&pinky);
			setmonsterijike(&aosuke);
			setmonsterijike(&guzuta);
		}
		else if(d==MAP_FRUIT){
			//フルーツ食べた
			getfruit();
		}
		if(cookie==0){
			//ステージクリア
			gamestatus=3;
			return;
		}
	}
	if(akabei.status==IJIKE && monsterhuntcheck(&akabei)) return;
	if(pinky.status==IJIKE && monsterhuntcheck(&pinky)) return;
	if(aosuke.status==IJIKE && monsterhuntcheck(&aosuke)) return;
	if(guzuta.status==IJIKE && monsterhuntcheck(&guzuta)) return;
	if(upflag==0 && score>=1000){
		upflag=1;
		player++;
		displayplayers();
		over10000soundcount=63;
	}
	if(akabei.status!=IJIKE && monsterhuntcheck(&akabei)) return;
	if(pinky.status!=IJIKE && monsterhuntcheck(&pinky)) return;
	if(aosuke.status!=IJIKE && monsterhuntcheck(&aosuke)) return;
	if(guzuta.status!=IJIKE && monsterhuntcheck(&guzuta)) return;
}

// コーヒーブレイク1～3
void coffeebreak1(void){
	unsigned char i;
	unsigned char a1,ac1,av1;
	unsigned char a2,ac2;
	int pacx,akax,pacspeed,akaspeed;
	clearscreen();
	pacx=255*256;
	akax=300*256;
	ac1=5;
	av1=0;
	a2=0;
	ac2=4;
	pacspeed=-290*256/245;
	akaspeed=-320*256/245;
	startmusic(musicdata2);
	while(akax>-20*256){
		ac1--;
		if(ac1==0){
			ac1=5;
			av1++;
			if(av1==6) av1=0;
		}
		if(av1==0) a1=0;
		else if(av1<=3) a1=9+av1;
		else a1=9+6-av1;
		ac2--;
		if(ac2==0){
			ac2=4;
			a2^=1;
		}
		putbmp(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[a1]);
		putbmp(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[6+a2]);
		playmusic60thsec();
		clrbmpmn(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN);
		clrbmpmn(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER);
		pacx+=pacspeed;
		akax+=akaspeed;
	}

	for(i=0;i<20;i++){
		playmusic60thsec();
	}

	pacx=-75*256;
	akax=-15*256;
	ac1=5;
	av1=0;
	a2=0;
	ac2=4;
	pacspeed=330*256/225;
	akaspeed=300*256/225;
	while(pacx<260*256){
		ac1--;
		if(ac1==0){
			ac1=5;
			av1++;
			if(av1==6) av1=0;
		}
		if(av1<=3) a1=av1;
		else a1=6-av1;
		ac2--;
		if(ac2==0){
			ac2=4;
			a2^=1;
		}
		putbmp(pacx/256,83,31,31,Bigpacbmp[a1]);
		putbmp(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER,Ijikebmp[a2]);
		playmusic60thsec();
		clrbmpmn(pacx/256,83,31,31);
		clrbmpmn(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER);
		pacx+=pacspeed;
		akax+=akaspeed;
	}
	for(i=0;i<30;i++){
		playmusic60thsec();
	}
}
void coffeebreak2(void){
	unsigned char i;
	unsigned char a1,ac1,av1;
	unsigned char a2,ac2;
	int pacx,akax,pacspeed,akaspeed;
	clearscreen();
	pacx=255*256;
	akax=300*256;
	ac1=5;
	av1=0;
	a2=0;
	ac2=4;
	pacspeed=-290*256/245;
	akaspeed=-320*256/245;
	startmusic(musicdata2+82);
	while(akax>110*256){
		ac1--;
		if(ac1==0){
			ac1=5;
			av1++;
			if(av1==6) av1=0;
		}
		if(av1==0) a1=0;
		else if(av1<=3) a1=9+av1;
		else a1=9+6-av1;
		ac2--;
		if(ac2==0){
			ac2=4;
			a2^=1;
		}
		putbmp(122,111,1,4,Pinbmp);
		putbmp(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[a1]);
		putbmp(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[6+a2]);
		playmusic60thsec();
		clrbmpmn(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN);
		clrbmpmn(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER);
		pacx+=pacspeed;
		akax+=akaspeed;
	}
	akaspeed=-9*256/80;
	while(akax>100*256){
		ac1--;
		if(ac1==0){
			ac1=5;
			av1++;
			if(av1==6) av1=0;
		}
		if(av1==0) a1=0;
		else if(av1<=3) a1=9+av1;
		else a1=9+6-av1;
		ac2--;
		if(ac2==0){
			ac2=4;
			a2^=1;
		}
		putbmp(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[a1]);
		putbmp(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[6+a2]);
		playmusic60thsec();
		clrbmpmn(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN);
		clrbmpmn(akax/256,101,XWIDTH_MONSTER-1,YWIDTH_MONSTER);
		clrbmpmn(akax/256+XWIDTH_MONSTER-1,101,1,YWIDTH_MONSTER-4);
		pacx+=pacspeed;
		akax+=akaspeed;
	}
	putbmp(akax/256,101,22,13,Yabukebmp[0]);
	for(i=0;i<20;i++){
		playmusic60thsec();
	}
	putbmp(akax/256,101,22,13,Yabukebmp[1]);
	for(i=0;i<60;i++){
		playmusic60thsec();
	}
}
void coffeebreak3(void){
	unsigned char i;
	unsigned char a1,ac1,av1;
	unsigned char a2,ac2;
	int pacx,akax,pacspeed,akaspeed;
	clearscreen();
	pacx=255*256;
	akax=300*256;
	ac1=5;
	av1=0;
	a2=0;
	ac2=4;
	pacspeed=-290*256/245;
	akaspeed=-320*256/245;
	startmusic(musicdata2);
	while(akax>-20*256){
		ac1--;
		if(ac1==0){
			ac1=5;
			av1++;
			if(av1==6) av1=0;
		}
		if(av1==0) a1=0;
		else if(av1<=3) a1=9+av1;
		else a1=9+6-av1;
		ac2--;
		if(ac2==0){
			ac2=4;
			a2^=1;
		}
		putbmp(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[a1]);
		putbmp(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER,Yabuke2bmp[a2]);
		playmusic60thsec();
		clrbmpmn(pacx/256,100,XWIDTH_PACMAN,YWIDTH_PACMAN);
		clrbmpmn(akax/256,101,XWIDTH_MONSTER,YWIDTH_MONSTER);
		pacx+=pacspeed;
		akax+=akaspeed;
	}

	for(i=0;i<20;i++){
		playmusic60thsec();
	}

	akax=-25*256;
	a2=0;
	ac2=4;
	akaspeed=280*256/225;
	while(akax<260*256){
		ac2--;
		if(ac2==0){
			ac2=4;
			a2^=1;
		}
		putbmp(akax/256,101,22,13,Hadakabmp[a2]);
		playmusic60thsec();
		clrbmpmn(akax/256,101,22,13);
		akax+=akaspeed;
	}
	for(i=0;i<30;i++){
		playmusic60thsec();
	}
}
void gamestart(void){
	unsigned int i;
	printstr(8,15,6,"READY!");
	if(gamestatus==0){ //ゲーム開始時
		startmusic(musicdata1);//ゲームスタートの音楽開始
		for(i=0;i<120;i++){ //まず2秒間演奏
			playmusic60thsec();
		}
		//キャラクター表示して最後まで演奏
		player--;
		displaychars();
		displayplayers();
		while(music.stop==0){
			playmusic60thsec();
		}
	}
	else{
		displaychars();
		displayplayers();
		wait60thsec(120); //2秒ウェイト
	}
	printstr(8,15,0,"      "); //READY消去
}
void deadanim(void){
	//パックマンがやられたときのアニメーション＆サウンド
	unsigned char i,j;
	wait60thsec(120); //2秒ウェイト
	erasechars();
	if(fruitscoretimer>0){ //フルーツを食べたときのスコア消去
		putmapchar(FRUITX-1,FRUITY);
		putmapchar(FRUITX  ,FRUITY);
		putmapchar(FRUITX+1,FRUITY);
	}

	for(i=0;i<9;i++){
		erasechars2(&pacman);
		putbmp(pacman.x/256-3,pacman.y/256-3,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmandeadbmp[i]);
		if(i>1 && i<8){
			for(j=0;j<13;j++){
				PR3=2400+(i-2)*320+(2400+(i-2)*320)*(12-j)/12;
				wait60thsec(1); // 1/60秒ウェイト
	 		}
	 	}
	 	else if(i==8){
			for(j=0;j<13;j++){
				PR3=1700+1700*(12-j)/12;
	 			wait60thsec(1); // 1/60秒ウェイト
	 		}
	 	}
	}
	erasechars2(&pacman);
	for(j=0;j<13;j++){
		PR3=1700+1700*(12-j)/12;
		wait60thsec(1); // 1/60秒ウェイト
	}
	PR3=0;//サウンド停止
}
void putwall(void){
	//壁点滅の時の再表示
	unsigned char *p;
	int i,j;
	p=scenedata;
	for(i=0;i<MAPYSIZE;i++){
		for(j=0;j<MAPXSIZE;j++){
			if(*p<=0x8d){
				printchar(j,i,COLOR_WALL,*p);
			}
			p++;
		}
	}
}
void stageclear(void){
	//面クリアしたときのアニメーション
	int i;
	pacman.animvalue=0; //強制的に口を閉じたものを表示させる
	pacman.animcount=pacman.animcount0;
	putpacman();
 	wait60thsec(120); //2秒ウェイト
	erasechars();
	if(fruitscoretimer>0){ //フルーツを食べたときのスコア消去
		putmapchar(FRUITX-1,FRUITY);
		putmapchar(FRUITX  ,FRUITY);
		putmapchar(FRUITX+1,FRUITY);
	}
	putpacman();
	for(i=1;i<=6;i++){
 		wait60thsec(10); // 10/60秒ウェイト
		set_palette(COLOR_WALL,255,255,255); //壁の色を白く
		putwall();
		putpacman();
 		wait60thsec(10); // 10/60秒ウェイト
		set_palette(COLOR_WALL,250,20,40); //壁の色を戻す
		putwall();
		putpacman();
	}
	switch(stage){ //コーヒーブレイクチェック
		case 2:
			coffeebreak1();
			break;
		case 5:
			coffeebreak2();
			break;
		case 9:
		case 13:
		case 17:
			coffeebreak3();
	}
}

void gameover(){
	//ゲームオーバー時
	printstr(6,10,2,"GAME OVER");
	wait60thsec(180); // 3秒ウェイト
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
	int i;
	unsigned char a1,ac1,av1;
	unsigned char a2,ac2;
	int pacx,akax,pacspeed,akaspeed;

	while(1){
		clearscreen();

		printstr(5,2,2,"1UP");
		printstr(18,2,2,"HI-SCORE");
		printscore(4,3,7,score);
		printchar(10,3,7,'0');
		printscore(19,3,7,highscore);
		printchar(25,3,7,'0');

		//ロゴ表示
		putbmp(63,80,114,36,Titlelogobmp[0]);

		printstr( 9,20,7,"FOR PIC32MX");
		printstr(10,22,7,"BY KENKEN");
		printstr( 6,25,4,"PUSH START BUTTON");

		if(startkeycheck(600)) return;//10秒ウェイト

		clearscreen();
		printstr(5,2,2,"1UP");
		printstr(18,2,2,"HI-SCORE");
		printscore(4,3,7,score);
		printchar(10,3,7,'0');
		printscore(19,3,7,highscore);
		printchar(25,3,7,'0');
		printstr(4,6,7,"CHARACTER  /  NICKNAME");
		if(startkeycheck(50)) return;
		putbmp(3*8-3,8*8-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[AKABEI*8+2]);
		printstr(5,8,COLOR_AKABEI,"OIKAKE\x90\x90\x90\x90\x90\x90\x90");
		if(startkeycheck(50)) return;
		printstr(18,8,COLOR_AKABEI,"\"AKABEI\"");
		if(startkeycheck(50)) return;
		putbmp(3*8-3,10*8-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[PINKY*8+2]);
		printstr(5,10,COLOR_PINKY,"MACHIBUSE\x90\x90\x90\x90");
		if(startkeycheck(50)) return;
		printstr(18,10,COLOR_PINKY,"\"PINKY\"");
		if(startkeycheck(50)) return;
		putbmp(3*8-3,12*8-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[AOSUKE*8+2]);
		printstr(5,12,COLOR_AOSUKE,"KIMAGURE\x90\x90\x90\x90\x90");
		if(startkeycheck(50)) return;
		printstr(18,12,COLOR_AOSUKE,"\"AOSUKE\"");
		if(startkeycheck(50)) return;
		putbmp(3*8-3,14*8-3,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[GUZUTA*8+2]);
		printstr(5,14,COLOR_GUZUTA,"OTOBOKE\x90\x90\x90\x90\x90\x90");
		if(startkeycheck(50)) return;
		printstr(18,14,COLOR_GUZUTA,"\"GUZUTA\"");
		if(startkeycheck(50)) return;
		printchar(11,21,COLOR_COOKIE,CODE_COOKIE);
		printstr(13,21,7,"10 PTS");
		printchar(11,22,COLOR_COOKIE,CODE_POWERCOOKIE);
		printstr(13,22,7,"50 PTS");
		printstr(6,25,4,"PUSH START BUTTON");

		gamecount=0;
		pacx=239*256;
		akax=264*256;
		ac1=5;
		av1=0;
		a2=0;
		ac2=4;
		pacspeed=-200;
		akaspeed=-200;
		while(pacx>46*256){
			ac1--;
			if(ac1==0){
				ac1=5;
				av1++;
				if(av1==6) av1=0;
			}
			if(av1==0) a1=0;
			else if(av1<=3) a1=9+av1;
			else a1=9+6-av1;
			ac2--;
			if(ac2==0){
				ac2=4;
				a2^=1;
			}
			printchar(6,17,COLOR_POWERCOOKIE,CODE_POWERCOOKIE);
			putbmp(pacx/256,133,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[a1]);
			putbmp(akax/256,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[AKABEI*8+6+a2]);
			putbmp(akax/256+18,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[PINKY*8+6+a2]);
			putbmp(akax/256+36,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[AOSUKE*8+6+a2]);
			putbmp(akax/256+54,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Monsterbmp[GUZUTA*8+6+a2]);
			if(startkeycheck(1)) return;
			clrbmpmn(pacx/256,133,XWIDTH_PACMAN,YWIDTH_PACMAN);
			clrbmpmn(akax/256,134,XWIDTH_MONSTER+54,YWIDTH_MONSTER);
			pacx+=pacspeed;
			akax+=akaspeed;
			gamecount++;
			blinkpowercookie();
		}
		pacspeed=-pacspeed;
		akaspeed=100;
		i=0;
		while(i<4){
			ac1--;
			if(ac1==0){
				ac1=5;
				av1++;
				if(av1==6) av1=0;
			}
			if(av1==0) a1=0;
			else if(av1<=3) a1=3+av1;
			else a1=3+6-av1;
			ac2--;
			if(ac2==0){
				ac2=4;
				a2^=1;
			}
			if(i<=0) putbmp(akax/256,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Ijikebmp[a2]);
			if(i<=1) putbmp(akax/256+18,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Ijikebmp[a2]);
			if(i<=2) putbmp(akax/256+36,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Ijikebmp[a2]);
			if(i<=3) putbmp(akax/256+54,134,XWIDTH_MONSTER,YWIDTH_MONSTER,Ijikebmp[a2]);
			putbmp(pacx/256,133,XWIDTH_PACMAN,YWIDTH_PACMAN,Pacmanbmp[a1]);
			if(startkeycheck(1)) return;
			clrbmpmn(pacx/256,133,XWIDTH_PACMAN,YWIDTH_PACMAN);
			switch(i){
				case 0:
					if(pacx/256>akax/256-6){
						i++;
						clrbmpmn(akax/256,134,XWIDTH_MONSTER,YWIDTH_MONSTER);
						putbmp(pacx/256,136,XWIDTH_SCORE,YWIDTH_SCORE,Scorebmp[0]);
						if(startkeycheck(30)) return;
					}
					break;
				case 1:
					if(pacx/256>akax/256+18-6){
						i++;
						clrbmpmn(akax/256+18,134,XWIDTH_MONSTER,YWIDTH_MONSTER);
						putbmp(pacx/256,136,XWIDTH_SCORE,YWIDTH_SCORE,Scorebmp[1]);
						if(startkeycheck(30)) return;
					}
					break;
				case 2:
					if(pacx/256>akax/256+36-6){
						i++;
						clrbmpmn(akax/256+36,134,XWIDTH_MONSTER,YWIDTH_MONSTER);
						putbmp(pacx/256,136,XWIDTH_SCORE,YWIDTH_SCORE,Scorebmp[2]);
						if(startkeycheck(30)) return;
					}
					break;
				case 3:
					if(pacx/256>akax/256+54-6){
						i++;
						clrbmpmn(akax/256+54,134,XWIDTH_MONSTER,YWIDTH_MONSTER);
						putbmp(pacx/256,136,XWIDTH_SCORE,YWIDTH_SCORE,Scorebmp[3]);
						if(startkeycheck(30)) return;
					}
			}
			clrbmpmn(akax/256,134,XWIDTH_MONSTER+54,YWIDTH_MONSTER);
			pacx+=pacspeed;
			akax+=akaspeed;
		}
	}
}
void game(void){
	gameinit2();//スコアなど初期化
	gamestatus=0;//0:ゲームスタート、1:ゲーム中、2:プレイヤー1減、3:ステージクリア、4:ゲームオーバー
	while(gamestatus<4){
		gameinit3();//シーンクリア、通路初期表示
		if(gamestatus==3) gamestatus=1;
		while(gamestatus<3){
			gamecount=0;//全体カウンター
			gameinit4();//パックマン、モンスター位置初期化など
			gamestart();//Ready!表示
			gamestatus=1;
			drawcount=0;
			while(gamestatus==1){
				while(drawcount==0) asm("wait");//アイドルにして割り込み待ち
				drawcount=0;
				gamecount++;
				sound();		//効果音出力
				erasechars();	//キャラクター表示消去
				keycheck();		//ボタン押下チェック
				movechars();	//キャラクター移動
				displaychars();	//キャラクター表示
				fruitcheck();	//フルーツ関係チェック
				huntedcheck();	//食った、食われたチェック
				displayscore();	//スコア表示
				pausecheck();	//一時停止ボタンチェック
			}
			PR3=0;//サウンド停止
			set_palette(COLOR_POWERCOOKIE,0,255,255);//パワーえさの色標準に戻す
			if(gamestatus==2){
				deadanim(); //パックマンやられた時のアニメーション
				if(player==0) gamestatus=4;//ゲームオーバー
				else player--;
			}
			else if(gamestatus==3){
				//ステージクリア
				stageclear(); //ステージクリア時アニメーション、コーヒーブレイク
			}
		}
	}
	gameover(); //ゲーム終了処理
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

	LCD_WriteIndex(0x37);//垂直表示開始位置設定（24行下にスクロール）
//    LCD_WriteData2(24);
    LCD_WriteData2(320-24);

	init_graphic();
	gameinit(); //ゲーム全体初期化
	while(1){
		title();//タイトル画面、スタートボタンで戻る
		game();//ゲームメインループ
	}
}
