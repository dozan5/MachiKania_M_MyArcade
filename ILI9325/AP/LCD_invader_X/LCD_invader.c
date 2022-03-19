//スペースインベーダー for PIC32MX250F128B / Color LCD Game System w/bootloader by K.Tanaka
//をPIC32MX370F512H用に改編

#include <plib.h>
#include "LCDdriver.h"
#include "graphlib.h"

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

extern unsigned char bmp_missile1[],bmp_missile2[];

unsigned short keystatus,keystatus2,oldkey; //最新のボタン状態と前回のボタン状態
int ufox; //UFO X座標
int missilex,missiley; //自機ミサイル座標
int al_dir,al_x,al_y,al_conter; //インベーダー移動方向、左上座標、移動速度カウンター
int zanki; //自機残数
int explodecounter; //自機爆発中カウンタ
int ufo_dir,ufo_score,ufo_counter; //UFO移動方向、UFOスコア、UFO出現カウンタ
int stage; //ステージ番号
int gamestatus; //ゲームステータス
int al_animation; //インベーダーアニメーションカウンタ
int al_zan,cannonx,al_missilecount; //インベーダー残数、自機X座標、敵ミサイル出現カウンタ
int alien[5][11]; //インベーダー配列
int al_missilex1,al_missilex2,al_missiley1,al_missiley2; //敵ミサイルの座標（2つ）
unsigned int highscore,score; //ハイスコア、得点

//sound data
//上位16ビット 音の長さ（60分のn秒）　0の場合繰り返し数
//下位16ビット 音程 48000000/16/f[Hz]
unsigned int SOUND1[]={0x000A095E,1};//FIRE
unsigned int SOUND2[]={0x000213FB,0x000809FD,1};//AILEN EXPLOSION
unsigned int SOUND3[]={0x000315A5,0x000314AB,1};//UFO MOVING
unsigned int SOUND4[]={0x000418FA,0x00020000,3};//UFO SCORE
unsigned int SOUND5[]={0x00036A8F,0x00010000,30};//CANNON EXPLOSION
unsigned int * soundarray[]={SOUND1,SOUND2,SOUND3,SOUND4,SOUND5};

//Sound構造体
struct {
	unsigned int *p; //曲配列の演奏中の位置
	unsigned int *start; //曲配列の開始位置
	unsigned short count; //発音中の音カウンタ
	unsigned short loop; //曲配列繰り返しカウンタ
	unsigned char stop; //0:演奏中、1:終了
} Sound;

volatile unsigned short drawcount;
/**********************
*  Timer2 割り込み処理関数
***********************/
void __ISR(8, ipl4) T2Handler(void){
	drawcount++;
	IFS0bits.T2IF = 0;			// T2割り込みフラグクリア
}

void playsound(void){
//60分の1秒ごとに呼び出し、効果音を変更
	if(Sound.stop) return; //サウンド停止中
	Sound.count--;
	if(Sound.count>0) return;
	Sound.p++; //次の音へ
	if(*Sound.p<0x10000){ //上位16ビットが0の場合、繰り返し
		Sound.loop--;
		if(Sound.loop==0){
			PR3=0; //サウンド停止
			Sound.stop=1;
			return;
		}
		Sound.p=Sound.start; //最初に戻る
	}
	Sound.count=*Sound.p>>16;
	PR3=*Sound.p&0xffff; //音程設定
}
void sound(int n){
//効果音開始
	unsigned int *p;
	Sound.p=soundarray[n-1];
	Sound.start=Sound.p;
	Sound.count=*Sound.p>>16;
	PR3=*Sound.p&0xffff;
	Sound.stop=0;
	p=Sound.p;
	while(*p>=0x10000) *p++;
	Sound.loop=*p;
}
void wait60thsec(unsigned short n){
	// 60分のn秒ウェイト
	drawcount=0;
	while(drawcount<n) asm("wait");
}
void wait60thsecwithsound(unsigned short n){
	// 60分のn秒ウェイト（効果音継続）
	while(n>0){
		drawcount=0;
		while(drawcount==0) asm("wait");
		playsound();
		n--;
	}
}
void keycheck(void){
//ボタン状態読み取り
//keystatus :現在押されているボタンに対応するビットを1にする
//keystatus2:前回押されていなくて、今回押されたボタンに対応するビットを1にする
	oldkey=keystatus;
	keystatus=~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT | KEYSTART | KEYFIRE);
	keystatus2=keystatus & ~oldkey; //ボタンから手を離したかチェック
}
void initgame(void){
//ゲーム初期化
	printstr(64,80,7,0,"SPACE ALIEN");
	printstr(48,110,7,0,"PUSH START BUTTON");
	while(1){
		//STARTキー待ち
		keycheck();
		if(keystatus & KEYSTART) break;
		wait60thsec(1);
		rand();
	}
	Sound.stop=1;
	stage=0;
	score=0;
	zanki=3;
	clearscreen();
}
void addscore(int s){
//得点追加
	score+=s;
	if(score>=99999) score=99999;
	if(score>highscore) highscore=score;
}
void printscore(void){
//得点表示
	printnum2(16,304,7,0,score,5);
	printnum2(136,304,7,0,highscore,5);
}
void putzanki(void){
//自機残数表示
	int i;
	printnum(8,208,5,0,zanki);
	for(i=1;i<zanki;i++){
		if(i>6) break;
		printstr(i*16+16,208,5,0,"\xa0\xa1");
	}
	for(;i<=6;i++){
		printstr(i*16+16,208,5,0,"  ");
	}
}
void clearchar(void){
// キャラクター表示消去
	//ミサイル消去
	if(missiley>0){
		boxfill(missilex,missiley,missilex+1,missiley+3,0);
	}
	else if(missiley==-1){
		boxfill(missilex-3,0,missilex+4,7,0);
	}
	if(al_missiley1>0){
		boxfill(al_missilex1,al_missiley1,al_missilex1+1,al_missiley1+3,0);
	}
	else if(al_missiley1==-1){
		boxfill(al_missilex1-2,198,al_missilex1+3,205,0);
		al_missiley1=0;
	}
	if(al_missiley2>0){
		boxfill(al_missilex2,al_missiley2,al_missilex2+1,al_missiley2+3,0);
	}
	else if(al_missiley2==-1){
		boxfill(al_missilex2-2,198,al_missilex2+3,205,0);
		al_missiley2=0;
	}
}
void clearalien(int x,int y){
//インベーダー表示消去（全体）
	int i,j,w;
	for(j=0;j<=4;j++){
		w=x;
		for(i=0;i<=10;i++){
			if(alien[j][i]) boxfill(w,y,w+15,y+7,0);
			w+=16;
		}
		y+=16;
	}
}
void button(void){
	keycheck();
}
void fire(void){
//ミサイル発射チェック
	int p,q;
	if(explodecounter>0) return;
	if(missiley==0 && (keystatus2 & KEYFIRE)){
		//自機ミサイル発射
		rand();
		missiley=180;
		missilex=cannonx+8;
		sound(1);
	}
	if(al_missilecount>0) al_missilecount--; //敵ミサイルカウンタ
	if(al_missilecount==0 && (al_missiley1==0 || al_missiley2==0)){
		p=rand()%11;
		for(q=4;q>=0;q--){
			if(alien[q][p]>0) break;
		}
		if(q<0 || al_y+q*16>=176) return; //ミサイル発射できる高さにいない
		//敵ミサイル発射
		if(al_missiley1==0){
			al_missilex1=al_x+p*16+7;
			al_missiley1=al_y+q*16+8;
		}
		else{
			al_missilex2=al_x+p*16+7;
			al_missiley2=al_y+q*16+8;
		}
		al_missilecount=50;
	}
}
void movecannon(void){
//自機移動
	if(explodecounter>0){
		//自機爆発中
		explodecounter--;
		if(explodecounter==0){
			//爆発終了、残数表示更新、自機消去
			putzanki();
			boxfill(cannonx,184,cannonx+15,191,0);
			cannonx=8;
		}
		return;
	}
	if(cannonx>0   && (keystatus&(KEYLEFT|KEYRIGHT))==KEYLEFT ) cannonx--;
	if(cannonx<192 && (keystatus&(KEYLEFT|KEYRIGHT))==KEYRIGHT) cannonx++;
}
void movealien(void){
//インベーダー移動
	int a,i,s;
	if(explodecounter>0) return;//自機爆発中
	al_conter++;//敵移動カウンター
	//敵残数によって移動速度を変える
	if((al_zan>=20 && al_conter<20) || (al_zan>=12 && al_conter<10) ||
		(al_zan>=6 && al_conter<6) || (al_zan>=3 && al_conter<2) ||
		al_conter<1) return;
	al_conter=0;
	al_x+=al_dir;
	s=0;
	al_animation=1-al_animation;

	//左右移動できるところまで移動。端の場合1段下げる
	if(al_dir>0 && al_x>32){
		a=12-al_x/16;
		for(i=0;i<=4;i++){
			s+=alien[i][a];
		}
		if(s>0){
			al_x-=al_dir;
			al_y+=8;
			al_dir=-al_dir;
		}
	}
	else if(al_x<0){
		a=-al_x/16;
		for(i=0;i<=4;i++){
			s+=alien[i][a];
		}
		if(s>0){
			al_x-=al_dir;
			al_y+=8;
			al_dir=-al_dir;
		}
	}
	if(s>0) clearalien(al_x,al_y-8); //1段下がった場合、敵の表示全体を消去
}
void moveufo(void){
//UFO移動
	ufo_counter++;
	if(ufox>=0){
		//UFO出現中
		if(ufo_counter>0 && (ufo_counter&1)==0) ufox+=ufo_dir;
		if(ufo_counter==6){
			ufo_counter=0;
			if(explodecounter==0) sound(3);
		}
		if(ufox<0 || ufox>184 || ufo_counter==-1){
			//UFOが端から逃げた場合
			boxfill(ufox,8,ufox+23,15,0);
			ufox=-1;
			ufo_dir=-ufo_dir;
			ufo_counter=0;
		}
	}
	else{
		//UFOがいない時
		if(ufo_counter>=1500 && al_zan>7){
			ufo_counter=0;
			if(ufo_dir>0) ufox=0;
			else ufox=184;
			sound(3);
		}
	}
}
void movemissile(void){
//ミサイル移動
	if(missiley>0){
		missiley-=4;
		if(missiley<=0) missiley=-3;//てっぺんで爆発させるカウンタとする
	}
	else if(missiley<0) missiley++;//てっぺんで爆発中
	if(al_missiley1){
		al_missiley1++;
		if(al_missiley1>=200) al_missiley1=-3;//地面で爆発させるカウンタ
	}
	if(al_missiley2){
		al_missiley2++;
		if(al_missiley2>=200) al_missiley2=-3;//地面で爆発させるカウンタ
	}
}
void checkhit(void){
// ミサイルとインベーダーの衝突チェック
	int x,y;
	if(missilex< al_x    ) return;
	if(missilex>=al_x+176) return;
	if(missiley< al_y    ) return;
	if(missiley>=al_y+72 ) return;
	x=(missilex-al_x)/16;
	y=(missiley-al_y)/16;
	if((al_x+x*16+2 )>missilex) return;
	if((al_x+x*16+13)<missilex) return;
	if((al_y+y*16+2 )>missiley) return;
	if((al_y+y*16+15)<missiley) return;
	if(alien[y][x]<=0) return;
	if(explodecounter==0) sound(2);

	//インベーダーに命中
	addscore(alien[y][x]*10);
	alien[y][x]=-4; //爆発カウンター
	al_zan--;
	al_conter=-3; //爆発中で移動停止カウンター
	missiley=0;
}
void checkcollision(void){
//各種衝突チェック
	int s,i,j;
	// インベーダーとミサイル
	if(missiley>0) checkhit();

	// UFOとミサイル
	if(missiley>=8 && missiley<16 && ufox>=0 && ufo_counter>=0){
		if(missilex>=ufox+4 && missilex<ufox+19){
			missiley=0;
			sound(2);
			ufo_counter=-25;//UFO爆発カウンター
			ufo_score=((rand()&3)+2)*50;
			if(ufo_score==250) ufo_score=300;
			addscore(ufo_score);
		}
	}

	// ミサイル同士の衝突チェック
	if(missiley>0){
		if(missilex==al_missilex1 && missiley>=al_missiley1 && missiley<al_missiley1+4){
			missiley=0;
			al_missiley1=0;
			sound(2);
		}
		if(missilex==al_missilex2 && missiley>=al_missiley2 && missiley<al_missiley2+4){
			missiley=0;
			al_missiley2=0;
			sound(2);
		}
	}

	// 自機と敵ミサイルチェック
	if(explodecounter==0){
		if(al_missiley1>181 && al_missiley1<192 && al_missilex1>=cannonx+2 && al_missilex1<=cannonx+14 ||
		  (al_missiley2>181 && al_missiley2<192 && al_missilex2>=cannonx+2 && al_missilex2<=cannonx+14)){
			explodecounter=120;
			sound(5);
		}
	}

	// ミサイルとトーチカのチェック
	if(missiley>160){
		s=getColor(missilex,missiley)|getColor(missilex,missiley+1)|
			getColor(missilex,missiley+2)|getColor(missilex,missiley+3);
		if(s){
			boxfill(missilex-1,missiley,missilex+1,missiley+3,0);
			missiley=0;
		}
	}
	if(al_missiley1>160){
		s=getColor(al_missilex1,al_missiley1)|getColor(al_missilex1,al_missiley1+1)|
			getColor(al_missilex1,al_missiley1+2)|getColor(al_missilex1,al_missiley1+3);
		if(s){
			boxfill(al_missilex1-1,al_missiley1-1,al_missilex1+2,al_missiley1+5,0);
			al_missiley1=0;
		}
	}
	if(al_missiley2>160){
		s=getColor(al_missilex2,al_missiley2)|getColor(al_missilex2,al_missiley2+1)|
			getColor(al_missilex2,al_missiley2+2)|getColor(al_missilex2,al_missiley2+3);
		if(s){
			boxfill(al_missilex2-1,al_missiley2-1,al_missilex2+2,al_missiley2+5,0);
			al_missiley2=0;
		}
	}
}
void putmissile(void){
//ミサイル表示
	if(missiley>0) putbmpmn(missilex,missiley,1,4,bmp_missile1);
	else if(missiley<=-2) putfont(missilex-3,0,2,0,0x90); //てっぺんで爆発中
	if(al_missiley1>0) putbmpmn(al_missilex1,al_missiley1,2,4,bmp_missile2);
	if(al_missiley2>0) putbmpmn(al_missilex2,al_missiley2,2,4,bmp_missile2);
	if(al_missiley1<0) putfont(al_missilex1-2,198,2,0,0x91); //地面で爆発中
	if(al_missiley2<0) putfont(al_missilex2-2,198,2,0,0x91); //地面で爆発中
}
void putalien1(int x,int y,int n){
//インベーダーを1個表示
	int c,p;
	if(n==-1){
		//1個分消去
		boxfill(x,y,x+15,y+7,0);
		return;
	}
	if(al_conter>0) return;
	if(n<0) p=0x8c; //爆発中
	else p=0x80+(n-1)*4+al_animation*2;

	//行によって色を変える
	if(y<8) c=2;
	else if(y<32) c=3;
	else if(y<64) c=4;
	else if(y<96) c=5;
	else if(y<128) c=3;
	else if(y<160) c=6;
	else c=2;
	putfont(x,y,c,0,p);
	putfont(x+8,y,c,0,p+1);
}
void putaliens(void){
//インベーダーの全体表示
	int x,y,i,j,w,n;
	x=al_x;
	y=al_y;
	for(j=0;j<=4;j++){
		w=x;
		for(i=0;i<=10;i++){
			n=alien[j][i];
			if(n) putalien1(w,y,n);
			if(n<0) alien[j][i]++; //爆発中カウンター
			w+=16;
		}
		y+=16;
	}
}
void putufo(void){
//UFO表示
	if(ufox<0) return;
	if(ufo_counter>=0){
		printstr(ufox,8,3,0,"\xb0\xb1\xb2");
	}
	else if(ufo_counter==-24){
		//UFO爆発中
		printstr(ufox,8,3,0,"\xb3\xb4\xb5");
		sound(2);
	}
	else if(ufo_counter==-18){
		//UFO得点表示中
		printnum(ufox,8,3,0,ufo_score);
		sound(4);
	}
}
void putcannon(void){
//自機表示
	if(explodecounter==0){
		printstr(cannonx,184,5,0,"\xa0\xa1");
	}
	else{
		//爆発中
		if(explodecounter & 2) printstr(cannonx,184,5,0,"\xa2\xa3");
		else printstr(cannonx,184,5,0,"\xa4\xa5");
	}
}
int checkgame(void){
//ゲームステータスを更新
	int a,i,s;
	if(explodecounter==120){
		//自機がやられた直後
		zanki--;
		if(zanki==0) return 2;//ゲームオーバー
	}
	if(al_zan==0) return 1; //敵全滅、次ステージへ
	if(al_y>=120){
		s=0;
		a=11-(al_y-8)/16;
		for(i=0;i<=10;i++) s+=alien[a][i];
		if(s>0) return 2;//インベーダーが地面まで侵略してゲームオーバー
	}
	return 0;
}
void gameover(void){
//ゲームオーバー処理
	putzanki();
	printstr(74,130,4,0,"GAME OVER");
	wait60thsecwithsound(240);
#ifdef BOOTLOADER
	if(highscore>read_recscore(0)){
		write_recscore(highscore,0);//ブートローダ対応共通領域のレコードにハイスコア書き込み
	}
#else
	if(highscore>*(unsigned int *)FLASHPAGE){
		writehighscore();
	}
#endif
}

void nextstage(void){
//次ステージへ進む処理
	int i,x;
	stage++;
	if(stage>=2){ //ステージ1の場合は飛ばす
		printstr(40,60,6,0,"CONGRATULATIONS!");
		wait60thsecwithsound(60);
		printstr(60,85,6,0,"NEXT STAGE");
		wait60thsec(120);
	}
	clearscreen();
	printstr(16,296,5,0,"SCORE");
	printstr(128,296,2,0,"HI-SCORE");
	printstr(144,208,4,0,"STAGE");
	printnum(192,208,5,0,stage);
	printscore();
	putzanki();

	//インベーダー初期化
	for(i=0;i<=10;i++) alien[0][i]=3;
	for(i=0;i<=10;i++) alien[1][i]=2;
	for(i=0;i<=10;i++) alien[2][i]=2;
	for(i=0;i<=10;i++) alien[3][i]=1;
	for(i=0;i<=10;i++) alien[4][i]=1;

	boxfill(0,206,215,207,2);//地面表示

	//トーチカ表示
	x=21;
	for(i=1;i<=4;i++){
		printstr(x,160,2,0,"\xf0\xf1\xf2");
		printstr(x,168,2,0,"\xf3\xf4\xf5");
		x+=48;
	}

	printstr(80,100,4,0,"STAGE ");
	printnum(128,100,4,0,stage);
	wait60thsec(180);
	boxfill(60,100,150,107,0);

	//各種パラーメータ設定
	al_x=16;
	al_y=((stage-1)%8)*8+32; //ステージによってインベーダー高さ設定
	al_dir=2;
	cannonx=8;
	ufox=-1;
	ufo_counter=0;
	ufo_dir=1;
	explodecounter=0;
	missiley=0;
	al_animation=0;
	al_conter=0;
	al_zan=55;
	al_missiley1=0;
	al_missiley2=0;
	al_missilecount=50;
	keycheck();
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

	init_graphic();//グラフィック使用開始
	LCD_WriteIndex(0x6a);//垂直表示開始位置設定（24行下にスクロール）ILI9325
//	LCD_WriteData(320-24);
	LCD_WriteData(272-24);

	while(1){
		initgame(); //ゲーム初期化
		do{
			nextstage(); //次ステージへ
			do{
				wait60thsecwithsound(1); //60分の1秒ウェイトと効果音更新
				clearchar(); //キャラクター表示消去
				button(); //ボタン押下状態読み込み
				fire(); //ミサイル発射処理
				movecannon(); //自機移動
				movealien(); //インベーダー移動
				moveufo(); //UFO移動
				movemissile(); //ミサイル移動
				checkcollision(); //各種衝突チェック
				putmissile(); //ミサイル表示
				putaliens(); //インベーダー表示
				putufo(); //UFO表示
				putcannon(); //自機表示
				printscore(); //得点表示
				gamestatus=checkgame(); //ゲームステータス更新
			} while(gamestatus==0);
			//gamestatus 0:通常、1:次ステージへ、2:ゲームオーバー
		} while(gamestatus==1);
		gameover(); //ゲームオーバー
	}
}
