/* Sokoban ブートローダ付き液晶ゲームシステム用
 * をPIC32MX370F512H用に改編
 * 
 * PIC32MX250F128B with M032C9341B3(ILI9341) 8bit
 * プログラムフラッシュメモリサイズを認識し、最終アドレスを自動で判断
 * PIC32MX270F256Bにも対応
 * システムクロック48MHz、周辺クロック48MHz
 *
 * 8bit mode接続
 * D0 RB0
 * D1 RB1
 * D2 RB2
 * D3 RB3
 * D4 RB4
 * D5 RB5
 * D6 RB7
 * D7 RB8
 * RS RB9
 * WR RB10
 * CS RB11
 * RD RB13
 * RST RB14
*/

#include <plib.h>
#include "lcd.h"
#include "font.h"
#include "LCDdriver.h"

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

unsigned short key,key2,okey; //最新のボタン状態と前回のボタン状態

/* グローバル変数定義  */
short i,j, k, l,swf, temp, Size;
short cx,cy,hosu,men,pmx,pmy,psx,psy,mx,my,slc,scc,scs,udcun,wt;
char Line, Colum,sw0,sw1,swu,swd,swl,swr,sww;
char md,mdo,stgf,bm,mu,upf,mcf,dpm,wc,upc,esc,es;
char damy[16][2],ts[6];;
char MsgScrn[] ={0,0,0};
unsigned char vram[120][60];
char mram[18][23];
unsigned char undom[10][36];
unsigned char bset[16][2],dxy[18][2];
//unsigned char lcd_buf[256]__attribute__((space(dma)));  //dma_buffer

void keycheck(void){

//ボタン状態読み取り
//keystatus :現在押されているボタンに対応するビットを1にする
//keystatus2:前回押されていなくて、今回押されたボタンに対応するビットを1にする
    swu=1,swl=1,swr=1,swd=1,sw0=1,sw1=1,sww=1;
	oldkey=key;
	key=~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT | KEYSTART | KEYFIRE);
    swu=PORTDbits.RD2;
    swd=PORTDbits.RD0;
    swr=PORTDbits.RD3;
    swl=PORTDbits.RD1;
    sw0=PORTDbits.RD5;
    sw1=PORTDbits.RD4;

	key2=key & ~okey; //ボタンから手を離したかチェック

}

/****************************
* キャラクタフォントのVRAM書き込み(１バイト横２ドット分）
* colum 横　line 縦　letter キャラクタNo　color1 フォアカラー　color2 バックカラー
*****************************/
void lcd_vChar(char colum, char line, unsigned char letter, unsigned char Color1,unsigned char Color2){
	unsigned char j, i,c1,c2, Mask;

	if((colum < 42) && (line < 30)){		// 範囲チェック
		for(j=0; j<8; j++){				// 横5ドット
			Mask = 0x80;					// 上位ビットから表示
			for(i=0; i<3; i++){			// 縦1ライン分表示
				if((font[letter][j] & Mask) != 0)
					c1=Color1;			//奇数ドットフォアカラー
				else
					c1=Color2;			//バックカラー
				Mask = Mask >> 1;
				if((font[letter][j] & Mask) != 0)
					c2=Color1;			//偶数ドットカラー
				else
					c2=Color2;			//バックカラー
				Mask = Mask >> 1;
				lcd_vramset(colum*6+i*2, line*8+j, c1,c2);//VRAM書き込みへ
			}
		}

	}
}
/****************************
* マップチップ大のVRAM書き込み(12*12)
* cx 横ライン　cy 縦ライン　pat キャラクタNo
*****************************/
void vcip(short x,short y, unsigned char pat){
	unsigned char j, i,c1,c2;
		for(j=0; j<12; j++){
			for(i=0; i<6; i++){
				c1=cip[pat*12+j][i*2];
				c2=cip[pat*12+j][i*2+1];
				lcd_vramset(x*12+i*2,y*12+j,c1,c2);
			}
		}
}
/****************************
* マップ大の読み込み(12*12)
*pmx,pmy 画面スクロール時の補正値
*****************************/
void vmap(){
	unsigned char i,j;
	for (i=0; i<bm; i++){
		if(mram[bset[i][1]][bset[i][0]]<2)	//荷物置き場の表示処理
		mram[bset[i][1]][bset[i][0]]=1;
	}
		for (i=0; i<20; i++){
		for (j=0; j<20; j++){
			if((j+pmx)<23)
			vcip(j,i,mram[i+pmy][j+pmx]);	//10*10チップ書き込みへ
		}
	}
}
/****************************
* マップチップ小のVRAM書き込み(8*8)
* cx 横ライン　cy 縦ライン　pat キャラクタNo
*****************************/
void vcips(short x,short y, unsigned char pat){
	unsigned char j, i,c1,c2;
		for(j=0; j<8; j++){
			for(i=0; i<4; i++){
				c1=cips[pat*8+j][i*2];
				c2=cips[pat*8+j][i*2+1];
				lcd_vramset(x*8+i*2,y*8+j,c1,c2);
			}
		}
}
/****************************
* マップ小の読み込み(8*8)
*　*psx,psy 画面スクロール時の補正値
*****************************/
void vmaps(){
	unsigned char i,j;
	for (i=0; i<bm; i++){
		if(mram[bset[i][1]][bset[i][0]]<2)
		mram[bset[i][1]][bset[i][0]]=1;
	}
		for (i=0; i<30; i++){
		for (j=0; j<30; j++){
			if((j+pmx)<23)
			vcips(j,i,mram[i+psy][j+psx]);	//8*8チップ書き込みへ
		}
	}
}

/****************************
 * マップ用VRAMの読み込み・書き込み
 * 23*18エリアマップ
 *　d=0 床　1 荷物置き場 2 壁 3 荷物 4 作業員 8 すでに荷物置き場にある荷物 9 面の広さ
 *****************************/
void mramset(unsigned char stg) {
	unsigned char i, j, c, d;
	c = 0;
	for (i = 0; i < 18; i++) {
		for (j = 0; j < 23; j++) {
			d = map[men * 18 + i][j]; // 面チップの読み込み
			if ((d == 0) || (d == 2)) { // 床か壁なら
				mram[i][j] = d; // マップ用ramに書き込み
			}
			if (d == 1) { // 荷物置き場の読み込み
				bset[c][0] = j; // x座標
				bset[c][1] = i; // y座標
				c++; // 置き場数のカウント
			}
			if (d == 3) { // 壁チップの読み込み
				mram[i][j] = d;
			}
			if (d == 4) { // 作業員の読み込み
				cx = j;
				cy = i;
			}
			if (d == 8) { // すでに置き場に置かれた荷物の読み込み
				bset[c][0] = j;
				bset[c][1] = i;
				mram[i][j] = 3;
				c++;
			}
			if (d == 9) { // マップの広さ読み込み
				mx = j;
				my = i;
			}
		}
	}
	bm = c; // 	荷物置き場の数保存
	if (mx < 11) { // マップが表示画面より小さければ
		pmx = 0; // スクロール補正pmxを0に
	} else { // マップが表示画面より大きければ
		pmx = cx - 5; // 補正値を作業員座標より-5
		if (pmx < 0)
			pmx = 0;
		if (pmx + 10 > mx)
			pmx = mx - 10;
	}
	if (mx < 17) { //以下、 作業員がスクロール時画面の中央付近になるように制御
		psx = 0;
	} else {
		psx = cx - 8;
		if (psx < 0)
			psx = 0;
		if (psx + 16 > mx)
			psx = mx - 16;
	}
	if (my < 11) {
		pmy = 0;
	} else {
		pmy = cy - 5;
		if (pmy < 0)
			pmy = 0;
		if (pmy + 10 > my)
			pmy = my - 10;
	}
	if (my < 17) {
		psy = 0;
	} else {
		psy = cy - 8;
		if (psy < 0)
			psy = 0;
		if (psy + 16 > my)
			psy = my - 16;
	}
}
/****************************
* VRAMのクリアー
*****************************/
void lcd_vClear(unsigned char Color){
	short i,j;
	for (i=0; i<120; i++){
		for (j=0; j<60; j++){
			vram[i][j]=Color*16+Color;
		}
	}

}
/****************************
* LCDの一括表示
*****************************/
void lcd_disp(void){
	unsigned short x,y,c,k,i,j;
	unsigned short cda;

    LCD_WriteIndex(0x2A);
    LCD_WriteData2(0);
//    LCD_WriteData2(LCD_X_RES-1);
    LCD_WriteData2(240-1);

	LCD_WriteIndex(0x2B);
    LCD_WriteData2(0);
//    LCD_WriteData2(LCD_Y_RES-1);
    LCD_WriteData2(240-1);

	LCD_WriteIndex(0x2C);

	for (y = 0; y < 120; y++) {
		for (i = 0; i < 2; i++) {
			for (x = 0; x < 60; x++) {
				cda = col[vram[y][x] / 16];
				cda=(cda>>11)+(cda&0x7e0)+((cda&0x1f)<<11); //swap R and B
				LCD_WriteData16(cda);
				LCD_WriteData16(cda);
				cda = col[vram[y][x] % 16];
				cda=(cda>>11)+(cda&0x7e0)+((cda&0x1f)<<11); //swap R and B
				LCD_WriteData16(cda);
				LCD_WriteData16(cda);
			}

		}
	}
/*
	for (y = 0; y < 120; y++) {
		for (i = 0; i < 2; i++) {
			for (x = 0; x < 20; x++) {
				LCD_WriteData16(0);
				LCD_WriteData16(0);
				LCD_WriteData16(0);
				LCD_WriteData16(0);
			}

		}
	}
*/
}

/****************************
 * vramへの書き込み
 *****************************/
void lcd_vramset(short cx, short cy, unsigned char dat1, unsigned char dat2) {
	if ((cx < 120) && (cy < 120)) {
		vram[cy][cx / 2] = dat1 * 16 + dat2;
	}
}
/****************************
*  vramへ文字列書き込み
*****************************/
void lcd_vStr(char colum, char line, char *s, unsigned char Color1, unsigned char Color2)
{
    	while (*s){
		lcd_vChar(colum++, line, *s++, Color1, Color2);
		if(colum >= 42){
			line++;
			colum = 0;
			if(line >= 30)
				line = 0;
		}
	}
}

/****************************
*  サウンド1処理
*****************************/
void sund2(unsigned short hz){
	//OpenTimer2(T2_ON & T2_GATE_OFF & T2_PS_1_256 & T2_SOURCE_INT,hz-1);
	//SetDCOC1PWM(hz/2);

}
/****************************
*  サウンド2処理
*****************************/
void sund(unsigned short hz){
	if(hz==0) PR3=0;
	else PR3=3000000/hz;
}


/****************************
*  数値の文字列変換
*****************************/
void tostr(unsigned int index,char imx){
	unsigned char i;
		for (i=0; i<imx+1; i++){
			ts[i]=0;
		}
	switch(imx){
		case 0: ts[0]=0;
			break;
		case 1:
			ts[0]=index%10+0x30;
			break;
		case 2:ts[0]=(index%100)/10+0x30;
			ts[1]=index%10+0x30;
			break;
		case 3:
			ts[0]=(index%1000)/100+0x30;
			ts[1]=(index%100)/10+0x30;
			ts[2]=index%10+0x30;
			break;
		case 4:
			ts[0]=(index%10000)/1000+0x30;
			ts[1]=(index%1000)/100+0x30;
			ts[2]=(index%100)/10+0x30;
			ts[3]=index%10+0x30;
			break;
		case 5:
			ts[0]=(index%100000)/10000+0x30;
			ts[1]=(index%10000)/1000+0x30;
			ts[2]=(index%1000)/100+0x30;
			ts[3]=(index%100)/10+0x30;
			ts[4]=index%10+0x30;
			break;
		case 6:
			ts[0]=index/100000+0x30;
			ts[1]=(index%100000)/10000+0x30;
			ts[2]=(index%10000)/1000+0x30;
			ts[3]=(index%1000)/100+0x30;
			ts[4]=(index%100)/10+0x30;
			ts[5]=index%10+0x30;
			break;
	}
}

/*********** メインルーチン ***************/
int main(void)
{
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

	T5CON = 0x0010;				// プリスケーラ1:2、タイマ5停止状態
	IPC5bits.T5IP = 4;			// 割り込みレベル4
	IFS0bits.T5IF = 0;
	IEC0bits.T5IE = 1;			// タイマ5割り込み有効化

	// 液晶表示器の初期化と開始メッセージ表示
//	LCD_Init();
	LCD_Clear(0);
	lcd_vClear(0);
	lcd_disp();
	//T1CON = 0x0030;
	//PR1 = 7800;					// 50msec 7800
	//IFS0bits.T1IF = 0;			// 割り込みフラグクリア
	//IEC0bits.T1IE = 1;			// 割り込み許可
	//T1CONbits.TON = 1;
	MsgScrn[3] =0;
	mcls();
	md=0;
	k=12;
	l=0;
	//sund(0);
	//sund2(0);
	/****** メイン ループ  *******/
 	while(1){
            	//IFS0bits.T1IF = 0;

        keycheck();
	switch(md){			// モード処理
		case 0:			// スタート画面

		//lcd_vClear(0);
			opening();
			break;
		case 1:			// ゲーム画面

			stage();
			break;
		case 2:			// メニュー画面
		lcd_vClear(0);

			stsel();
			break;
		default:
			break;
	}

	if(sw0!=0)
		swf=0;
	}
}
/****************************
*  マップのクリアー
*****************************/
void mcls(void){
	mu=0;
		upf=0;
		upc=0;
		mcf=0;
		hosu=0;
		cx=0;
		cy=0;
		esc=0;
		slc=0;
		for(i=0; i<16; i++){
		bset[i][0]=0;
		bset[i][1]=0;
		}
		for (i=0; i<18; i++){
			for(j=0; j<21; j++){
				mram[i][j]=0;
			}
		}
		mramset(0);
}
/****************************
*  割り込み処理　
*****************************/
//void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
//{



//}
/****************************
*  作業員の操作
*****************************/
void move(void){
	char d1,d2;
	unsigned short wts;
	wts=200;				//　操作用ウェイトデータ
	wt=0;				// 操作用ウェイト
	if(swl==0){			//　左ボタンが押されたら
		mu=0;
		slc=0;
		d1=mram[cy][cx-1];	// 作業員の進行方向のチップ
		d2=mram[cy][cx-2];	//　作業員の２進行方向のチップ

		if(d1==0 || d1==1){	// 進行方向が床か荷物置き場なら
			mram[cy][cx]=0;	// 作業員の移動処理
			cx=cx-1;
			hosu++;			// 歩数のカウント
		}
		if(d1==3){			// 進行方向が荷物なら
			if((d2==0) || (d2==1)){	// ２進行方向が床か荷物置き場なら
				mram[cy][cx]=0;		// 作業員の移動処理
				cx=cx-1;
				mram[cy][cx]=0;		// 荷物の移動処理
				mram[cy][cx-1]=3;
				hosu++;
				sund(1600);			// 移動音を出す
			}
		}
		wt=wts;					// ウェイト時間設定
		goto pas0;				//パスへ移動
	}
	if(swu==0){					//　上ボタンが押されたら
		slc=0;					//　以下同上
		mu=1;
		d1=mram[cy-1][cx];
		d2=mram[cy-2][cx];
		if(d1==0){
			mram[cy][cx]=0;
			cy=cy-1;
			hosu++;
		}
		if(d1==1){
			mram[cy][cx]=0;
			cy=cy-1;
			hosu++;
		}
		if(d1==3){
			if((d2==0) || (d2==1)){
				mram[cy][cx]=0;
				cy=cy-1;
				mram[cy][cx]=0;
				mram[cy-1][cx]=3;
				if(dpm==1)
				hosu++;
				sund(1600);
			}
		}
		wt=wts;
		goto pas0;
	}
	if(swr==0){					//　右ボタンが押されたら
		mu=2;
		slc=0;
		d1=mram[cy][cx+1];
		d2=mram[cy][cx+2];
		if(d1==0){
			mram[cy][cx]=0;
			cx=cx+1;
			hosu++;
		}
		if(d1==1){
			mram[cy][cx]=0;
			cx=cx+1;
			hosu++;
		}
		if(d1==3){
			if((d2==0) || (d2==1)){
				mram[cy][cx]=0;
				cx=cx+1;
				mram[cy][cx]=0;
				mram[cy][cx+1]=3;
				hosu++;
				sund(1600);
			}
		}
		wt=wts;
		goto pas0;
	}
	if(swd==0){					//　下ボタンが押されたら
		mu=3;
		slc=0;
		d1=mram[cy+1][cx];
		d2=mram[cy+2][cx];
		if(d1==0){
			mram[cy][cx]=0;
			cy=cy+1;
			hosu++;
		}
		if(d1==1){
			mram[cy][cx]=0;
			cy=cy+1;
			hosu++;
		}
		if(d1==3){
			if((d2==0) || (d2==1)){
				mram[cy][cx]=0;
				cy=cy+1;
				mram[cy][cx]=0;
				mram[cy+1][cx]=3;
				hosu++;
				sund(1600);
			}
		}
		wt=wts;
	}
pas0:
	  	if(cx-pmx<3)		//　画面スクロールの補正処理（pmx,y通常、psx,y縮小時）
		pmx--;
		if(pmx<0)
		pmx=0;
		if(cx-pmx>7){
		pmx++;
		if(cx+2>mx)
		pmx--;
		}
		if(cx-psx<3)
		psx--;
		if(psx<0)
		psx=0;
		if(cx-psx>12){
		psx++;
		if(cx+2>mx)
		psx--;
		}
		if(cy-pmy<3)
		pmy--;
		if(pmy<0)
		pmy=0;
		if(cy-pmy>7){
		pmy++;
		if(cy+2>mx)
		pmy--;
		}
		if(cy-psy<3)
		psy--;
		if(psy<0)
		psy=0;
		if(cy-psy>13){
		psy++;
		if(cy+2>mx)
		psy--;
		}


}
/****************************
*  スコア表示と荷物置き場の処理
*****************************/
void scr(void){
	unsigned char i;
	upf=0;
	MsgScrn[0] =0;MsgScrn[1] =0;MsgScrn[2] =0;
	tostr(men+1,2);
	lcd_vStr(0, 0,ts,3,0);
	tostr(hosu,3);
	lcd_vStr(3, 0,ts,3,0);
	for (i=0; i<bm; i++){
		if(mram[bset[i][1]][bset[i][0]]==3){
			if(dpm==1){
				if(bset[i][0]-pmx>=0 && bset[i][1]-pmy>=0)
				vcip(bset[i][0]-pmx,bset[i][1]-pmy,8);
			}
			else
			{
				if(bset[i][0]-psx>=0 && bset[i][1]-psy>=0)
				vcips(bset[i][0]-psx,bset[i][1]-psy,8);
			}
			upf++;
		}
	}
	if(upc!=upf){
		upc=upf;
	sund(400);
	delay_ms(100);
	sund(0);
	}
	tostr(upf,2);
	lcd_vStr(7, 0,ts,3,0);
	lcd_vStr(9, 0,"/",3,0);
	tostr(bm,2);
	lcd_vStr(10, 0,ts,3,0);

}
/****************************
*  スタート画面の処理
*****************************/
void opening(void){
	static char sel,i;
    delay_ms(100);
	lcd_vClear(0);
	if((swu==0) && (swf==0)){
		sel=0;
		swf=1;
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	}
	if((swd==0) && (swf==0)){
		sel=1;
		swf=1;
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	}
    /*
    lcd_vStr(0, 0,"0",0,0);
    lcd_vStr(1, 0,"1",1,0);
    lcd_vStr(2, 0,"2",2,0);
    lcd_vStr(3, 0,"3",3,0);
    lcd_vStr(4, 0,"4",4,0);
    lcd_vStr(5, 0,"5",5,0);
    lcd_vStr(6, 0,"6",6,0);
    lcd_vStr(7, 0,"7",7,0);
    lcd_vStr(8, 0,"8",8,0);
    lcd_vStr(9, 0,"9",9,0);
    lcd_vStr(10, 0,"0",10,0);
    lcd_vStr(11, 0,"1",11,0);
    lcd_vStr(12, 0,"2",12,0);
    lcd_vStr(13, 0,"3",13,0);
    lcd_vStr(14, 0,"4",14,0);
    lcd_vStr(15, 0,"5",15,0);
     * */
	lcd_vStr(7, 2,"SOKOBAN",13,0);
	if(sel==0)
	{
	lcd_vStr(7, 4," START  ",3,1);
	lcd_vStr(7, 6," SELECT ",2,0);
	}
	if(sel==1)
	{
	lcd_vStr(7, 4," START  ",2,0);
	lcd_vStr(7, 6," SELECT ",3,1);
	}
	for(i=0; i<11; i++){
		vcip(i,7,2);
	}
	vcip(k,6,3);
	vcip(k+1,6,4);
	l++;
	if(l>10){
	l=0;
	k--;
	if(k<1)
	k=11;
	}
	lcd_disp();
	if(sw0==0 && swf==0){
		swf=1;
		if(sel==0){
		md=1;
		dpm=1;
		lcd_vClear(0);
		lcd_disp();
		lcd_vStr(5, 6,"         ",3,0);
		lcd_vStr(5, 7,"  START! ",3,0);
		lcd_vStr(5, 8,"         ",3,0);
		lcd_disp();
		sund(1600);
		delay_ms(200);
		sund(713);
		delay_ms(200);
		sund(635);
		delay_ms(200);
		sund(0);
		delay_ms(500);
		}
		else
		{
		md=2;
		dpm=0;
		delay_ms(500);
		}
		lcd_vClear(0);
		lcd_disp();
		mramset(0);
		delay_ms(200);
	}
	if(sw1==0 && swf==0){
		md=mdo;
	}
}
/****************************
* ゲームステージ
*****************************/
void stage(void){

	if(esc==1){
		escset();
		goto pas;
	}

	lcd_vClear(0);
	if((sw1==0) && (swf==0)){
		esc=1;
		es=0;
		slc=0;
		sund(400);
		delay_ms(200);
		sund(0);
		delay_ms(200);
	}
	if((sw0==0) && (swf==0)){
		swf=1;
		slc=0;
		if(dpm==1){
		dpm=0;
		}
		else
		{
		dpm=1;
		}
		sund(400);
		delay_ms(200);
		sund(0);
		delay_ms(200);
	}
	move();
		slc++;
	if(slc>200){
		slc=210;
		scc++;
		if(scc>38)
		scc=0;
		mram[cy][cx]=10+scc/10;
	}
	else
	{
	mram[cy][cx]=mu+4;
	}
	if(dpm==1)
	vmap();
	else
	vmaps();
	scr();
	lcd_disp();
	delay_ms(wt);
	sund(0);
	if(upf==bm){
		lcd_vStr(5, 6,"         ",3,0);
		lcd_vStr(5, 7," CLEAR!! ",3,0);
		lcd_vStr(5, 8,"         ",3,0);
		lcd_disp();
		sund(635);
		delay_ms(200);
		sund(424);
		delay_ms(1000);
		sund(0);
		delay_ms(1000);
		men++;
		mcf=0;
		if(men==30)
		men=0;
		mcls();
		delay_ms(2000);
	}

pas:
Nop();
}
/****************************
* ステージ選択
*****************************/
void stsel(void){
	if(swu==0){
		men++;
		if(men>29)
		men=29;
		mcls();
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	}
	if(swd==0){
		men--;
		if(men<0)
		men=0;
		mcls();
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	}
	if(swr==0){
	psx++;
	if(psx+15>mx)
	psx--;
	delay_ms(200);
	}
	if(swl==0){
	psx--;
	if(psx<0)
	psx=0;
	delay_ms(200);
	}
	if((sw0==0) && (swf==0)){
	md=1;
	dpm=1;
	lcd_vClear(0);
		lcd_vStr(5, 6,"         ",3,0);
		lcd_vStr(5, 7,"  START! ",3,0);
		lcd_vStr(5, 8,"         ",3,0);
		lcd_disp();
		sund(800);
		delay_ms(200);
		sund(713);
		delay_ms(200);
		sund(635);
		delay_ms(200);
		sund(0);
	}
	if((sw1==0) && (swf==0)){
		md=mdo;
	}
	vmaps();
	mram[cy][cx]=mu+4;
	tostr(men+1,2);
	lcd_vStr(0, 0,"No = ",3,0);
	lcd_vStr(5, 0,ts,3,0);
	lcd_disp();

}
/****************************
*  メニュー
*****************************/
void escset(void){
	lcd_vStr(5, 3,"            ",2,0);
	lcd_vStr(5, 4," RETRAY     ",2,0);
	lcd_vStr(5, 5,"            ",2,0);
	lcd_vStr(5, 6," MAP_SELCT  ",2,0);
	lcd_vStr(5, 7,"            ",2,0);
	lcd_vStr(5, 8," START_MENU ",2,0);
	lcd_vStr(5, 9,"            ",2,0);
	lcd_vStr(5, 10," RETURN     ",2,0);
	lcd_vStr(5, 11,"            ",2,0);

	switch(es){
		case 0:
			lcd_vStr(5, 4," RETRAY     ",3,1);
			break;
		case 1:
			lcd_vStr(5, 6," MAP_SELCT  ",3,1);
			break;
		case 2:
			lcd_vStr(5, 8," START_MENU ",3,1);
			break;
		case 3:
			lcd_vStr(5, 10," RETURN     ",3,1);
			break;
		}
		lcd_disp();
	if((sw0==0) && (swf==0)){
		swf=1;
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	switch(es){
		case 0:
			mcls();
			mramset(0);
			lcd_vClear(0);
			lcd_vStr(5, 6,"         ",3,0);
			lcd_vStr(5, 7," RETRAY! ",3,0);
			lcd_vStr(5, 8,"         ",3,0);
			lcd_disp();
			sund(1600);
			delay_ms(200);
			sund(713);
			delay_ms(200);
			sund(635);
			delay_ms(200);
			sund(0);
			delay_ms(500);
			esc=0;
			break;
		case 1:
			mcls();
			mdo=1;
			md=2;
			esc=0;
			break;
		case 2:
			mcls();
			md=0;
			k=12;
			l=0;
			esc=0;
			break;
		case 3:
			esc=0;
			break;
		}
	}
	if((sw1==0) && (swf==0)){
		esc=0;
		swf=1;
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	}
	if((swu==0) && (swf==0)){
		es--;
		swf=1;
		if(es<0)
		es=0;
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	}
	if((swd==0) && (swf==0)){
		es++;
		swf=1;
		if(es>3)
		es=3;
		sund(400);
		delay_ms(100);
		sund(0);
		delay_ms(100);
	}
}
