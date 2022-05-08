//VELUDDA用　サウンド関連データ by K.Tanaka

//sounddata配列　低いドから3オクターブ分の周期カウンタ値、PR3に書き込むと音程設定される
const unsigned short sounddata[]={
	22803,21523,20315,19175,18098,17083,16124,15219,14365,13558,12797,12079,
	11401,10761,10157, 9587, 9049, 8541, 8062, 7609, 7182, 6779, 6398, 6039,
	 5700, 5380, 5078, 4793, 4524, 4270, 4031, 3804, 3591, 3389, 3199, 3019,
	 2850
};

//効果音データ
const unsigned short sounddata1[]={
//ミサイル発射音
	2000,2000,4000,4000,0
};
const unsigned short sounddata2[]={
//敵爆発音
	1500,30000,1,30000,1,30000,1,30000,1,30000,0
};
const unsigned short sounddata3[]={
//自機爆発音
	1000,2500,1,35000,35000,1,35000,1,36000,1,37000,1,38000,1,39000,1,40000,41000,42000,43000,
	44000,45000,46000,47000,48000,49000,50000,1,50000,1,50000,1,50000,1,50000,1,50000,0
};

//曲データ
//musicdata配列　音階,長さ,音階,長さ,・・・・・　最後に音階254で曲終了、253で最初へリピート
//	音階　0:ド 12:上のド 24:その上のド　255:休符
//	長さ　60分のn秒
const unsigned char musicdata1[]={
//ゲームスタート音楽
//ツィゴイネルワイゼン
	19,30,24,30,26,28,255,2,27,118,255,2,26,48,255,2,27,5,26,5,24,43,255,2,23,13,255,2,24,150,254
};
const unsigned char musicdata2[]={
//ステージ1の曲
//グノシエンヌ1
	255,40,24,20,27,20,26,40,24,38,255,2,24,5,23,75,24,5,23,75,
	255,40,24,20,27,20,26,40,24,40,28,5,29,75,28,5,29,75,
	255,40,24,20,27,20,26,40,24,38,255,2,24,5,23,75,20,5,19,75,
	17,5,19,35,17,5,19,75,255,40,20,5,19,33,255,2,19,5,17,115,
	255,160,

	255,40,24,20,27,20,26,40,24,38,255,2,24,5,23,75,24,5,23,75,
	255,40,24,20,27,20,26,40,24,40,28,5,29,75,28,5,29,75,
	255,40,24,20,27,20,26,40,24,38,255,2,24,5,23,75,20,5,19,75,
	17,5,19,35,17,5,19,75,255,40,20,5,19,33,255,2,19,5,17,115,
	255,160,

	20,5,22,75,20,5,19,75,20,5,22,75,20,5,19,75,
	20,5,19,33,255,2,19,5,17,115,255,160,
	20,5,22,75,20,5,19,75,20,5,22,75,20,5,19,75,
	20,5,19,33,255,2,19,5,17,115,255,160,

	255,20,24,20,26,20,28,20,29,20,31,20,35,20,31,20,29,40,31,5,29,75,255,40,
	31,5,29,73,255,2,29,5,28,75,25,5,24,73,255,2,24,5,23,75,
	20,5,19,33,255,2,19,5,17,115,255,160,

	255,20,24,20,26,20,28,20,29,20,31,20,35,20,31,20,29,40,31,5,29,75,255,40,
	31,5,29,73,255,2,29,5,28,75,25,5,24,73,255,2,24,5,23,75,
	20,5,19,33,255,2,19,5,17,115,255,160,

	20,5,22,75,20,5,19,75,20,5,22,75,20,5,19,75,
	20,5,19,33,255,2,19,5,17,115,255,160,
	20,5,22,75,20,5,19,75,20,5,22,75,20,5,19,75,
	20,5,19,33,255,2,19,5,17,115,255,160,

	253
};
const unsigned char musicdata3[]={
//ステージ2の曲
//グノシエンヌ2
	26,40,28,70,255,10,24,20,26,20,24,20,26,20,24,20,26,20,
	22,40,24,70,255,10,21,20,22,20,21,20,22,20,21,20,22,20,
	19,40,23,20,16,120,255,160,255,140,
	26,40,28,70,255,10,24,20,26,20,24,20,26,20,24,20,26,20,
	22,40,24,70,255,10,21,20,22,20,21,20,22,20,21,20,22,20,
	20,40,17,120,255,120,255,140,

	17,20,19,20,20,20,22,20,20,20,19,20,17,20,
	19,20,20,20,22,20,20,20,23,38,255,2,23,20,16,120,255,80,
	17,20,19,20,20,20,22,20,24,20,26,20,24,20,22,20,
	20,20,22,20,20,20,23,38,255,2,23,20,16,120,255,40,
	23,20,21,120,255,200,255,120,

	28,20,27,20,25,20,27,20,28,20,30,20,32,20,30,20,28,20,
	27,20,25,20,22,20,25,20,22,20,18,100,23,20,24,38,255,2,24,20,17,100,255,140,
	28,20,27,20,25,20,27,20,28,20,30,20,32,20,30,20,28,20,
	27,20,25,20,22,20,25,20,22,20,18,100,23,20,24,38,255,2,24,20,17,100,
	24,20,22,38,255,2,22,100,255,120,255,140,

	17,20,19,20,20,20,22,20,20,20,19,20,17,20,
	19,20,20,20,22,20,20,20,23,38,255,2,23,20,16,120,255,80,
	17,20,19,20,20,20,22,20,24,20,26,20,24,20,22,20,
	20,20,22,20,20,20,23,38,255,2,23,20,16,120,255,40,
	23,20,21,120,255,200,255,120,

	253
};
const unsigned char musicdata4[]={
//ステージ3の曲
//グノシエンヌ3
	33,18,255,2,33,20,27,18,255,2,27,20,30,18,255,2,30,60,
	255,40,28,18,255,2,28,20,27,18,255,2,27,20,24,18,255,2,
	24,20,23,18,255,2,23,60,255,240,
	33,18,255,2,33,20,27,18,255,2,27,20,30,18,255,2,30,60,
	255,40,28,18,255,2,28,20,27,18,255,2,27,20,24,18,255,2,
	24,20,23,18,255,2,23,60,255,75,

	23,5,26,40,23,18,255,2,23,18,24,18,255,2,24,60,
	23,18,255,2,23,60,255,75,23,5,26,120,255,80,
	23,18,255,2,23,20,26,18,255,2,26,20,23,18,255,2,
	23,20,20,20,21,20,23,20,26,20,23,20,21,20,
	20,20,21,20,23,20,26,20,23,20,21,20,20,20,21,20,
	23,20,26,20,29,18,255,2,29,20,21,18,255,2,21,60,255,200,

	28,18,255,2,28,20,22,18,255,2,22,20,
	25,18,255,2,25,60,255,40,23,18,255,2,23,20,
	22,18,255,2,22,20,19,18,255,2,19,20,18,18,255,2,18,60,255,240,
	28,18,255,2,28,20,22,18,255,2,22,20,
	25,18,255,2,25,60,255,40,23,18,255,2,23,20,
	22,18,255,2,22,20,19,18,255,2,19,20,18,18,255,2,18,60,255,75,
	18,5,21,120,255,40,

	16,20,18,20,19,20,22,20,23,20,25,20,28,20,25,20,
	23,20,22,20,19,20,18,20,16,20,18,20,19,20,22,20,
	23,20,25,20,28,20,25,20,23,20,22,20,19,20,18,20,
	16,20,18,20,19,20,22,20,23,20,25,20,28,20,25,20,
	26,18,255,2,26,60,255,240,
	16,20,18,20,19,20,22,20,23,20,25,20,28,20,25,20,
	23,20,22,20,19,20,18,20,16,20,18,20,19,20,22,20,
	23,20,25,20,28,20,25,20,23,20,22,20,19,20,18,20,
	16,20,18,20,19,20,22,20,23,20,25,20,28,20,25,20,
	26,18,255,2,26,60,255,75,18,5,21,240,255,75,
	24,5,20,240,255,80,

	20,20,23,20,24,20,27,20,26,20,24,20,23,20,20,20,
	24,18,255,2,24,60,255,75,23,5,26,240,255,80,
	20,20,23,20,24,20,27,20,26,20,24,20,23,20,20,20,
	24,18,255,2,24,60,255,75,23,5,26,160,

	253
};