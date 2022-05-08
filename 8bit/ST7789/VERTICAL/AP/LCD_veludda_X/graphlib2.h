// グラフィックライブラリ用ヘッダーファイル
// 画面上部の固定表示領域対応

extern const unsigned char FontData[];
extern void pset(int x,int y,unsigned char c);
extern void pset_fixarea(int x,int y,unsigned char c);
extern void putbmpmn(int x,int y,char m,char n,const unsigned char bmp[]);
extern void clrbmpmn(int x,int y,char m,char n);
extern void putfont(int x,int y,unsigned int c,unsigned char n);
extern void putfont_fixarea(int x,int y,unsigned int c,unsigned char n);
extern void line(int x1,int y1,int x2,int y2,unsigned int c);
extern void circle(int x0,int y0,unsigned int r,unsigned int c);
extern void printstr(int x,int y,unsigned int c,unsigned char *s);
extern void printstr_fixarea(int x,int y,unsigned int c,unsigned char *s);
extern void printnum(int x,int y,unsigned char c,unsigned int n);
extern void printnum_fixarea(int x,int y,unsigned char c,unsigned int n);
extern void initvscanv();
extern void pset2(unsigned char x,unsigned char y,unsigned char c);
extern void putfont2(int x,int y,unsigned int c,unsigned char n);
extern void printstr2(int x,int y,unsigned int c,unsigned char *s);
extern void printnum2(int x,int y,unsigned char c,unsigned int n);
extern void putbmpmn_fixarea(unsigned char x,unsigned char y,char m,char n,const unsigned char bmp[]);
extern void clrbmpmn_fixarea(unsigned char x,unsigned char y,char m,char n);
extern void putbmpmn2(unsigned char x,unsigned char y,char m,char n,unsigned char bmp[]);
extern void putbmpmn3(unsigned char x,unsigned char y,char m,char n,const unsigned char bmp[]);
