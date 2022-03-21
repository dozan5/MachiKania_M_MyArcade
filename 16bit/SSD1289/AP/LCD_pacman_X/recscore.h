// PIC32MX370F512H　SDブートローダー対応　バージョン2対応 情報記録プログラム用ヘッダー

//書き込みページ（4096バイト単位で消去される）
#define RECSCORESIZE 0x1000 //実際に使用するのは1024バイトのみ
#define RECSCOREFLASH (0x9d080000-RECSCORESIZE)
#define MAXRECNUMBER 64 // 記録可能なレコード数


void init_recscore(void);
void write_recscore(unsigned int s1,unsigned int s2);
unsigned int read_recscore(int n);
