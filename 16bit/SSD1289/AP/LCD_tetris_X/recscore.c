// PIC32MX370F512H　SDブートローダー対応　バージョン2対応　情報記録プログラム
//　フラッシュの最終ページに共通形式で記録することで、1つのボードで複数のアプリのハイスコアなどの情報を保存可能
//　形式：ファイル名8バイト＋32bit×2個の情報で1レコード。最大64レコードを保持可能

// 0x9d07f000-0x9d07ffff:記録領域

#include <plib.h>
#include "recscore.h"

void *recscore_ad=0;//レコードへのポインタ、init_recscoreで設定される
int recscorebuf[4*MAXRECNUMBER]; // フラッシュ退避バッファ

// 記録領域からワーク領域にコピーし、ロードしたファイル名の先頭8文字でレコードを検索。なければ新規レコードを確保
// グローバル変数recscore_adにレコード先頭アドレスを設定
// 記録領域がいっぱいで作成できなければrecscore_ad=0を返す
void init_recscore(void){
	int *p1,*p2;
	unsigned int id1,id2;

	recscore_ad=0;
	id1=*((unsigned int *)(RECSCOREFLASH-16));
	id2=*((unsigned int *)(RECSCOREFLASH-12));

	//RAMにコピー
	p1=(int *)RECSCOREFLASH;
	p2=recscorebuf;
	while(p2<recscorebuf+4*MAXRECNUMBER) *p2++=*p1++;

	//記録場所検索、なければ新規
	p2=recscorebuf;
	while(p2<recscorebuf+4*MAXRECNUMBER){
		if(*p2==id1 && *(p2+1)==id2){
			recscore_ad=p2;
			return;
		}
		else if(*p2==-1){
			*p2=id1;
			*(p2+1)=id2;
			recscore_ad=p2;
			return;
		}
		p2+=4;
	}
}

// 2つの32ビットデータを書き込み
void write_recscore(unsigned int s1,unsigned int s2){

	if(recscore_ad==0) return;// 書き込み領域なし
	*((unsigned int *)(recscore_ad)+2)=s1;
	*((unsigned int *)(recscore_ad)+3)=s2;
	NVMErasePage((void *)RECSCOREFLASH);//ページ消去
	NVMWriteRow((void *)RECSCOREFLASH,(void *)recscorebuf); //1Row=512バイト書き込み
	NVMWriteRow((void *)(RECSCOREFLASH+512),(void *)recscorebuf+512); //1Row=512バイト書き込み
}

// レコードのn番目（0-1）の32ビットデータを返す
unsigned int read_recscore(int n){
	if(recscore_ad==0) return 0;
	return *((unsigned int *)recscore_ad+n+2);
}
