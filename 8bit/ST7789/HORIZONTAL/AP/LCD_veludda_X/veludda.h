// veludda.h by K.Tanaka

// 入力ボタンのポート、ビット定義
#define KEYPORT PORTD
#define KEYDOWN 0x0001
#define KEYLEFT 0x0002
#define KEYUP 0x0004
#define KEYRIGHT 0x0008
#define KEYSTART 0x0010
#define KEYFIRE 0x0020

#define MAPDX (256*4) //全体マップの横サイズ（ビットマップデータは4分の1のみ）
#define MAPDY (128*4) //全体マップの縦サイズ（ビットマップデータは4分の1のみ）
#define NORMALIZEX(x) ((x)&(MAPDX*256-1)) //絶対座標の右端からはみ出した部分を左端からの位置にするマクロ
#define NORMALIZEY(y) ((y)&(MAPDY*256-1)) //絶対座標の下端からはみ出した部分を上端からの位置にするマクロ
//#define MAPDXMASK (MAPDX*256-1) //絶対座標の右端からはみ出した部分を左端にするためのマスク値
//#define MAPDYMASK (MAPDY*256-1) //絶対座標の下端からはみ出した部分を上端にするためのマスク値
#define ADX1 (128/4*3) //自機のX位置から画面左端までの距離
#define ADY1 (180/4*3) //自機のY位置から画面上端までの距離
#define ADX2 (X_RES/4*3-ADX1) //自機のX位置から画面右端までの距離
#define ADY2 ((Y_RES-TOPLINE)/4*3-ADY1) //自機のY位置から画面下端までの距離

#define MAX_GNDENEMY 50 //地上敵最大数
#define MAX_ENEMY 50 //空中敵最大数
#define MAX_MISSILE 20 //ミサイル最大数
#define MAX_CANNON 5 //砲弾最大数

#define XSIZE_GNDENEMY 14 //地上敵画像横サイズ
#define YSIZE_GNDENEMY 14 //地上敵画像縦サイズ
#define SQSIZE_GNDENEMY (XSIZE_GNDENEMY*YSIZE_GNDENEMY)
#define XSIZE_ENEMY 12 //空中敵画像横サイズ
#define YSIZE_ENEMY 12 //空中敵画像縦サイズ
#define SQSIZE_ENEMY (XSIZE_ENEMY*YSIZE_ENEMY)
#define XSIZE_SHIP 16 //自機画像横サイズ
#define YSIZE_SHIP 16 //自機画像縦サイズ
#define SQSIZE_SHIP (XSIZE_SHIP*YSIZE_SHIP)
#define XSIZE_EXPLODE 12 //敵爆発画像横サイズ
#define YSIZE_EXPLODE 12 //敵爆発画像縦サイズ
#define SQSIZE_EXPLODE (XSIZE_EXPLODE*YSIZE_EXPLODE)
#define XSIZE_EXPLODE2 16 //自機爆発画像横サイズ
#define YSIZE_EXPLODE2 16 //自機爆発画像縦サイズ
#define SQSIZE_EXPLODE2 (XSIZE_EXPLODE2*YSIZE_EXPLODE2)
#define XSIZE_MISSILE 4 //ミサイル画像横サイズ
#define YSIZE_MISSILE 4 //ミサイル画像縦サイズ
#define SQSIZE_MISSILE (XSIZE_MISSILE*YSIZE_MISSILE)
#define XSIZE_CANNON 8 //砲弾画像横サイズ
#define YSIZE_CANNON 8 //砲弾画像縦サイズ
#define SQSIZE_CANNON (XSIZE_CANNON*YSIZE_CANNON)
#define XSIZE_LOGO 209 //タイトルロゴ画像横サイズ
#define YSIZE_LOGO 33 //タイトルロゴ画像縦サイズ

#define SHIPS 5 //自機の初期残数
#define CANNONFIRE 80 //砲弾発射判断距離
#define CONTINUEMAX 3 //コンティニュー可能回数

//空中敵、地上敵用構造体定義
typedef struct {
	char on; // 有効／無効／爆発中
	int x; // x座標*256
	int y; // y座標*256
	short vx; // x方向移動速度*256
	short vy; // y方向移動速度*256
	unsigned char no; // 敵番号
	unsigned char count; // カウンタ
	const unsigned char *bmp; // 画像へのポインタ
} _Enemy;

//ミサイル、砲弾用構造体定義
typedef struct {
	char on; // 有効／無効
	int x; // x座標*256
	int y; // y座標*256
	short vx; // x方向移動速度*256
	short vy; // y方向移動速度*256
	unsigned char count; // カウンタ
	const unsigned char *bmp; // 画像へのポインタ
} _Missile;

//外部変数宣言
extern const unsigned char Bmp_ship[];
extern const unsigned char Bmp_gndenemy[][SQSIZE_GNDENEMY];
extern const unsigned char Bmp_enemy[][SQSIZE_ENEMY];
extern const unsigned char Bmp_explode[][SQSIZE_EXPLODE];
extern const unsigned char Bmp_explode2[][SQSIZE_EXPLODE2];
extern const unsigned char Bmp_missile1[SQSIZE_MISSILE];
extern const unsigned char Bmp_missile2[SQSIZE_MISSILE];
extern const unsigned char Bmp_cannon[][SQSIZE_CANNON];
extern const unsigned char Bmp_ship_mini[];
extern const unsigned char Bmp_enemy_mini[];
extern const unsigned char Bmp_logo[];

extern unsigned char mapdata[],paldata2[];//マップデータとパレットデータ

extern const unsigned short sounddata[];//音程データ
extern const unsigned short sounddata1[],sounddata2[],sounddata3[];//効果音データ
extern const unsigned char musicdata1[],musicdata2[],musicdata3[],musicdata4[];//曲データ
