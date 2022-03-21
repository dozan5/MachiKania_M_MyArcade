// ベクトル式VRAM　LCD出力プログラム　PIC32MX370F512H用　by K.Tanaka
// 画面上部に固定表示領域あり
//
// plib.h専用ため、XC32 v1.33以降には未対応
//
// VRAM解像度256×256ドット＋最上部固定8行
// 画面出力解像度256×216ドット＋最上部8行
// 256色同時表示、1バイトで1ドットを表す
// カラーパレット対応

// 画面出力解像度240×216ドットに改編

// (vstartx,vstarty):画面左上になるVRAM上の座標（256倍）
// (vscanv1_x,vscanv1_y):画面右方向のスキャンベクトル（256倍）
// (vscanv2_x,vscanv2_y):画面下方向のスキャンベクトル（256倍）

#include "rotatevideo_lcd.h"
#include "LCDdriver.h"
#include <plib.h>

#define nop()	asm volatile("nop")
#define nop5()	nop();nop();nop();nop();nop();
#define nop10()	nop5();nop5();

// グローバル変数定義
unsigned char VRAM[VRAM_X*VRAM_Y] __attribute__ ((aligned (4))); //VRAM
unsigned char TOPVRAM[VRAM_X*TOPLINE] __attribute__ ((aligned (4))); //画面上部の固定VRAM
volatile short vscanv1_x,vscanv1_y,vscanv2_x,vscanv2_y;	//映像表示スキャン用ベクトル
volatile short vscanstartx,vscanstarty; //映像表示スキャン開始座標
volatile short vscanx,vscany; //映像表示スキャン処理中座標

volatile unsigned short drawcount;	//　1画面表示終了ごとに1足す。アプリ側で0にする。
					// 最低1回は画面表示したことのチェックと、アプリの処理が何画面期間必要かの確認に利用。

//カラー信号波形テーブル
//256色分のカラーパレット
unsigned short ClTable[256];

/**********************
*  Timer2 割り込み処理関数
***********************/
void __ISR(8, ipl4) T2Handler(void){
	drawcount++;
	IFS0bits.T2IF = 0;			// T2割り込みフラグクリア
}

void lineoutput(unsigned short x,unsigned short y,unsigned short vx,unsigned short vy,unsigned char *vp){
	asm volatile("#":::"v0");
	asm volatile("#":::"v1");
	asm volatile("#":::"a0");
	asm volatile("#":::"a1");
	asm volatile("#":::"a2");
	asm volatile("#":::"a3");
	asm volatile("#":::"t0");
	asm volatile("#":::"t1");
	asm volatile("#":::"t2");
	asm volatile("#":::"t3");
	asm volatile("#":::"t4");
	asm volatile("#":::"t5");

	// a0=x;a1=y;a2=vx;a3=vy;
	//	t0=ClTable;
	asm volatile("la	$t0,%0"::"i"(ClTable));
	//	t1=&LATB;
	asm volatile("la	$t1,%0"::"i"(&LATB));
	//	t2=*vp;
	asm volatile("lw	$t2,16($sp)");
//	asm volatile("addiu	$t5,$zero,0x200");//LCD's WR PORT bit
	asm volatile("addiu	$t5,$zero,0x100");//LCD's WR PORT bit
//	asm volatile("addiu	$t3,$zero,256");// loop counter 256
	asm volatile("addiu	$t3,$zero,240");// loop counter 240
asm volatile("label_loop1:");
	asm volatile("ext	$v0,$a0,8,8");
	asm volatile("rotr	$v1,$a1,8");
	asm volatile("ins	$v0,$v1,8,8");
	asm volatile("addu	$v0,$v0,$t2");
	asm volatile("lbu	$v1,0($v0)");
	asm volatile("sll	$v1,$v1,1");
	asm volatile("addu	$v1,$v1,$t0");
	asm volatile("lhu	$t4,0($v1)");

//	asm volatile("sb	$t4,0($t1)"); // (byte)LATB=t4
    asm volatile("sh $t4,0xBF886130"); // (byte)LATD=t4

	asm volatile("addu	$a0,$a0,$a2");
	asm volatile("addu	$a1,$a1,$a3");

//	asm volatile("sw	$t5,4($t1)"); // LATBCLR=t5
    asm volatile("sw $t5,0xBF886334"); // LATDCLR=t5

	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

//	asm volatile("sw	$t5,8($t1)"); // LATBSET=t5
    asm volatile("sw $t5,0xBF886338");

	asm volatile("srl	$t4,$t4,8");
	asm volatile("sb	$t4,0($t1)"); // (byte)LATB=t4
	asm volatile("addiu	$t3,$t3,-1");

	asm volatile("bnez	$t3,label_loop1");
}

//液晶に画面データを転送
void putlcdall(void){
	unsigned short x,y,i;
	for(i=0;i<TOPLINE;i++){
//		LCD_SetCursor(32,i+8);
		LCD_SetCursor(0,i+8);
//		LCD_SetCursor(0,320-(i+8));
		lineoutput(0,i<<8,0x100,0,TOPVRAM);
	}
	x=vscanstartx;
	y=vscanstarty;
	for(i=0;i<216;i++){
//		LCD_SetCursor(32,i+16);
		LCD_SetCursor(0,i+16);
//		LCD_SetCursor(0,320-(i+16));
		lineoutput(x,y,vscanv1_x,vscanv1_y,VRAM);
		x+=vscanv2_x;
		y+=vscanv2_y;
	}
}

// 画面クリア
void clearscreen(void){
	unsigned int *vp;
	int i;
	vp=(unsigned int *)VRAM;
	for(i=0;i<VRAM_X*VRAM_Y/4;i++) *vp++=0;
	vp=(unsigned int *)TOPVRAM;
	for(i=0;i<VRAM_X*TOPLINE/4;i++) *vp++=0;
}

void set_palette(unsigned char n,unsigned char b,unsigned char r,unsigned char g){
	// カラーパレット設定
	// n:パレット番号0-255、r,g,b:0-255
	// R5G6B5形式で保存、ただし上位と下位を入れ替え

	unsigned short c;
	c=((r&0xf8)<<8)+((g&0xfc)<<3)+((b&0xf8)>>3);
//	ClTable[n]=c<<8;
//	ClTable[n]+=c>>8;
	ClTable[n]=c;
}

// 液晶画面回転ライブラリ初期化
void init_rotateLCD(void){
	unsigned int i;
	clearscreen();
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
	//VRAMスキャンベクトル初期化
	vscanv1_x=256;
	vscanv1_y=0;
	vscanv2_x=0;
	vscanv2_y=256;
	vscanstartx=0;
	vscanstarty=0;

	OSCCONCLR=0x10; // WAIT命令はアイドルモード
	INTEnableSystemMultiVectoredInt();
	SYSTEMConfig(96000000,SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE); //キャッシュ有効化（周辺クロックには適用しない）
	BMXCONCLR=0x40;	// RAMアクセスウェイト0

	LCD_Clear(0);

	// Timer2初期設定（60分の1秒カウンタ）
	T2CON = 0x0050;				// プリスケーラ1:32、タイマ2停止状態
	IPC2bits.T2IP = 4;			// 割り込みレベル4
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;			// タイマ2割り込み有効化
	PR2=50000;				// 60分の1秒
	T2CONSET = 0x8000;			// タイマ2スタート
}
