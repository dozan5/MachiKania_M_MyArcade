// 回転スクロールシューティングゲーム VELUDDA用キャラクター定義 by K.Tanaka
// veludda_chars.c

#include "veludda.h"

// 自機画像
const unsigned char Bmp_ship[SQSIZE_SHIP]={
	0,0,0,0,0,15,7,7,7,7,15,0,0,0,0,0,
	0,0,0,0,7,7,7,7,7,7,7,7,0,0,0,0,
	0,0,15,7,7,7,7,7,7,7,7,7,7,15,0,0,
	0,0,7,7,7,7,7,2,2,7,7,7,7,7,0,0,
	0,7,7,7,7,7,2,2,2,2,7,7,7,7,7,0,
	15,7,7,7,7,7,7,7,7,7,7,7,7,7,7,15,
	7,7,7,7,2,7,7,0,0,7,7,2,7,7,7,7,
	7,7,7,2,2,7,0,0,0,0,7,2,2,7,7,7,
	7,7,7,2,2,7,0,0,0,0,7,2,2,7,7,7,
	7,7,7,7,2,7,7,0,0,7,7,2,7,7,7,7,
	15,7,7,7,7,7,7,7,7,7,7,7,7,7,7,15,
	0,7,7,7,7,7,2,2,2,2,7,7,7,7,7,0,
	0,0,7,7,7,7,7,2,2,7,7,7,7,7,0,0,
	0,0,15,7,7,7,7,7,7,7,7,7,7,15,0,0,
	0,0,0,0,7,7,7,7,7,7,7,7,0,0,0,0,
	0,0,0,0,0,15,7,7,7,7,15,0,0,0,0,0
};
// 地上敵画像
const unsigned char Bmp_gndenemy[][SQSIZE_GNDENEMY]={
{
	0,0,0,0,7,7,7,7,7,7,0,0,0,0,
	0,0,0,7,7,7,7,7,7,7,7,0,0,0,
	0,0,7,7,7,7,7,7,7,7,7,7,0,0,
	0,7,7,7,10,10,10,10,10,10,7,7,7,0,
	7,7,7,10,10,10,8,8,10,10,10,7,15,15,
	7,7,7,10,10,8,12,12,8,10,10,15,15,15,
	7,7,7,10,8,12,12,12,12,8,10,15,15,15,
	7,7,7,10,8,12,12,12,12,8,10,15,15,15,
	7,7,7,10,10,8,12,12,8,10,10,15,15,15,
	7,7,7,10,10,10,8,8,10,10,10,15,15,15,
	0,7,7,7,10,10,10,10,10,10,15,15,15,0,
	0,0,7,7,15,15,15,15,15,15,15,15,0,0,
	0,0,0,15,15,15,15,15,15,15,15,0,0,0,
	0,0,0,0,15,15,15,15,15,15,0,0,0,0
},
{
	0,0,0,0,7,7,7,7,7,7,0,0,0,0,
	0,0,0,7,7,7,7,7,7,7,7,0,0,0,
	0,0,7,7,7,7,7,7,7,7,7,7,0,0,
	0,7,7,7,10,10,10,10,10,10,7,7,7,0,
	7,7,7,10,10,10,8,8,10,10,10,7,15,15,
	7,7,7,10,10,8,12,12,8,10,10,15,15,15,
	7,7,7,10,8,12,6,6,12,8,10,15,15,15,
	7,7,7,10,8,12,6,6,12,8,10,15,15,15,
	7,7,7,10,10,8,12,12,8,10,10,15,15,15,
	7,7,7,10,10,10,8,8,10,10,10,15,15,15,
	0,7,7,7,10,10,10,10,10,10,15,15,15,0,
	0,0,7,7,15,15,15,15,15,15,15,15,0,0,
	0,0,0,15,15,15,15,15,15,15,15,0,0,0,
	0,0,0,0,15,15,15,15,15,15,0,0,0,0
},
{
	0,0,0,0,7,7,7,7,7,7,0,0,0,0,
	0,0,0,7,7,7,7,7,7,7,7,0,0,0,
	0,0,7,7,7,7,7,7,7,7,7,7,0,0,
	0,7,7,7,10,10,10,10,10,10,7,7,7,0,
	7,7,7,10,10,10,8,8,10,10,10,7,15,15,
	7,7,7,10,10,8,6,6,8,10,10,15,15,15,
	7,7,7,10,8,6,2,2,6,8,10,15,15,15,
	7,7,7,10,8,6,2,2,6,8,10,15,15,15,
	7,7,7,10,10,8,6,6,8,10,10,15,15,15,
	7,7,7,10,10,10,8,8,10,10,10,15,15,15,
	0,7,7,7,10,10,10,10,10,10,15,15,15,0,
	0,0,7,7,15,15,15,15,15,15,15,15,0,0,
	0,0,0,15,15,15,15,15,15,15,15,0,0,0,
	0,0,0,0,15,15,15,15,15,15,0,0,0,0
},
{
	0,0,0,0,7,7,7,7,7,7,0,0,0,0,
	0,0,0,7,7,7,7,7,7,7,7,0,0,0,
	0,0,7,7,7,7,7,7,7,7,7,7,0,0,
	0,7,7,7,10,10,10,10,10,10,7,7,7,0,
	7,7,7,10,10,10,8,8,10,10,10,7,15,15,
	7,7,7,10,10,8,2,2,8,10,10,15,15,15,
	7,7,7,10,8,2,2,2,2,8,10,15,15,15,
	7,7,7,10,8,2,2,2,2,8,10,15,15,15,
	7,7,7,10,10,8,2,2,8,10,10,15,15,15,
	7,7,7,10,10,10,8,8,10,10,10,15,15,15,
	0,7,7,7,10,10,10,10,10,10,15,15,15,0,
	0,0,7,7,15,15,15,15,15,15,15,15,0,0,
	0,0,0,15,15,15,15,15,15,15,15,0,0,0,
	0,0,0,0,15,15,15,15,15,15,0,0,0,0
},
{
	0,0,0,0,7,7,7,8,7,7,0,0,0,0,
	0,0,0,7,8,7,8,7,7,7,7,0,0,0,
	0,0,7,7,15,15,7,7,8,8,7,8,0,0,
	0,8,7,15,8,10,10,8,10,8,7,8,7,0,
	7,7,7,8,8,8,8,8,10,8,10,7,15,15,
	7,8,8,10,10,8,15,15,8,10,10,15,8,15,
	7,7,8,8,8,15,8,8,15,8,8,8,15,15,
	7,15,8,10,8,15,8,15,15,8,10,8,8,15,
	7,15,15,8,8,8,15,15,8,10,10,15,15,15,
	7,7,7,10,10,8,8,8,8,8,8,15,8,15,
	0,7,8,8,10,8,10,10,8,10,15,8,15,0,
	0,0,7,7,15,8,15,15,15,8,15,8,0,0,
	0,0,0,15,15,15,8,8,15,8,15,0,0,0,
	0,0,0,0,15,15,15,15,15,15,0,0,0,0
}
};

//空中敵ビットマップ
//敵種類ごとに4パターン用意
//配列一番目は敵番号*4+パターン番号
const unsigned char Bmp_enemy[][SQSIZE_ENEMY]={
//敵番号0
{
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,7,5,5,7,0,0,0,0,
	0,0,0,7,7,5,5,7,7,0,0,0,
	0,15,15,7,5,5,5,5,7,15,15,0,
	6,6,15,7,5,5,5,5,7,15,6,6,
	6,6,15,7,7,5,5,7,7,15,6,6,
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,15,7,7,7,7,7,7,15,0,0,
	0,0,15,7,7,2,2,7,7,15,0,0
},
{
	0,0,0,0,6,6,0,0,0,0,0,0,
	0,0,0,0,6,6,15,0,0,0,0,0,
	15,15,0,0,15,15,15,0,0,0,0,0,
	7,7,0,0,7,7,7,7,0,0,0,0,
	7,7,7,0,7,5,5,7,7,0,0,0,
	2,7,7,7,5,5,5,5,5,7,7,7,
	2,7,7,7,5,5,5,5,5,7,7,7,
	7,7,7,0,7,5,5,7,7,0,0,0,
	7,7,0,0,7,7,7,7,0,0,0,0,
	15,15,0,0,15,15,15,0,0,0,0,0,
	0,0,0,0,6,6,15,0,0,0,0,0,
	0,0,0,0,6,6,0,0,0,0,0,0
},
{
	0,0,15,7,7,2,2,7,7,15,0,0,
	0,0,15,7,7,7,7,7,7,15,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0,
	6,6,15,7,7,5,5,7,7,15,6,6,
	6,6,15,7,5,5,5,5,7,15,6,6,
	0,15,15,7,5,5,5,5,7,15,15,0,
	0,0,0,7,7,5,5,7,7,0,0,0,
	0,0,0,0,7,5,5,7,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0
},
{
	0,0,0,0,0,0,6,6,0,0,0,0,
	0,0,0,0,0,15,6,6,0,0,0,0,
	0,0,0,0,0,15,15,15,0,0,15,15,
	0,0,0,0,7,7,7,7,0,0,7,7,
	0,0,0,7,7,5,5,7,0,7,7,7,
	7,7,7,5,5,5,5,5,7,7,7,2,
	7,7,7,5,5,5,5,5,7,7,7,2,
	0,0,0,7,7,5,5,7,0,7,7,7,
	0,0,0,0,7,7,7,7,0,0,7,7,
	0,0,0,0,0,15,15,15,0,0,15,15,
	0,0,0,0,0,15,6,6,0,0,0,0,
	0,0,0,0,0,0,6,6,0,0,0,0
},
//敵番号1
{
	0,0,0,0,6,7,7,6,0,0,0,0,
	0,0,0,6,6,7,7,6,6,0,0,0,
	0,0,6,6,5,7,7,2,6,6,0,0,
	0,6,6,5,5,7,7,2,2,6,6,0,
	6,6,5,5,5,7,7,2,2,2,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,
	6,6,2,2,2,7,7,5,5,5,6,6,
	0,6,6,2,2,7,7,5,5,6,6,0,
	0,0,6,6,2,7,7,5,6,6,0,0,
	0,0,0,6,6,7,7,6,6,0,0,0,
	0,0,0,0,6,7,7,6,0,0,0,0
},
{
	0,0,0,6,6,6,6,6,6,0,0,0,
	0,0,7,6,6,6,6,6,6,7,0,0,
	0,7,7,7,5,5,5,5,7,7,7,0,
	6,6,7,7,7,5,5,7,7,7,6,6,
	6,6,2,7,7,7,7,7,7,2,6,6,
	6,6,2,2,7,7,7,7,2,2,6,6,
	6,6,2,2,7,7,7,7,2,2,6,6,
	6,6,2,7,7,7,7,7,7,2,6,6,
	6,6,7,7,7,5,5,7,7,7,6,6,
	0,7,7,7,5,5,5,5,7,7,7,0,
	0,0,7,6,6,6,6,6,6,7,0,0,
	0,0,0,6,6,6,6,6,6,0,0,0
},
{
	0,0,0,0,6,7,7,6,0,0,0,0,
	0,0,0,6,6,7,7,6,6,0,0,0,
	0,0,6,6,2,7,7,5,6,6,0,0,
	0,6,6,2,2,7,7,5,5,6,6,0,
	6,6,2,2,2,7,7,5,5,5,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,
	6,6,5,5,5,7,7,2,2,2,6,6,
	0,6,6,5,5,7,7,2,2,6,6,0,
	0,0,6,6,5,7,7,2,6,6,0,0,
	0,0,0,6,6,7,7,6,6,0,0,0,
	0,0,0,0,6,7,7,6,0,0,0,0
},
{
	0,0,0,6,6,6,6,6,6,0,0,0,
	0,0,7,6,6,6,6,6,6,7,0,0,
	0,7,7,7,2,2,2,2,7,7,7,0,
	6,6,7,7,7,2,2,7,7,7,6,6,
	6,6,5,7,7,7,7,7,7,5,6,6,
	6,6,5,5,7,7,7,7,5,5,6,6,
	6,6,5,5,7,7,7,7,5,5,6,6,
	6,6,5,7,7,7,7,7,7,5,6,6,
	6,6,7,7,7,2,2,7,7,7,6,6,
	0,7,7,7,2,2,2,2,7,7,7,0,
	0,0,7,6,6,6,6,6,6,7,0,0,
	0,0,0,6,6,6,6,6,6,0,0,0
},
//敵番号2
{
	7,7,0,0,0,0,0,0,8,0,0,0,
	0,7,7,0,0,0,0,0,0,8,0,0,
	0,0,7,0,0,0,0,0,0,0,8,0,
	0,0,7,7,0,0,0,0,0,0,8,0,
	0,0,7,7,7,7,15,15,6,6,6,0,
	0,0,0,7,7,7,2,2,2,2,6,0,
	0,0,0,0,15,2,4,4,4,4,6,6,
	0,0,0,0,15,2,4,4,4,8,4,6,
	8,0,0,0,6,2,4,4,4,8,4,6,
	0,8,0,0,6,2,4,8,8,4,4,4,
	0,0,8,8,6,6,6,4,4,4,4,0,
	0,0,0,0,0,0,6,6,6,4,0,0
},
{
	0,0,0,8,0,0,0,0,0,0,7,7,
	0,0,8,0,0,0,0,0,0,7,7,0,
	0,8,0,0,0,0,0,0,0,7,0,0,
	0,8,0,0,0,0,0,0,7,7,0,0,
	0,6,6,6,15,15,7,7,7,7,0,0,
	0,6,2,2,2,2,7,7,7,0,0,0,
	6,6,4,4,4,4,2,15,0,0,0,0,
	6,4,8,4,4,4,2,15,0,0,0,0,
	6,4,8,4,4,4,2,6,0,0,0,8,
	4,4,4,8,8,4,2,6,0,0,8,0,
	0,4,4,4,4,6,6,6,8,8,0,0,
	0,0,4,6,6,6,0,0,0,0,0,0
},
{
	0,0,4,6,6,6,0,0,0,0,0,0,
	0,4,4,4,4,6,6,6,8,8,0,0,
	4,4,4,8,8,4,2,6,0,0,8,0,
	6,4,8,4,4,4,2,6,0,0,0,8,
	6,4,8,4,4,4,2,15,0,0,0,0,
	6,6,4,4,4,4,2,15,0,0,0,0,
	0,6,2,2,2,2,7,7,7,0,0,0,
	0,6,6,6,15,15,7,7,7,7,0,0,
	0,8,0,0,0,0,0,0,7,7,0,0,
	0,8,0,0,0,0,0,0,0,7,0,0,
	0,0,8,0,0,0,0,0,0,7,7,0,
	0,0,0,8,0,0,0,0,0,0,7,7
},
{
	0,0,0,0,0,0,6,6,6,4,0,0,
	0,0,8,8,6,6,6,4,4,4,4,0,
	0,8,0,0,6,2,4,8,8,4,4,4,
	8,0,0,0,6,2,4,4,4,8,4,6,
	0,0,0,0,15,2,4,4,4,8,4,6,
	0,0,0,0,15,2,4,4,4,4,6,6,
	0,0,0,7,7,7,2,2,2,2,6,0,
	0,0,7,7,7,7,15,15,6,6,6,0,
	0,0,7,7,0,0,0,0,0,0,8,0,
	0,0,7,0,0,0,0,0,0,0,8,0,
	0,7,7,0,0,0,0,0,0,8,0,0,
	7,7,0,0,0,0,0,0,8,0,0,0
},
//敵番号3
{
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,5,5,4,4,7,7,4,4,5,5,0,
	0,0,5,4,4,7,7,4,4,5,0,0,
	7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,
	0,0,5,4,4,7,7,4,4,5,0,0,
	0,5,5,4,4,7,7,4,4,5,5,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0
},
{
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,5,5,10,10,7,7,10,10,5,5,0,
	0,0,5,10,10,7,7,10,10,5,0,0,
	7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,
	0,0,5,10,10,7,7,10,10,5,0,0,
	0,5,5,10,10,7,7,10,10,5,5,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0
},
{
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,5,5,6,6,7,7,6,6,5,5,0,
	0,0,5,6,6,7,7,6,6,5,0,0,
	7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,
	0,0,5,6,6,7,7,6,6,5,0,0,
	0,5,5,6,6,7,7,6,6,5,5,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0
},
{
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,5,5,2,2,7,7,2,2,5,5,0,
	0,0,5,2,2,7,7,2,2,5,0,0,
	7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,
	0,0,5,2,2,7,7,2,2,5,0,0,
	0,5,5,2,2,7,7,2,2,5,5,0,
	0,5,5,5,5,7,7,5,5,5,5,0,
	0,0,5,5,0,7,7,0,5,5,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0
},
//敵番号4
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,7,7,7,7,7,7,7,7,0,0,
	0,7,7,10,10,10,10,10,10,7,7,0,
	0,0,7,7,10,10,10,10,7,7,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,0,6,6,0,0,0,0,0,
	6,6,6,6,6,6,6,6,6,6,6,6,
	0,6,6,6,6,6,6,6,6,6,6,0,
	0,0,0,6,6,6,6,6,6,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0
},
{
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,7,10,10,10,10,7,0,0,0,
	0,0,7,10,10,10,10,10,10,7,0,0,
	0,0,7,7,10,10,10,10,7,7,0,0,
	0,0,0,7,7,7,7,7,7,0,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,6,6,6,6,6,6,0,0,0,
	0,0,6,6,6,6,6,6,6,6,0,0,
	0,0,6,6,6,6,6,6,6,6,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0
},
{
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,7,7,10,10,7,7,0,0,0,
	0,0,0,7,7,10,10,7,7,0,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,0,0,7,7,0,0,0,0,0,
	0,0,0,0,0,6,6,0,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,0,0,6,6,0,0,0,0,0
},
{
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,7,10,10,10,10,7,0,0,0,
	0,0,7,10,10,10,10,10,10,7,0,0,
	0,0,7,7,10,10,10,10,7,7,0,0,
	0,0,0,7,7,7,7,7,7,0,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,6,6,6,6,6,6,0,0,0,
	0,0,6,6,6,6,6,6,6,6,0,0,
	0,0,6,6,6,6,6,6,6,6,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0,
	0,0,0,0,6,6,6,6,0,0,0,0
},
//敵番号5
{
	0,0,0,0,4,4,4,4,0,0,0,0,
	0,0,4,4,4,4,4,4,4,4,0,0,
	0,4,4,4,4,4,4,4,4,4,4,0,
	0,4,4,4,4,4,4,4,4,4,4,0,
	4,4,4,4,4,4,4,4,4,4,4,4,
	4,4,4,4,4,7,7,4,4,4,4,4,
	4,4,4,4,4,7,7,4,4,4,4,4,
	4,4,4,4,4,4,4,4,4,4,4,4,
	0,4,4,4,4,4,4,4,4,4,4,0,
	0,4,4,4,4,4,4,4,4,4,4,0,
	0,0,4,4,4,4,4,4,4,4,0,0,
	0,0,0,0,4,4,4,4,0,0,0,0
},
{
	0,0,0,0,4,4,4,4,0,0,0,0,
	0,0,4,4,4,4,4,4,4,4,0,0,
	0,4,4,4,4,4,4,4,4,4,4,0,
	0,4,4,4,4,7,7,4,4,4,4,0,
	4,4,4,4,7,7,7,7,4,4,4,4,
	4,4,4,7,7,4,4,7,7,4,4,4,
	4,4,4,7,7,4,4,7,7,4,4,4,
	4,4,4,4,7,7,7,7,4,4,4,4,
	0,4,4,4,4,7,7,4,4,4,4,0,
	0,4,4,4,4,4,4,4,4,4,4,0,
	0,0,4,4,4,4,4,4,4,4,0,0,
	0,0,0,0,4,4,4,4,0,0,0,0
},
{
	0,0,0,0,4,4,4,4,0,0,0,0,
	0,0,4,4,4,4,4,4,4,4,0,0,
	0,4,4,4,4,7,7,4,4,4,4,0,
	0,4,4,7,7,7,7,7,7,4,4,0,
	4,4,4,7,7,4,4,7,7,4,4,4,
	4,4,7,7,4,4,4,4,7,7,4,4,
	4,4,7,7,4,4,4,4,7,7,4,4,
	4,4,4,7,7,4,4,7,7,4,4,4,
	0,4,4,7,7,7,7,7,7,4,4,0,
	0,4,4,4,4,7,7,4,4,4,4,0,
	0,0,4,4,4,4,4,4,4,4,0,0,
	0,0,0,0,4,4,4,4,0,0,0,0
},
{
	0,0,0,0,7,7,7,7,0,0,0,0,
	0,0,7,7,7,4,4,7,7,7,0,0,
	0,7,7,4,4,4,4,4,4,7,7,0,
	0,7,4,4,4,4,4,4,4,4,7,0,
	7,7,4,4,4,4,4,4,4,4,7,7,
	7,4,4,4,4,4,4,4,4,4,4,7,
	7,4,4,4,4,4,4,4,4,4,4,7,
	7,7,4,4,4,4,4,4,4,4,7,7,
	0,7,4,4,4,4,4,4,4,4,7,0,
	0,7,7,4,4,4,4,4,4,7,7,0,
	0,0,7,7,7,4,4,7,7,7,0,0,
	0,0,0,0,7,7,7,7,0,0,0,0
},
};

//爆発パターンビットマップ1
const unsigned char Bmp_explode[][SQSIZE_EXPLODE]={
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,6,14,0,14,14,0,0,0,
	0,0,0,14,6,6,14,14,0,0,0,0,
	0,0,6,14,14,14,14,14,14,0,0,0,
	0,0,6,14,14,14,14,6,6,0,0,0,
	0,0,0,14,14,6,14,6,14,0,0,0,
	0,0,0,14,6,6,14,14,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,6,0,0,0,0,0,0,
	0,0,0,0,0,6,0,0,0,0,0,0,
	0,0,6,6,14,14,0,14,14,0,0,0,
	0,0,0,14,14,14,14,14,0,0,0,0,
	0,14,6,14,6,7,7,14,14,6,6,0,
	0,0,6,14,14,7,7,6,6,0,0,0,
	0,0,0,14,14,6,14,6,14,0,0,0,
	0,0,0,14,6,6,14,14,14,0,0,0,
	0,0,0,0,6,0,0,14,0,0,0,0,
	0,0,0,0,6,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,6,0,0,0,0,2,0,
	0,6,6,6,0,6,2,2,0,2,2,0,
	0,2,2,2,2,14,2,14,14,2,0,0,
	0,2,2,2,7,7,14,2,2,0,0,0,
	0,14,6,14,7,7,7,7,2,6,6,0,
	0,0,2,2,2,7,7,7,2,0,0,0,
	0,0,0,2,7,7,7,7,14,2,2,0,
	0,0,2,2,2,6,14,2,14,6,0,0,
	0,0,2,2,2,0,2,2,2,6,0,0,
	0,0,0,2,6,0,0,2,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
},
{
	2,0,0,2,0,2,0,0,2,0,0,0,
	0,2,0,2,0,2,0,2,0,0,2,0,
	0,6,2,14,2,2,2,2,0,2,2,0,
	0,2,2,14,6,2,14,14,14,2,0,0,
	0,2,14,6,6,6,6,6,14,0,0,0,
	0,14,6,6,14,6,6,2,2,2,2,2,
	2,2,2,6,14,6,6,14,6,6,2,0,
	0,6,6,6,14,2,6,14,6,14,6,0,
	0,0,2,6,6,2,14,2,14,6,0,0,
	0,2,2,14,14,2,6,6,2,6,0,0,
	2,2,0,2,2,0,14,14,14,2,0,0,
	0,0,0,0,2,0,0,0,0,0,2,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,6,0,0,0,0,0,0,
	0,0,6,6,2,6,2,2,0,0,0,0,
	0,0,14,2,2,2,2,14,2,2,0,0,
	0,14,14,2,14,2,2,14,2,0,0,0,
	0,14,2,2,2,14,2,2,2,14,0,0,
	0,0,2,14,2,14,2,2,2,14,0,0,
	0,0,2,14,14,2,2,14,2,2,2,0,
	0,0,2,2,14,2,14,14,2,6,0,0,
	0,0,14,2,2,2,2,2,2,6,0,0,
	0,0,0,14,14,0,0,2,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,2,2,2,2,0,0,0,0,
	0,0,0,2,14,2,14,14,2,0,0,0,
	0,0,14,14,2,2,2,2,2,0,0,0,
	0,0,0,14,2,14,14,2,14,0,0,0,
	0,0,0,14,2,2,2,2,14,0,0,0,
	0,0,0,2,14,2,14,14,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,2,2,0,0,0,0,0,
	0,0,0,0,10,10,10,10,0,0,0,0,
	0,0,0,2,10,2,2,10,10,0,0,0,
	0,0,0,2,10,10,10,10,10,0,0,0,
	0,0,0,0,2,10,10,2,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,10,2,0,0,0,0,0,0,
	0,0,0,0,2,10,10,10,0,0,0,0,
	0,0,0,0,0,10,2,0,0,0,0,0,
	0,0,0,0,10,2,10,2,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
}};
//爆発パターンビットマップ2
const unsigned char Bmp_explode2[][SQSIZE_EXPLODE2]={
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,
	0,0,0,0,0,14,6,6,6,14,0,0,0,0,0,0,
	0,0,0,0,0,14,6,6,14,14,6,0,0,0,0,0,
	0,0,0,0,6,6,14,14,6,14,6,6,0,0,0,0,
	0,0,0,0,6,6,14,14,6,6,14,0,0,0,0,0,
	0,0,0,0,0,14,6,6,6,6,14,0,0,0,0,0,
	0,0,0,0,0,14,6,0,14,14,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,
	0,0,0,6,6,6,14,6,6,6,14,0,0,0,0,0,
	0,0,0,0,14,14,14,6,6,6,14,6,0,0,0,0,
	0,0,0,0,0,14,6,6,6,14,14,6,0,0,0,0,
	0,0,0,0,0,14,6,7,14,7,7,6,0,0,0,0,
	0,6,6,6,6,6,14,7,7,14,6,6,0,0,0,0,
	0,0,0,0,6,6,14,7,6,7,14,14,14,14,0,0,
	0,0,0,0,0,14,6,6,6,6,14,6,14,0,0,0,
	0,0,0,0,0,14,6,14,14,14,6,6,6,0,0,0,
	0,0,0,0,0,0,6,14,0,0,6,0,0,0,0,0,
	0,0,0,6,6,6,6,6,0,0,6,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,2,0,0,14,0,2,0,0,0,0,0,0,0,
	0,0,0,2,6,6,14,6,2,6,14,0,0,0,0,0,
	0,6,6,2,14,14,7,7,6,6,2,2,0,0,0,0,
	0,2,2,2,2,14,7,7,7,7,14,6,6,6,0,0,
	0,0,14,14,14,14,7,7,7,7,7,14,14,6,0,0,
	0,2,2,2,6,6,14,7,7,7,6,2,2,0,0,0,
	0,0,0,6,6,6,14,7,6,7,14,2,2,14,0,0,
	0,0,0,6,6,14,7,7,7,6,14,2,2,2,6,0,
	0,0,0,2,2,2,6,14,14,2,2,6,6,6,0,0,
	0,0,0,0,0,0,6,6,14,2,6,14,14,0,0,0,
	0,0,0,6,2,2,6,6,14,0,6,2,2,0,0,0,
	0,0,0,0,2,6,0,0,0,0,0,0,2,2,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,
	2,2,0,0,6,2,0,0,2,0,2,6,0,0,2,0,
	0,2,2,2,6,2,6,0,2,0,2,6,6,2,2,0,
	0,0,2,2,6,6,6,6,2,6,2,6,2,2,6,0,
	0,6,6,6,14,14,6,14,6,6,2,2,2,6,0,0,
	0,2,2,2,6,14,6,7,14,14,14,6,6,6,0,0,
	0,6,6,6,14,14,6,7,6,14,14,2,2,2,2,2,
	0,2,2,2,6,6,14,2,6,7,6,2,2,6,0,0,
	0,0,14,14,6,6,14,2,6,7,14,2,2,6,0,0,
	0,0,6,6,6,14,14,2,14,6,14,14,2,2,2,0,
	2,2,2,2,2,2,6,14,14,2,2,6,6,2,0,0,
	0,0,6,6,6,6,2,6,14,2,2,14,6,6,0,0,
	0,2,2,2,2,2,2,6,14,6,2,6,2,6,6,0,
	2,2,6,6,2,6,6,2,6,6,2,6,2,2,2,2,
	0,0,0,6,2,6,0,2,0,2,2,6,6,2,0,0,
	0,0,0,0,2,0,0,0,0,2,0,0,0,2,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,14,14,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,2,2,2,2,6,14,0,0,0,0,0,
	0,0,0,0,14,2,14,6,2,6,14,6,0,0,0,0,
	0,0,0,14,14,14,6,6,2,2,2,6,14,0,0,0,
	0,0,0,14,2,14,6,2,2,2,2,2,14,0,0,0,
	0,0,0,6,2,6,14,2,2,2,2,6,14,0,0,0,
	0,0,6,6,2,2,2,2,2,2,14,14,14,14,0,0,
	0,0,0,2,2,2,2,6,6,6,2,6,14,14,0,0,
	0,0,0,2,2,14,2,2,2,14,2,14,0,0,0,0,
	0,0,14,14,14,2,6,14,14,14,6,0,0,0,0,0,
	0,0,0,6,6,6,6,6,0,0,6,0,0,0,0,0,
	0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,14,14,0,0,0,14,14,0,0,0,0,
	0,0,0,0,0,14,14,14,14,14,2,2,0,0,0,0,
	0,0,0,0,0,2,2,2,14,2,14,14,0,0,0,0,
	0,0,0,0,0,2,2,2,14,2,2,0,0,0,0,0,
	0,0,0,0,14,14,14,2,14,2,14,14,0,0,0,0,
	0,0,0,0,0,2,2,2,2,2,2,14,0,0,0,0,
	0,0,0,0,0,14,14,2,14,14,2,14,0,0,0,0,
	0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,
	0,0,0,0,0,2,2,10,10,10,2,0,0,0,0,0,
	0,0,0,0,0,10,10,2,2,10,2,2,0,0,0,0,
	0,0,0,0,2,2,2,10,10,10,10,10,0,0,0,0,
	0,0,0,0,0,0,2,10,10,2,2,0,0,0,0,0,
	0,0,0,0,0,0,0,2,10,10,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,
	0,0,0,0,0,10,10,2,10,0,0,0,0,0,0,0,
	0,0,0,0,0,0,10,2,2,10,0,0,0,0,0,0,
	0,0,0,0,0,0,0,10,10,10,0,0,0,0,0,0,
	0,0,0,0,0,0,0,10,2,2,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
}};
const unsigned char Bmp_cannon[][SQSIZE_CANNON]={
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,7,7,0,0,0,
	0,0,0,7,7,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,2,2,0,0,0,
	0,0,2,7,7,2,0,0,
	0,0,2,7,7,2,0,0,
	0,0,0,2,2,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,
	0,0,0,2,2,0,0,0,
	0,0,2,7,7,2,0,0,
	0,2,7,7,7,7,2,0,
	0,2,7,7,7,7,2,0,
	0,0,2,7,7,2,0,0,
	0,0,0,2,2,0,0,0,
	0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,
	0,0,2,2,2,2,0,0,
	0,2,10,7,7,10,2,0,
	0,2,7,7,7,7,2,0,
	0,2,7,7,7,7,2,0,
	0,2,10,7,7,10,2,0,
	0,0,2,2,2,2,0,0,
	0,0,0,0,0,0,0,0
},
{
	0,0,0,2,2,0,0,0,
	0,0,2,10,10,2,0,0,
	0,2,7,7,7,7,2,0,
	2,10,7,7,7,7,10,2,
	2,10,7,7,7,7,10,2,
	0,2,7,7,7,7,2,0,
	0,0,2,10,10,2,0,0,
	0,0,0,2,2,0,0,0
},
{
	0,0,10,2,2,10,0,0,
	0,2,2,7,7,2,2,0,
	10,2,7,7,7,7,2,10,
	2,7,7,6,6,7,7,2,
	2,7,7,6,6,7,7,2,
	10,2,7,7,7,7,2,10,
	0,2,2,7,7,2,2,0,
	0,0,10,2,2,10,0,0
},
{
	0,0,2,2,2,2,0,0,
	0,2,7,7,7,7,2,0,
	2,7,7,6,6,7,7,2,
	2,7,6,7,7,6,7,2,
	2,7,6,7,7,6,7,2,
	2,7,7,6,6,7,7,2,
	0,2,7,7,7,7,2,0,
	0,0,2,2,2,2,0,0
},
{
	0,0,2,2,2,2,0,0,
	0,2,6,6,6,6,2,0,
	2,6,6,7,7,6,6,2,
	2,6,7,7,7,7,6,2,
	2,6,7,7,7,7,6,2,
	2,6,6,7,7,6,6,2,
	0,2,6,6,6,6,2,0,
	0,0,2,2,2,2,0,0
}};
const unsigned char Bmp_missile1[SQSIZE_MISSILE]={
	0,6,6,0,
	14,6,6,6,
	14,14,6,6,
	0,14,14,0
};
const unsigned char Bmp_missile2[SQSIZE_MISSILE]={
	0,2,2,0,
	10,2,2,2,
	10,10,2,2,
	0,10,10,0
};
const unsigned char Bmp_ship_mini[8*8]={
	0,0,0,7,7,0,0,0,
	0,15,7,7,7,7,15,0,
	0,7,7,7,7,7,7,0,
	7,7,7,15,15,7,7,7,
	7,7,7,15,15,7,7,7,
	0,7,7,7,7,7,7,0,
	0,15,7,7,7,7,15,0,
	0,0,0,7,7,0,0,0
};
const unsigned char Bmp_enemy_mini[8*8]={
	0,0,0,7,7,0,0,0,
	0,0,0,7,7,0,0,0,
	0,0,15,5,5,15,0,0,
	0,7,7,5,5,7,7,0,
	6,7,7,5,5,7,7,6,
	0,0,0,7,7,0,0,0,
	0,0,7,7,7,7,0,0,
	0,15,7,2,2,7,15,0
};
const unsigned char Bmp_logo[]={
	//色番号,ランレングス,・・・・
	//209×33ドット
	0,20,10,1,2,4,10,1,0,13,10,1,2,7,10,1,0,75,10,1,2,6,10,1,0,22,10,1,2,6,10,1,
	0,48,10,1,2,3,0,6,10,1,2,2,10,1,0,5,2,8,0,10,2,13,10,1,0,9,10,1,2,2,10,1,0,6,2,3,10,1,0,12,2,3,0,6,2,3,10,1,0,3,2,3,0,6,2,3,0,6,10,1,2,13,0,16,10,1,2,13,0,23,10,1,2,3,0,6,10,1,2,3,
	0,9,2,12,10,1,0,4,2,10,0,7,2,17,10,1,0,2,2,1,10,1,0,4,2,12,0,13,10,1,2,11,10,1,0,3,10,1,2,10,10,1,0,4,10,1,2,16,10,1,0,12,10,1,2,16,10,1,0,21,10,1,2,13,
	0,10,10,1,2,9,0,5,10,1,2,11,0,4,10,1,2,10,0,4,2,9,0,5,2,9,10,1,0,15,10,1,2,9,0,6,10,1,2,8,10,1,0,4,2,20,0,10,2,20,0,21,10,1,2,11,
	0,12,10,1,2,7,10,1,0,5,10,1,2,11,0,3,10,1,2,9,10,1,0,6,10,1,2,7,10,1,0,5,2,7,10,1,0,17,10,1,2,7,0,8,10,1,2,6,10,1,0,4,2,7,0,5,10,1,2,9,10,1,0,7,2,7,0,5,10,1,2,9,10,1,0,20,10,1,2,9,
	0,14,2,7,10,1,0,5,2,12,10,1,0,2,2,10,0,8,2,7,10,1,0,5,10,1,2,6,0,19,2,6,10,1,0,9,10,1,2,4,10,1,0,4,10,1,2,4,10,1,0,10,2,8,0,6,10,1,2,4,10,1,0,10,2,8,0,21,2,9,
	0,14,10,1,2,6,10,1,0,5,10,1,2,11,10,1,0,1,10,1,2,9,10,1,0,8,2,7,0,6,10,1,2,6,0,19,2,6,10,1,0,10,2,4,0,5,2,4,0,4,10,1,2,4,0,4,10,1,2,7,0,5,2,4,0,4,10,1,2,4,0,4,10,1,2,7,0,20,2,9,
	0,14,10,1,2,6,10,1,0,5,10,1,2,3,0,2,2,6,10,1,0,3,10,1,2,7,0,8,10,1,2,7,0,6,10,1,2,6,0,19,2,6,10,1,0,10,2,4,0,4,10,1,2,3,0,3,10,1,2,5,10,1,0,5,10,1,2,6,10,1,0,3,10,1,2,3,0,3,10,1,2,5,10,1,0,5,10,1,2,6,10,1,0,19,2,9,
	0,15,2,6,10,1,0,6,2,2,10,1,0,2,10,1,2,5,10,1,0,4,2,7,0,6,2,9,10,1,0,6,10,1,2,6,0,19,2,6,10,1,0,10,2,3,10,1,0,4,10,1,2,2,10,1,0,2,2,6,10,1,0,7,10,1,2,6,0,3,10,1,2,2,10,1,0,2,2,6,10,1,0,7,10,1,2,6,0,19,2,9,
	0,15,10,1,2,6,0,7,2,1,10,1,0,2,10,1,2,5,0,5,10,1,2,6,0,6,10,1,2,7,10,1,0,7,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,4,10,1,2,2,0,2,2,7,10,1,0,8,2,6,10,1,0,2,10,1,2,2,0,2,2,7,10,1,0,8,2,6,10,1,0,17,10,1,2,9,10,1,
	0,14,10,1,2,6,10,1,0,10,10,1,2,5,0,5,10,1,2,6,0,3,2,2,0,2,2,6,10,1,0,8,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,4,10,1,2,1,10,1,0,1,2,8,0,9,10,1,2,6,0,2,10,1,2,1,10,1,0,1,2,8,0,9,10,1,2,6,0,17,2,3,10,1,2,7,
	0,15,2,6,10,1,0,10,10,1,2,4,10,1,0,5,10,1,2,6,0,3,2,2,0,17,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,4,10,1,2,1,10,1,0,1,2,8,0,10,2,6,10,1,0,1,10,1,2,1,10,1,0,1,2,8,0,10,2,6,10,1,0,16,2,3,10,2,2,6,
	0,15,10,1,2,6,0,10,10,1,2,4,0,6,10,1,2,6,0,2,10,1,2,2,0,17,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,5,2,1,10,1,0,2,2,7,0,10,2,6,10,1,0,2,2,1,10,1,0,2,2,7,0,10,2,6,10,1,0,15,10,1,2,3,0,2,2,6,10,1,
	0,14,10,1,2,6,10,1,0,9,2,4,0,7,10,1,2,6,0,2,2,3,0,17,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,9,10,1,2,6,0,10,10,1,2,6,0,6,10,1,2,6,0,10,10,1,2,6,0,9,2,5,10,2,2,2,10,1,0,2,2,6,10,1,
	0,15,2,7,0,8,2,4,10,1,0,7,10,1,2,6,0,1,2,4,0,17,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,10,2,6,0,10,10,1,2,6,0,7,2,6,0,10,10,1,2,6,0,8,2,10,10,1,0,2,10,1,2,6,
	0,15,10,1,2,6,0,7,10,1,2,4,0,8,10,1,2,11,10,1,0,16,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,10,2,6,0,10,10,1,2,6,0,7,2,6,0,10,10,1,2,6,0,7,10,1,2,10,0,3,10,1,2,6,
	0,16,2,6,10,1,0,6,2,4,0,9,10,1,2,11,10,1,0,16,10,1,2,6,0,19,2,6,10,1,0,10,10,1,2,2,10,1,0,10,2,6,0,10,10,1,2,6,0,7,2,6,0,10,10,1,2,6,0,7,10,1,2,10,0,4,2,6,10,1,
	0,15,2,7,0,5,10,1,2,3,10,1,0,9,10,1,2,12,0,16,10,1,2,6,0,8,10,1,2,2,0,8,2,6,10,1,0,10,10,1,2,2,10,1,0,10,2,6,0,10,10,1,2,6,0,7,2,6,0,10,10,1,2,6,0,7,2,4,0,3,2,4,10,1,0,3,2,7,
	0,15,10,1,2,6,0,5,2,4,0,10,10,1,2,6,0,3,2,3,0,16,10,1,2,6,0,8,10,1,2,3,0,7,2,6,10,1,0,10,10,1,2,2,10,1,0,10,2,6,0,10,10,1,2,6,0,7,2,6,0,10,10,1,2,6,0,7,10,1,2,3,0,2,10,1,2,5,10,1,0,2,2,7,
	0,16,2,6,10,1,0,3,10,1,2,3,0,11,10,1,2,6,0,4,2,2,10,1,0,15,10,1,2,6,0,9,2,3,10,1,0,6,2,6,10,1,0,10,10,1,2,2,10,1,0,10,2,6,0,10,10,1,2,6,0,7,2,6,0,10,10,1,2,6,0,8,10,1,2,2,0,2,2,7,0,1,10,1,2,7,10,1,
	0,15,2,7,0,3,2,3,10,1,0,11,10,1,2,6,0,4,10,1,2,2,0,15,10,1,2,6,0,9,10,1,2,3,0,6,2,6,10,1,0,10,10,1,2,2,10,1,0,10,2,6,0,10,2,7,0,7,2,6,0,10,2,7,0,12,10,1,2,3,10,2,2,2,0,1,2,8,10,1,
	0,15,10,1,2,6,0,2,10,1,2,3,0,12,10,1,2,6,0,5,10,1,2,1,0,5,2,1,0,9,10,1,2,6,0,9,10,1,2,3,0,6,2,6,10,1,0,10,2,3,10,1,0,10,2,6,0,10,2,6,10,1,0,7,2,6,0,10,2,6,10,1,0,12,10,1,2,3,10,1,0,1,2,12,
	0,16,2,6,10,1,0,1,2,3,10,1,0,12,10,1,2,6,0,11,10,1,2,1,10,1,0,8,10,1,2,6,0,9,10,1,2,3,10,1,0,5,2,6,10,1,0,10,2,4,0,10,2,6,0,9,10,1,2,6,10,1,0,7,2,6,0,9,10,1,2,6,10,1,0,11,10,1,2,4,0,2,10,1,2,3,10,2,2,6,
	0,16,2,7,10,1,2,3,0,13,10,1,2,6,0,11,2,2,10,1,0,8,10,1,2,6,0,9,10,1,2,3,10,1,0,5,2,7,0,10,2,5,0,9,2,6,0,8,10,1,2,7,0,8,2,6,0,8,10,1,2,7,0,12,2,4,10,1,0,2,10,1,2,3,0,2,2,6,10,1,
	0,15,10,1,2,9,10,1,0,13,10,1,2,6,0,11,2,2,10,1,0,8,10,1,2,6,0,9,2,4,10,1,0,5,10,1,2,6,0,9,10,1,2,5,0,9,2,6,0,8,2,8,0,8,2,6,0,8,2,8,0,3,10,1,2,3,10,1,0,3,2,5,0,4,2,2,10,1,0,2,2,6,10,1,
	0,16,2,9,0,14,10,1,2,6,10,1,0,9,10,1,2,3,0,8,10,1,2,6,10,1,0,7,10,1,2,4,0,6,10,1,2,7,0,7,10,1,2,6,0,9,2,6,0,6,10,1,2,8,10,1,0,8,2,6,0,6,10,1,2,8,10,1,0,3,2,5,10,2,2,5,10,1,0,4,10,1,2,1,0,3,10,1,2,6,
	0,16,2,9,0,14,10,1,2,7,0,8,10,1,2,4,10,1,0,7,10,1,2,7,0,7,2,5,0,7,2,8,0,5,2,7,10,1,0,9,2,20,10,1,0,9,2,20,10,1,0,3,10,1,2,12,0,11,2,6,10,1,
	0,15,10,1,2,8,0,14,10,1,2,8,10,1,0,5,2,6,10,1,0,7,10,1,2,8,10,1,0,4,2,6,10,1,0,6,10,1,2,19,0,9,10,1,2,20,0,9,10,1,2,20,0,4,2,12,10,1,0,11,2,7,
	0,15,2,9,10,1,0,13,2,22,10,1,0,6,2,20,10,1,0,7,2,18,0,10,10,1,2,19,0,10,10,1,2,19,0,5,2,12,0,12,2,7,10,1,
	0,13,10,1,2,10,0,12,2,24,0,5,2,22,0,8,2,16,0,10,10,1,2,19,0,10,10,1,2,19,0,6,10,1,2,10,10,1,0,12,2,8,10,1,
	0,12,2,11,10,1,0,9,10,1,2,26,0,2,10,1,2,24,0,8,2,14,0,10,2,19,10,1,0,10,2,19,10,1,0,8,2,9,10,1,0,11,10,1,2,11,
	0,10,10,1,2,12,0,9,10,1,2,26,10,1,0,1,10,1,2,24,0,10,2,10,10,1,0,10,10,1,2,17,0,12,10,1,2,17,0,11,10,1,2,7,0,13,2,12,
	0,99,10,1,2,3,0,75,2,4,10,1,0,27
};
