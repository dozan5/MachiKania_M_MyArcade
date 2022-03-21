// ��ʉ�]�X�N���[���Q�[���@���C���v���O�����@VELUDDA for PIC32MX370F512H by K.Tanaka Rev 1.0
//
// �g��k����]�@�\�t���t���o�̓V�X�e���g�p�i��8�s�Œ�\���j
// �𑜓x ��256�~�c216�h�b�g�{��8�s
// VRAM�e��256�~256�h�b�g�A8�r�b�g�J���[

// �𑜓x ��240�~�c216�h�b�g�ɉ���

#include <xc.h>
#include "rotatevideo_lcd.h"
#include "graphlib2.h"
#include "veludda.h"
#include "LCDdriver.h"

#define BOOTLOADER //�u�[�g���[�_�[�pAP�ɂ���ꍇ�L���ɂ���

//�u�[�g���[�_�[�g�p���͋��ʃn�C�X�R�A�������ݗ̈�𗘗p
//���g�p���̓t���b�V���Ō�̃y�[�W�Ƀn�C�X�R�A�L�^
#ifndef BOOTLOADER
//�n�C�X�R�A�������݃y�[�W�i4096�o�C�g�P�ʂŏ��������j
#define FLASHPAGE (0x9d080000-0x1000)
#endif

#ifdef BOOTLOADER
#include "recscore.h"
#endif

//����FRC 8MHz 1/2 * PLL 24 = 96MHz
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

//�O�p�֐��e�[�u���i1��256�����A�l��256�{�j
const short sindata[256]={
	0,6,13,19,25,31,38,44,50,56,62,68,74,80,86,92,
	98,104,109,115,121,126,132,137,142,147,152,157,162,167,172,177,
	181,185,190,194,198,202,206,209,213,216,220,223,226,229,231,234,
	237,239,241,243,245,247,248,250,251,252,253,254,255,255,256,256,
	256,256,256,255,255,254,253,252,251,250,248,247,245,243,241,239,
	237,234,231,229,226,223,220,216,213,209,206,202,198,194,190,185,
	181,177,172,167,162,157,152,147,142,137,132,126,121,115,109,104,
	98,92,86,80,74,68,62,56,50,44,38,31,25,19,13,6,
	0,-6,-13,-19,-25,-31,-38,-44,-50,-56,-62,-68,-74,-80,-86,-92,
	-98,-104,-109,-115,-121,-126,-132,-137,-142,-147,-152,-157,-162,-167,-172,-177,
	-181,-185,-190,-194,-198,-202,-206,-209,-213,-216,-220,-223,-226,-229,-231,-234,
	-237,-239,-241,-243,-245,-247,-248,-250,-251,-252,-253,-254,-255,-255,-256,-256,
	-256,-256,-256,-255,-255,-254,-253,-252,-251,-250,-248,-247,-245,-243,-241,-239,
	-237,-234,-231,-229,-226,-223,-220,-216,-213,-209,-206,-202,-198,-194,-190,-185,
	-181,-177,-172,-167,-162,-158,-153,-147,-142,-137,-132,-126,-121,-115,-109,-104,
	-98,-92,-86,-80,-74,-68,-62,-56,-50,-44,-38,-31,-25,-19,-13,-6
};
// �t���ڃf�[�^
// x>=y�̎��́Ay*256/x�̊p�x�i45�x�܂Łj
const unsigned char atandata[257]={
	 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5,
	 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9,10,10,10,
	10,10,10,10,11,11,11,11,11,11,11,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,
	15,15,15,15,15,15,15,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,19,19,19,
	19,19,19,19,19,20,20,20,20,20,20,20,20,21,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,23,23,
	23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,25,26,26,26,26,26,26,
	26,26,26,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,
	29,29,29,30,30,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,31,31,31,32,32,32,32,32,32,
	32
};
const short enemyposition[][10]={
	//�G�̑����ݒ�
	//�J��Ԃ���,�G�ԍ�,�J�E���^,�摜,X,Y,VX,VY,DX,DY

	//�X�e�[�W1�p
	{5,0,0,2, 452,200,   0, 256, 15, 15},//0
	{4,0,0,2, 527,245,   0, 256, 15,-15},//1
	{5,0,0,0, 964,320,   0,-256, 15,-15},//2
	{4,0,0,0,  15,275,   0,-256, 15, 15},//3
	{5,0,0,1, 452, 20, 256,   0, 15, 15},//4
	{4,0,0,1, 527, 95, 256,   0,-15, 15},//5
	{5,0,0,3,  60,340,-256,   0,-15, 15},//6
	{4,0,0,3,  15,415,-256,   0, 15, 15},//7

	//�X�e�[�W2�p
	{5,1,0,0, 500,250,   0, 256,  0, 20},//8
	{5,1,0,0, 524,250,   0, 256,  0, 20},//9
	{5,1,0,0, 200,100, 181, 181,-15,-15},//10
	{5,1,0,0, 950, 50,-181,-181,-15,-15},//11
	{5,1,0,0,  50,450, 256,   0, 20,  0},//12
	{5,1,0,0,  50,474, 256,   0, 20,  0},//13

	//�X�e�[�W3�p
	{6,2,-1,0, 50,250, 181, 181,250, 50},//14
	{6,2,-1,1,150,120,-181, 181,280, 30},//15
	{6,2,-1,2,130, 80,-181,-181,300,100},//16
	{6,2,-1,3,100, 50, 181,-181,250, 60},//17

	//�X�e�[�W4�p
	{6,3,-1,0, 50,250, 181, 181,250, 50},//18
	{6,3,-1,0,150,120,-181, 181,280, 30},//19
	{6,3,-1,0,130, 80,-181,-181,300,100},//20
	{6,3,-1,0,100, 50, 181,-181,250, 60},//21

	//�X�e�[�W5�p
	{6,4,-1,0, 50, 50,   0,   0,180, 30},//22
	{6,4,-1,0, 50,170,   0,   0,180, 30},//23
	{6,4,-1,0, 50,290,   0,   0,180, 30},//24
	{6,4,-1,0, 50,410,   0,   0,180, 30},//25

	//�X�e�[�W6�p
	{6,5,-1,0, 50, 50,   0,   0,180, 30},//26
	{6,5,-1,0, 50,170,   0,   0,180, 30},//27
	{6,5,-1,0, 50,290,   0,   0,180, 30},//28
	{6,5,-1,0, 50,410,   0,   0,180, 30} //29
};
const short gndenemyposition[][5]={
//�n��G�̈ʒu
//�J��Ԃ���,X,Y,DX,DY
	{5, 20,140, 80,  0},//0
	{2,320, 40,  0,150},//1
	{4,620, 10, 45,  0},//2
	{2,560, 60, 20,  0},//3
	{2,560, 80, 20,  0},//4
	{3,280,350, 70,  0},//5
	{2,180,440,110,  0},//6
	{5,705,220, 60,  0},//7
	{4,660,300,  0, 55},//8
	{4,990,300,  0, 55} //9
};
const unsigned char enemytable[][20]={
	//�e�X�e�[�W��enemyposition,gndenemyposition�z��ԍ����
	//�ŏ��ɋ󒆓G�A255������͒n��G�A�Ō��255��1�X�e�[�W���I��
	{0,1,2,3,4,5,6,7,255,255},
	{8,9,10,11,12,13,255,0,5,9,255},
	{14,15,16,17,255,0,1,3,4,5,8,255},
	{18,19,20,21,255,6,255},
	{22,23,24,25,255,0,3,4,2,7,5,6,255},
	{26,27,28,29,255,0,1,2,7,9,5,255},
	{2,3,4,5,8,9,12,13,14,255,0,8,2,5,9,255},
	{0,1,18,19,22,25,28,255,1,3,4,5,9,255},
	{6,7,10,11,17,20,24,26,255,0,2,5,7,8,9,255}
};

#define GNDENEMYSCORE 500 //�n��G�̓��_
const unsigned short scoretable[]={
	//�󒆓G�̓��_�e�[�u��
	10,20,50,80,40,30
};
unsigned char *enemyname[]={
	"STRA",
	"KURURU",
	"ZIGZA",
	"FIRET",
	"BYON",
	"ROUNDA"
};

//�ȉ��t�p�\����
struct {
	const unsigned char *p; //�Ȕz��̉��t���̈ʒu
	const unsigned char *startp; //�Ȕz��̃��s�[�g�ʒu
	unsigned char count; //�������̉��J�E���^
	unsigned short pr; //�������̉��i�^�C�}�����j
	unsigned char stop; //0:���t���A1:�I��
}  music; //���t���̉��y�\����

//�ȉ��t���e�[�u��
const unsigned char *musictable[]={
	musicdata2,musicdata3,musicdata4
};

// �O�p�֐��}�N����`
#define Sin(x) sindata[(x)]
#define Cos(x) sindata[((x)+64)&0xff]

// �O���[�o���ϐ���`
unsigned char gamestatus;//0:�Q�[���J�n�A1:�X�e�[�W���\�����A2:�Q�[�����A3:�v���C���[1���A4:�X�e�[�W�N���A�A5:�Q�[���I�[�o�[�\���A6:�I��
unsigned short gamestatuscount;//�Q�[���X�e�[�^�X�J�E���^
unsigned short gcount; //�S�̃J�E���^
unsigned char stage; //�X�e�[�W
unsigned char enemyleft; //�G�c��
unsigned char ships; //���@�̎c��
unsigned int score,highscore; //�X�R�A�A�n�C�X�R�A
unsigned short keystatus=0,keystatus2,oldkey; //�ŐV�̃{�^����ԂƑO��̃{�^�����
unsigned char random8; //8bit�����l

//�摜�ޔ�p�o�b�t�@
// �r�b�g�}�b�v�f�[�^,VRAM���Wx,y,����,�c���̏��ɕۑ�
unsigned char bmpbuf[6000];
unsigned char *bmpbufp; //bmpbuf�̐擪�ʒu�|�C���^

int r1x,r1y; //��ʍ���̐�΍��W*256
int win_sx,win_sy,win_ex,win_ey; //��ʕ`�悳��Ă��鑋�̃}�b�v��̐�΍��W�̍ŏ��A�ő�
unsigned char scr_sx,scr_sy,scr_ex,scr_ey; //��ʕ`�悳��Ă��鑋��VRAM��̍��[�A��[�A�E�[�A���[�̗�A�s
const unsigned short *sounddatap; //���ʉ��z��̈ʒu�A���t���̉��y��肱�����D��
unsigned short sound2pr; //�C�e�򗈉�
unsigned char sound2count;//�C�e�򗈉��J�E���^
unsigned char music_on;//BGM�I���I�t
unsigned char continuecount,continueflag;//�R���e�B�j���[���

struct {
	char on;//0:���S�A1:�ʏ�A2:������
	int x,y;//���@�̐�΍��W*256
	unsigned char angle;//���@�̊p�x
	unsigned char count;//�J�E���^
} ship; //���@���

_Enemy enemybuf[MAX_ENEMY],gndenemybuf[MAX_GNDENEMY];//�󒆓G�A�n��G�i�[�z��
_Missile missilebuf[MAX_MISSILE],cannonbuf[MAX_CANNON];//�~�T�C���A�C�e�i�[�z��

//----------------------
// ��������v���O�����J�n
//----------------------
#ifndef BOOTLOADER
void writehighscore(void){
	//�n�C�X�R�A�̃t���b�V����������
	NVMErasePage((void *)FLASHPAGE);//�y�[�W����
	NVMWriteWord((void *)FLASHPAGE,highscore);//���[�h��������
}
#endif

void playmusic1step(void){
	//���t���̋Ȃ�1�i�߂�
	if(music.stop) return; //���t�I���ς�
	music.count--;
	if(music.count>0){
		PR3=music.pr;
		return;
	}
	//���̉���炷
	if(*music.p==254){ //�ȏI��
		music.stop=1;
		music.pr=0;
		PR3=0;
		return;
	}
	if(*music.p==253){ //�Ȃ̍ŏ��ɖ߂�
		music.p=music.startp;
	}
	if(*music.p==255){
		music.pr=0;
		PR3=0; //�x��
	}
	else{
		music.pr=sounddata[*music.p]; //�����f�[�^
		PR3=music.pr;
	}
	music.p++;
	music.count=*music.p; //��������
	music.p++;
}

void startmusic(const unsigned char *m){
// BGM�X�^�[�g
	music.p=m;
	music.startp=m;
	music.count=1;
	music.stop=0;
}

void stopmusic(void){
// BGM��~
	music.stop=1;
	music.pr=0;
	PR3=0;
}

void sound(void){
//���ʉ���BGM���o�́i���ʉ��D��j
//60����1�b���ƂɌĂяo��
	unsigned short pr;//�^�C�}�[�J�E���^�[�l

	playmusic1step();//BGM�̉��t��1�i�߂�
	pr=2;

	//�C�e�򗈉�
	if(sound2count){
		if(--sound2count==0){
			if(music.stop) pr=0;
		}
		else if(!music_on){
			pr=sound2pr;
			if(sound2count>128) sound2pr-=75;//�C�e�㏸��
			else sound2pr+=75;//�C�e���~��
		}
		else if((sound2count&3)==0){
			//BGM����̏ꍇ�A4���1�񂾂��炷
			pr=sound2pr;
			if(sound2count>128) sound2pr-=300;//�C�e�㏸��
			else sound2pr+=300;//�C�e���~��
		}
	}
	//���̑��̌��ʉ�
	if(sounddatap!=NULL){
		if(*sounddatap==0){
			sounddatap=NULL;
			if(music.stop) pr=0;
		}
		else{
			pr=*sounddatap++;
			if(pr==1) pr=0;//�x��
		}
	}

	if(pr!=2) PR3=pr;//�����ύX�B������pr��2�̏ꍇBGM�D��
}

void keycheck(void){
//�{�^����ԓǂݎ��
//keystatus :���݉�����Ă���{�^���ɑΉ�����r�b�g��1�ɂ���
//keystatus2:�O�񉟂���Ă��Ȃ��āA���񉟂��ꂽ�{�^���ɑΉ�����r�b�g��1�ɂ���
	oldkey=keystatus;
	keystatus=~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT | KEYSTART | KEYFIRE);
	keystatus2=keystatus & ~oldkey; //�{�^�������𗣂������`�F�b�N
}

void pausecheck(void){
//��~�{�^���`�F�b�N
	if(keystatus2&KEYSTART){
		PR3=0;//�T�E���h��~
		do{
			while(drawcount==0) asm("wait");
			drawcount=0;
			keycheck();
		}while((keystatus2&KEYSTART)==0);
	}
}

unsigned char atan3(int x,int y){
//�t���ڊ֐�
//�߂�l �p�x0-255�i�}�b�v�E������0�A���v���j
	unsigned char a;
	if(x==0 && y==0) return 0;
	if(y>=0){
		if(x>=0){
			if(x>=y) return atandata[(int)(y*256/x)];
			else return 64-atandata[(int)(x*256/y)];
		}
		else{
			x=-x;
			if(y>=x) return 64+atandata[(int)(x*256/y)];
			else return 128-atandata[(int)(y*256/x)];
		}
	}
	else{
		y=-y;
		if(x<0){
			x=-x;
			if(x>=y) return 128+atandata[(int)(y*256/x)];
			else return 192-atandata[(int)(x*256/y)];
		}
		else{
			if(y>=x) return 192+atandata[(int)(x*256/y)];
			else return 256-atandata[(int)(y*256/x)];
		}
	}
}

void srand8(unsigned char s){
//8bit�����̎�ݒ�
	random8=s;
}

unsigned char rand8(void){
//8bit��������
	random8=random8*5+1;
	return random8;
}

void getbmpbuf(unsigned char x,unsigned char y,char m,char n){
// VRAM��̍��Wx,y���牡m*�cn�h�b�g����bmpbuf[]�Ɏ�荞��
// ��ʏ㉺�A���E�͊�ɂȂ����Ă���

	unsigned char i,j,x1;
	if(bmpbufp+m*n+4>=bmpbuf+sizeof(bmpbuf)) return;
	for(i=0;i<n;i++){
		x1=x;
		for(j=0;j<m;j++){
			*bmpbufp++=*(VRAM+((unsigned int)y<<8)+x1);
			x1++;
		}
		y++;
	}
	*bmpbufp++=x;
	*bmpbufp++=y-n;
	*bmpbufp++=m;
	*bmpbufp++=n;
}

void getandputbmpmn(unsigned char x,unsigned char y,char m,char n,const unsigned char bmp[]){
// ��m*�cn�̉摜�z��bmp[]�����W(x,y)�ɕ\��
// �������ݑO�Ɍ��̉摜��bmpbuf�Ɏ�荞��
	getbmpbuf(x,y,m,n);
	putbmpmn3(x,y,m,n,bmp);
}

int rotatex(int x,int y){
//���@�̊p�x�����W����]�����Ƃ���x���W
	return x*Cos(ship.angle)-y*Sin(ship.angle);
}

int rotatey(int x,int y){
//���@�̊p�x�����W����]�����Ƃ���y���W
	return x*Sin(ship.angle)+y*Cos(ship.angle);
}

void drawvline(unsigned char x1,unsigned char y1,unsigned char y2,int mx,int my,unsigned char d){
// ����������MAP�ɂ��������Ĕw�i��`�悷��
// x:VRAM�̕`��X���W
// y1,y2:VRAM��̕`��Y���W�i��[�A���[�j
// mx,my:MAP��̍��W
// d:�`�悷���
// �w�i�f�[�^��4�{�Ɉ����L�΂�

	unsigned char *p;
	unsigned char x,y,y3;
	x=0;
	while(d!=0){
		p=mapdata+(((mx+x)>>2) & (MAPDX/4-1));
		y=0;
		y3=y1;
		while(y3!=y2){
//			pset(x1,y3,*(p+(((my+y)/4) & (MAPDY/4-1))*(MAPDX/4)));
			*(VRAM+y3*VRAM_X+x1)=*(p+(((my+y)>>2) & (MAPDY/4-1))*(MAPDX/4));
			y++;
			y3++;
		}
//		pset(x1,y3,*(p+(((my+y)/4) & (MAPDY/4-1))*(MAPDX/4)));
		*(VRAM+y3*VRAM_X+x1)=*(p+(((my+y)>>2) & (MAPDY/4-1))*(MAPDX/4));
		x++;
		x1++;
		d--;
	}
}

void drawhline(unsigned char y1,unsigned char x1,unsigned char x2,int mx,int my,unsigned char d){
// ����������MAP�ɂ��������Ĕw�i��`�悷��
// y:VRAM�̕`��Y���W
// x1,x2:VRAM��̕`��X���W�i���[�A�E�[�j
// mx,my:MAP��̍��W
// d:�`�悷��s��
// �w�i�f�[�^��4�{�Ɉ����L�΂�

	unsigned char *p;
	unsigned char x,y,x3;
	y=0;
	while(d!=0){
		p=mapdata+(((my+y)>>2) & (MAPDY/4-1))*(MAPDX/4);
		x=0;
		x3=x1;
		while(x3!=x2){
//			pset(x3,y1,*(p+(((mx+x)/4) & (MAPDX/4-1))));
			*(VRAM+y1*VRAM_X+x3)=*(p+(((mx+x)>>2) & (MAPDX/4-1)));
			x++;
			x3++;
		}
//		pset(x3,y1,*(p+(((mx+x)/4) & (MAPDX/4-1))));
		*(VRAM+y1*VRAM_X+x3)=*(p+(((mx+x)>>2) & (MAPDX/4-1)));
		y++;
		y1++;
		d--;
	}
}

void init_background(void){
//�w�i�摜��VRAM�ւ̏����`��Avscan�֘A�ϐ��ݒ�
	int dx,dy;

	//win_sx,win_sy,win_ex,win_ey ��ʕ`�悳��Ă��鑋�̃}�b�v��̐�΍��W�̍ŏ��A�ő�
	//ADX1,ADY1 ���@�����ʍ���܂ł̃}�b�v��̋���
	//ADX2,ADY2 ���@�����ʉE���܂ł̃}�b�v��̋���
	//ship.angle ���@�̐i�s�����p�x�i�}�b�v�^�������0�A���v���j
	if(ship.angle<64){
		win_sx=(ship.x+rotatex(-ADX1,ADY2))>>8;
		win_sy=(ship.y+rotatey(-ADX1,-ADY1))>>8;
		win_ex=(ship.x+rotatex(ADX2,-ADY1))>>8;
		win_ey=(ship.y+rotatey(ADX2,ADY2))>>8;
		vscanstartx=(ADY1+ADY2)*Sin(ship.angle);
		vscanstarty=0;
	}
	else if(ship.angle<128){
		win_sx=(ship.x+rotatex(ADX2,ADY2))>>8;
		win_sy=(ship.y+rotatey(-ADX1,ADY2))>>8;
		win_ex=(ship.x+rotatex(-ADX1,-ADY1))>>8;
		win_ey=(ship.y+rotatey(ADX2,-ADY1))>>8;
		vscanstartx=win_ex-win_sx;
		vscanstarty=-(ADY1+ADY2)*Cos(ship.angle);
	}
	else if(ship.angle<192){
		win_sx=(ship.x+rotatex(ADX2,-ADY1))>>8;
		win_sy=(ship.y+rotatey(ADX2,ADY2))>>8;
		win_ex=(ship.x+rotatex(-ADX1,ADY2))>>8;
		win_ey=(ship.y+rotatey(-ADX1,-ADY1))>>8;
		vscanstartx=-(ADX1+ADX2)*Cos(ship.angle);
		vscanstarty=win_ey-win_sy;
	}
	else{
		win_sx=(ship.x+rotatex(-ADX1,-ADY1))>>8;
		win_sy=(ship.y+rotatey(ADX2,-ADY1))>>8;
		win_ex=(ship.x+rotatex(ADX2,ADY2))>>8;
		win_ey=(ship.y+rotatey(-ADX1,ADY2))>>8;
		vscanstartx=0;
		vscanstarty=-(ADX1+ADX2)*Sin(ship.angle);
	}

	dx=win_ex-win_sx;
	dy=win_ey-win_sy;

	win_sx&=MAPDX-1;
	win_sy&=MAPDY-1;
	win_ex&=MAPDX-1;
	win_ey&=MAPDY-1;

	drawhline(0,0,dx,win_sx,win_sy,dy+1);//VRAM�ɕ`��

	//scr_sx,scr_sy,scr_ex,scr_ey ��ʕ`�悳��Ă��鑋��VRAM��̍��[�A��[�A�E�[�A���[�̗�A�s
	scr_sx=0;
	scr_sy=0;
	scr_ex=dx;
	scr_ey=dy;

	//���@�̌�������Avscan�x�N�g���ݒ�
	vscanv1_x=(Cos(ship.angle)*3)/4;
	vscanv1_y=(Sin(ship.angle)*3)/4;
	vscanv2_x=(-Sin(ship.angle)*3)/4;
	vscanv2_y=(Cos(ship.angle)*3)/4;

	//r1x,r1y ��ʍ���̐�΍��W*256
	r1x=NORMALIZEX(ship.x+rotatex(-ADX1,-ADY1));
	r1y=NORMALIZEY(ship.y+rotatey(-ADX1,-ADY1));
}

void addscore(unsigned char n){
//�X�R�A�X�V
//n:�|�����G�̔ԍ��A255�̏ꍇ�͒n��G
	if(n!=255) score+=scoretable[n];
	else score+=GNDENEMYSCORE;
	if(score>=1000000) score=999999;
	if(score>highscore) highscore=score;
}

void addgndenemy(_Enemy *ep,int x,int y){
//�n��G����
	if(ep>=gndenemybuf+MAX_GNDENEMY) return;
	ep->on=1; //�L��
	ep->no=0; //�G�ԍ�
	ep->count=rand8(); //�J�E���^�[
	ep->x=NORMALIZEX(x); //X���W*256
	ep->y=NORMALIZEY(y); //Y���W*256
	ep->vx=0; //X�����ړ����x*256
	ep->vy=0; //Y�����ړ����x*256
	ep->bmp=Bmp_gndenemy[0]; //�摜�ւ̃|�C���^
}

void addenemy(_Enemy *ep,unsigned char no,unsigned char count,int x,int y,short vx,short vy,const unsigned char *bmp){
//�󒆓G����
	if(ep>=enemybuf+MAX_ENEMY) return;
	ep->no=no; //�G�ԍ�
	ep->count=count; //�J�E���^�[
	ep->x=NORMALIZEX(x); //X���W*256
	ep->y=NORMALIZEY(y); //Y���W*256
	ep->vx=vx; //X�����ړ����x*256
	ep->vy=vy; //Y�����ړ����x*256
	ep->bmp=bmp; //�摜�ւ̃|�C���^
	ep->on=1; //�L��
	enemyleft++; //�G�c��
}

void addmissile(int x,int y,short vx,short vy,unsigned char c,const unsigned char *bmp){
//�~�T�C������
	_Missile *p;
	for(p=missilebuf;p<missilebuf+MAX_MISSILE;p++){
		//�o�b�t�@���̋󂫂�����
		if(p->on) continue;
		p->on=1;
		p->x=x;
		p->y=y;
		p->vx=vx;
		p->vy=vy;
		p->bmp=bmp;
		p->count=c;
		sounddatap=sounddata1;//�~�T�C�����ˉ��ݒ�
		return;
	}
}

void addcannon(int x,int y,int vx,int vy){
//�C�e����
	_Missile *p;
	for(p=cannonbuf;p<cannonbuf+MAX_CANNON;p++){
		//�o�b�t�@���̋󂫂�����
		if(p->on) continue;
		p->on=1;
		p->x=x;
		p->y=y;
		p->vx=vx;
		p->vy=vy;
		p->bmp=Bmp_cannon[0];
		p->count=255;
		sound2count=253;//�C�e�򗈉��J�E���^
		sound2pr=11000;//�C�e�򗈉��������l
		return;
	}
}

void gameinit4(){
//���@�A�w�i�A�X�R�A�s�̏�����
	unsigned char x,n;
	ship.angle=0; //���@�̌���
	ship.x=MAPDX/2*256; //���@�̐�΍��W*256
	ship.y=(MAPDY-42)*256;
	ship.on=1;
	clearscreen();//��ʏ���
	init_background();//�w�i�����`��
	bmpbufp=bmpbuf;

	//��ʍŏ㕔�ɃX�R�A�A�c�����`��
	printstr_fixarea(0,0,7,"STAGE");
	printnum_fixarea(40,0,7,stage);
	putbmpmn_fixarea(64,0,8,8,Bmp_enemy_mini);
	printstr_fixarea(96,0,7,"SCORE");

	//���@�c���\��
	n=ships-1;
//	x=256-8;
	x=240-8;
	while(n>0 && x>=24*8){
		putbmpmn_fixarea(x,0,8,8,Bmp_ship_mini);
		n--;
		x-=9;
	}
	sounddatap=NULL;//���ʉ��Ȃ�
	sound2count=0;//�C�e�򗈉��Ȃ�
	PR3=0;//�T�E���h��~
}

void gameinit3(){
//�~�T�C���A�C�e������
//enemytable�z��ɂ��������āA�n��G�A�󒆓G�̔z�u��������
	_Enemy *p;
	_Missile *mp;
	unsigned char a,n;
	const unsigned char *tp;
	const short *pp;
	short x,y;

	srand8(gcount);//�����̎�ݒ�
	for(mp=missilebuf;mp<missilebuf+MAX_MISSILE;mp++) mp->on=0;//�~�T�C���i�[�z��N���A
	for(mp=cannonbuf;mp<cannonbuf+MAX_CANNON;mp++) mp->on=0;//�C�e�i�[�z��N���A
	enemyleft=0; //�G�c���N���A

	if(stage<=9) tp=enemytable[stage-1];
	else tp=enemytable[9-1];//�X�e�[�W10�ȍ~�̓X�e�[�W9�Ɠ���

	//�󒆓G�̏�����
	p=enemybuf;
	while(*tp!=255){
		pp=enemyposition[*tp++];
		n=pp[0];
		x=pp[4];
		y=pp[5];
		while(n>0){
			if(p->on==1 || gamestatus!=3){ //gamestatus==3�@���@1���̎��A���S�G�͕��������Ȃ�
				if(pp[2]>=0) a=pp[2];//�����̏ꍇ�J�E���^�͗����l
				else a=rand8();

				//addenemy(�o�b�t�@�|�C���^,�G�ԍ�,�J�E���^�����l,x,y,vx,vy,�r�b�g�}�b�v)
				addenemy(p,pp[1],a,x*256,y*256,pp[6],pp[7],Bmp_enemy[pp[1]*4+pp[3]]);
			}
			p++;
			x=(x+pp[8])&(MAPDX-1);
			y=(y+pp[9])&(MAPDY-1);
			n--;
		}
	}
	for(;p<enemybuf+MAX_ENEMY;p++) p->on=0;//�o�b�t�@�̏I���܂ŃN���A

	if(gamestatus!=3){ //gamestatus==3�@���@1���̎��A�n��G�͏��������Ȃ�
		//�n��G�̏�����
		p=gndenemybuf;
		tp++;
		while(*tp!=255){
			pp=gndenemyposition[*tp++];
			n=pp[0];
			x=pp[1];
			y=pp[2];
			while(n>0){
				//addgndenemy(�o�b�t�@�|�C���^,x,y)
				addgndenemy(p,x*256,y*256);
				p++;
				x=(x+pp[3])&(MAPDX-1);
				y=(y+pp[4])&(MAPDY-1);
				n--;
			}
		}
		for(;p<gndenemybuf+MAX_GNDENEMY;p++) p->on=0;//�o�b�t�@�̏I���܂ŃN���A
	}
}

void gameinit2(){
//�Q�[���J�n���̏�����
	if(keystatus==(KEYFIRE|KEYLEFT|KEYSTART) && stage>1 && continuecount<CONTINUEMAX){
		continuecount++;
	}
	else{
		stage=1; //�X�e�[�W��
		continuecount=0;
	}
	ships=SHIPS; //�c�@��
	score=0; //���_
	if(music_on) startmusic(musicdata1);//�Q�[���J�n���̉��y
}

void gameinit(){
//�Q�[���S�̏�����
	//�J���[�p���b�g�ݒ�
	int i;
	const unsigned char *p;
	p=paldata2;
	for(i=128;i<256;i++){
		set_palette(i,*p,*(p+2),*(p+1));
		p+=3;
	}
	gcount=0;//�S�̃J�E���^�[
	score=0;
	music_on=1;//BGM����
	stopmusic();//BGM��~��
	continuecount=0;
}

void title(){
//�^�C�g����ʕ`��
	const unsigned char *p;
	unsigned char x,y,c,n;
	int i;
	clearscreen();//��ʏ���
	initvscanv();//vscan�֘A�ϐ��������i�g��k���Ȃ��A��]�Ȃ��j
	drawcount=0;
	while(drawcount==0) asm("wait");//�\���I���҂�
	putlcdall();//�t����ʂ�VRAM�̓��e��]��

	//���S�`��
	p=Bmp_logo;
	y=0;
	x=0;
	while(y<YSIZE_LOGO){
		c=*p++;//�p���b�g�ԍ�
		n=*p++;//���������O�X
		while(n>0){
			pset(x+15,y+10,c);
			n--;
			x++;
			if(x>=XSIZE_LOGO){
				x=0;
				y++;
			}
		}
	}
	printstr2(120-3*8,90-24,7,"SPEED UP");
	printstr2(120-11*8,90+8,7,"TURN LEFT    TURN RIGHT");
	putfont(120-4,90-2*8,6,0x1e);//�u���v
	putfont(120-20,90-4,6,0x1d);//�u���v
	putfont(120+12,90-4,6,0x1c);//�u���v
	putbmpmn3(120-XSIZE_SHIP/2,90-YSIZE_SHIP/2,XSIZE_SHIP,YSIZE_SHIP,Bmp_ship);
	printstr2(120-11*8,90+24,7,"MISSILE:      BUTTON");
	printstr2(120-11*8+9*8,90+24,6,"FIRE");
	printstr2(8,150,7,"HI-SCORE");
	printnum2(8+10*8,150,7,highscore);
	printstr2(140,150,7,"SCORE");
	printnum2(140+7*8,150,7,score);
	printstr2(120-4*8,170,7,"BY KENKEN");
	printstr2(120-2*8,180,7,"WITH");
	printstr2(120-15*8,190,7,"GRAPH LCD ROTARY OUTPUT SYSTEM");
	printstr2(120-9*8,200,7,"FOR PIC32MX370F512H");
	printstr2(240-9*8,130,7,"MUSIC");
	putfont2(240-4*8,124,6,0x1e);//�u���v
	putfont2(240-4*8,136,6,0x1f);//�u���v
	while(1){
		//�g���ʂ����]���Ȃ���ʏ�T�C�Y�ɖ߂��A��]���S�̓��S�������珙�X�ɉ�����
		for(x=254;x>0;x-=2){
			while(drawcount==0) asm("wait");//60����1�b�E�F�C�g
			drawcount=0;
			putlcdall();//�t����ʂ�VRAM�̓��e��]��
			gcount++;//�S�̃J�E���^
			vscanv1_x=50*Cos(x)/(x+50);
			vscanv1_y=50*Sin(x)/(x+50);
			vscanv2_x=-vscanv1_y;
			vscanv2_y=vscanv1_x;
			vscanstartx=50*((int)(-128)*Cos(x)+108*Sin(x))/(x+50)+128*256;
			vscanstarty=50*((int)(-128)*Sin(x)-108*Cos(x))/(x+50)+108*256-Sin(x/4)*88;
			keycheck();
			if(keystatus & KEYSTART) return;//�Q�[���X�^�[�g
			if(keystatus2 & KEYUP) music_on=1;
			else if(keystatus2 & KEYDOWN) music_on=0;
			if(music_on){
				printstr2(240-3*8,124, 4,"ON");
				printstr2(240-3*8,136,12,"OFF");
			}
			else{
				printstr2(240-3*8,124,12,"ON");
				printstr2(240-3*8,136, 4,"OFF");
			}
		}
		initvscanv();
		for(i=0;i<15*60;i++){ //15�b��
			while(drawcount==0) asm("wait");//60����1�b�E�F�C�g
			drawcount=0;
			putlcdall();//�t����ʂ�VRAM�̓��e��]��
			gcount++;//�S�̃J�E���^
			keycheck();
			if(keystatus & KEYSTART) return;//�Q�[���X�^�[�g
			if(keystatus2 & KEYUP) music_on=1;
			else if(keystatus2 & KEYDOWN) music_on=0;
			if(music_on){
				printstr2(240-3*8,124, 4,"ON");
				printstr2(240-3*8,136,12,"OFF");
			}
			else{
				printstr2(240-3*8,124,12,"ON");
				printstr2(240-3*8,136, 4,"OFF");
			}
		}
	}
}

void scroll_drawground(){
//��ʃX�N���[�����w�i�`��
//�X�N���[���O��ł̍����̂ݕ`�悷��
	int oldsx,oldsy,oldex,oldey;
	int oldr1x,oldr1y;
	char d;

	oldr1x=r1x;
	oldr1y=r1y;
	r1x=NORMALIZEX(ship.x+rotatex(-ADX1,-ADY1));
	r1y=NORMALIZEY(ship.y+rotatey(-ADX1,-ADY1));

	//���@���ړ��A��]�������Avscanstartx,vscanstarty���ړ�������
	vscanstartx+=(unsigned short)(r1x-oldr1x);
	vscanstarty+=(unsigned short)(r1y-oldr1y);

	//���@�̉�]�ɍ��킹�Avscan�x�N�g����ύX
	vscanv1_x=(Cos(ship.angle)*3)>>2;
	vscanv1_y=(Sin(ship.angle)*3)>>2;
	vscanv2_x=(-Sin(ship.angle)*3)>>2;
	vscanv2_y=(Cos(ship.angle)*3)>>2;

	//�`�摋�ړ����A���`�敔����V���ɕ`�悷��
	oldsx=win_sx;
	oldsy=win_sy;
	oldex=win_ex;
	oldey=win_ey;
	if(ship.angle<64){
		win_sx=(ship.x+rotatex(-ADX1,ADY2))>>8;
		win_sy=(ship.y+rotatey(-ADX1,-ADY1))>>8;
		win_ex=(ship.x+rotatex(ADX2,-ADY1))>>8;
		win_ey=(ship.y+rotatey(ADX2,ADY2))>>8;
	}
	else if(ship.angle<128){
		win_sx=(ship.x+rotatex(ADX2,ADY2))>>8;
		win_sy=(ship.y+rotatey(-ADX1,ADY2))>>8;
		win_ex=(ship.x+rotatex(-ADX1,-ADY1))>>8;
		win_ey=(ship.y+rotatey(ADX2,-ADY1))>>8;
	}
	else if(ship.angle<192){
		win_sx=(ship.x+rotatex(ADX2,-ADY1))>>8;
		win_sy=(ship.y+rotatey(ADX2,ADY2))>>8;
		win_ex=(ship.x+rotatex(-ADX1,ADY2))>>8;
		win_ey=(ship.y+rotatey(-ADX1,-ADY1))>>8;
	}
	else{
		win_sx=(ship.x+rotatex(-ADX1,-ADY1))>>8;
		win_sy=(ship.y+rotatey(ADX2,-ADY1))>>8;
		win_ex=(ship.x+rotatex(ADX2,ADY2))>>8;
		win_ey=(ship.y+rotatey(-ADX1,ADY2))>>8;
	}
	//�}�b�v�E�[�A���[����͂ݏo���������𐳋K��
	win_sx&=MAPDX-1;
	win_sy&=MAPDY-1;
	win_ex&=MAPDX-1;
	win_ey&=MAPDY-1;

	//��ʂ̏㉺���E�̋󔒕�����V���ɕ`�悷��
	d=(char)(win_sx-oldsx);
	scr_sx+=d;
	if(d<0){
		//�����󂫏ꏊ��`��
		drawvline(scr_sx,scr_sy,scr_ey,win_sx,oldsy,-d);
	}
	d=(char)(win_ex-oldex);
	scr_ex+=d;
	if(d>0){
		//�E���󂫏ꏊ��`��
		drawvline(scr_ex-d+1,scr_sy,scr_ey,win_ex-d+1,oldsy,d);
	}
	d=(char)(win_sy-oldsy);
	scr_sy+=d;
	if(d<0){
		//�㑤�󂫏ꏊ��`��
		drawhline(scr_sy,scr_sx,scr_ex,win_sx,win_sy,-d);
	}
	d=(char)(win_ey-oldey);
	scr_ey+=d;
	if(d>0){
		//�����󂫏ꏊ��`��
		drawhline(scr_ey-d+1,scr_sx,scr_ex,win_sx,win_ey-d+1,d);
	}
}

void drawship(){
//���@�\��
	if(ship.on==1){
		//�ʏ�
		getandputbmpmn(((ship.x>>8)-win_sx+scr_sx-XSIZE_SHIP/2)&0xff,((ship.y>>8)-win_sy+scr_sy-YSIZE_SHIP/2)&0xff,
			XSIZE_SHIP,YSIZE_SHIP,Bmp_ship);
	}
	else if(ship.on==2){
		//�����摜
		getandputbmpmn(((ship.x>>8)-win_sx+scr_sx-XSIZE_EXPLODE2/2)&0xff,((ship.y>>8)-win_sy+scr_sy-YSIZE_EXPLODE2/2)&0xff,
			XSIZE_EXPLODE2,YSIZE_EXPLODE2,Bmp_explode2[ship.count>>2]);
		ship.count++;
		if(ship.count==32) ship.on=0;//�����I��
	}
}

void drawgndenemy(){
//�n��G�\��
	_Enemy *p;
	const unsigned char *bmp;
	unsigned short x,y;
	for(p=gndenemybuf;p<gndenemybuf+MAX_GNDENEMY;p++){
		if(!p->on) continue;
		//��ʕ`�摋���ɓ����Ă��邩�`�F�b�N
		x=p->x>>8;
		y=p->y>>8;
		if(win_sx<win_ex){ //�`�摋�̉��������}�b�v���Ɏ��܂��Ă���ꍇ
			if(x<win_sx) continue; //�`�摋��荶�ɂ���
			if(x>win_ex) continue; //�`�摋���E�ɂ���
		}
		else{ //�`�摋���}�b�v�E�[�ɂ܂������Ă���ꍇ
			if(x>win_ex && x<win_sx) continue; //�`�摋�̊O�ɂ���
		}
		if(win_sy<win_ey){ //�`�摋�̏㉺�������}�b�v���Ɏ��܂��Ă���ꍇ
			if(y<win_sy) continue; //�`�摋����ɂ���
			if(y>win_ey) continue; //�`�摋��艺�ɂ���
		}
		else{ //�`�摋���}�b�v���[�ɂ܂������Ă���ꍇ
			if(y>win_ey && y<win_sy) continue; //�`�摋�̊O�ɂ���
		}
		if(p->on==2){
			//������
			getandputbmpmn((x-win_sx+scr_sx-XSIZE_EXPLODE/2)&0xff,(y-win_sy+scr_sy-YSIZE_EXPLODE/2)&0xff,
				XSIZE_EXPLODE,YSIZE_EXPLODE,Bmp_explode[p->count]);
			p->count++;
			if(p->count==8) p->on=3;//������
			continue;
		}
		if(p->on==3) bmp=Bmp_gndenemy[4]; //������
		else if(p->count<208) bmp=Bmp_gndenemy[0]; //�ʏ�
		else bmp=Bmp_gndenemy[((p->count-208)>>4)+1]; //�C�e���ˑO
		getandputbmpmn((x-win_sx+scr_sx-XSIZE_GNDENEMY/2)&0xff,(y-win_sy+scr_sy-YSIZE_GNDENEMY/2)&0xff,
			XSIZE_GNDENEMY,YSIZE_GNDENEMY,bmp);
	}
}

void drawenemy(){
//�󒆓G�`��
	_Enemy *p;
	unsigned short x,y;
	for(p=enemybuf;p<enemybuf+MAX_ENEMY;p++){
		if(!p->on) continue;

		//��ʕ`�摋���ɓ����Ă��邩�`�F�b�N
		x=p->x>>8;
		y=p->y>>8;
		if(win_sx<win_ex){
			if(x<win_sx) continue;
			if(x>win_ex) continue;
		}
		else{
			if(x>win_ex && x<win_sx) continue;
		}
		if(win_sy<win_ey){
			if(y<win_sy) continue;
			if(y>win_ey) continue;
		}
		else{
			if(y>win_ey && y<win_sy) continue;
		}
		if(p->on==1){
			//�ʏ펞
			getandputbmpmn((x-win_sx+scr_sx-XSIZE_ENEMY/2)&0xff,(y-win_sy+scr_sy-YSIZE_ENEMY/2)&0xff,
				XSIZE_ENEMY,YSIZE_ENEMY,p->bmp);
		}
		else{
			//������
			getandputbmpmn((x-win_sx+scr_sx-XSIZE_EXPLODE/2)&0xff,(y-win_sy+scr_sy-YSIZE_EXPLODE/2)&0xff,
				XSIZE_EXPLODE,YSIZE_EXPLODE,Bmp_explode[p->count]);
			p->count++;
			if(p->count==8) p->on=0;//����
		}
	}
}

void drawmissile(){
//�~�T�C���`��
	_Missile *p;
	unsigned short x,y;
	for(p=missilebuf;p<missilebuf+MAX_MISSILE;p++){
		if(!p->on) continue;

		//��ʕ`�摋���ɓ����Ă��邩�`�F�b�N
		x=p->x>>8;
		y=p->y>>8;
		if(win_sx<win_ex){
			if(x<win_sx) continue;
			if(x>win_ex) continue;
		}
		else{
			if(x>win_ex && x<win_sx) continue;
		}
		if(win_sy<win_ey){
			if(y<win_sy) continue;
			if(y>win_ey) continue;
		}
		else{
			if(y>win_ey && y<win_sy) continue;
		}
		getandputbmpmn((x-win_sx+scr_sx-XSIZE_MISSILE/2)&0xff,(y-win_sy+scr_sy-YSIZE_MISSILE/2)&0xff,
			XSIZE_MISSILE,YSIZE_MISSILE,p->bmp);
	}
}

void drawcannon(){
//�C�e�`��
	_Missile *p;
	unsigned short x,y;
	for(p=cannonbuf;p<cannonbuf+MAX_CANNON;p++){
		if(!p->on) continue;

		//��ʕ`�摋���ɓ����Ă��邩�`�F�b�N
		x=p->x>>8;
		y=p->y>>8;
		if(win_sx<win_ex){
			if(x<win_sx) continue;
			if(x>win_ex) continue;
		}
		else{
			if(x>win_ex && x<win_sx) continue;
		}
		if(win_sy<win_ey){
			if(y<win_sy) continue;
			if(y>win_ey) continue;
		}
		else{
			if(y>win_ey && y<win_sy) continue;
		}
		getandputbmpmn((x-win_sx+scr_sx-XSIZE_CANNON/2)&0xff,(y-win_sy+scr_sy-YSIZE_CANNON/2)&0xff,
			XSIZE_CANNON,YSIZE_CANNON,p->bmp);
	}
}

void drawscore(){
//�G�c���A�X�R�A�̕\��
	putfont_fixarea(10*8,0,0,' ');
	printnum_fixarea(9*8,0,7,enemyleft);
	printnum_fixarea(18*8,0,7,score);
}

void erasechars(){
//bmpbuf�Ɋi�[���ꂽ��ʓ��e���t���ɍĕ`�悵�Ĕw�i�摜�ɖ߂�
	unsigned char m,n;
	while(bmpbufp>bmpbuf){
		m=*(bmpbufp-2);
		n=*(bmpbufp-1);
		putbmpmn2(*(bmpbufp-4),*(bmpbufp-3),m,n,bmpbufp-4-m*n);
		bmpbufp-=4+m*n;
	}
}

void moveship(){
//���@�̈ړ�
	if(ship.on!=1) return;
	if(keystatus & KEYLEFT) ship.angle--;
	if(keystatus & KEYRIGHT) ship.angle++;
	if(keystatus & KEYUP){
		//��{�^����1.5�{��
		ship.x=NORMALIZEX(ship.x+Sin(ship.angle)+Sin(ship.angle)/2);
		ship.y=NORMALIZEY(ship.y-Cos(ship.angle)-Cos(ship.angle)/2);
	}
	else{
		ship.x=NORMALIZEX(ship.x+Sin(ship.angle));
		ship.y=NORMALIZEY(ship.y-Cos(ship.angle));
	}
}

void movegndenemy(){
	//�n��G�̖C�e���ˏ���
	_Enemy *p;
	int dx,dy,dx1,dy1;
	if(ship.on!=1) return;
	for(p=gndenemybuf;p<gndenemybuf+MAX_GNDENEMY;p++){
		//p->on 0:�����A1:�ʏ�A2:�������A3:������
		if(p->on!=1) continue;
		//�J�E���^��0�Ŏ��@�ɋ߂��Ƒ_���ĖC�e����
		if(++p->count) continue;
		dx=ship.x-p->x;
		if(dx<0) dx1=-dx; else dx1=dx;
		if(dx1>(MAPDX-CANNONFIRE)*256){
			if(dx<0) dx+=MAPDX*256;
			else dx-=MAPDX*256;
		}
		else if(dx1>=CANNONFIRE*256) continue;
		dy=ship.y-p->y;
		if(dy<0) dy1=-dy; else dy1=dy;
		if(dy1>(MAPDY-CANNONFIRE)*256){
			if(dy<0) dy+=MAPDY*256;
			else dy-=MAPDY*256;
		}
		else if(dy1>=CANNONFIRE*256) continue;
		addcannon(p->x,p->y,dx/128+Sin(ship.angle),dy/128-Cos(ship.angle)); //�C�e����
	}
}

char enemynearcheck(_Enemy *p){
//�G�����@�̋ߖT�ɂ��邩�`�F�b�N
//�߂�l�@�ߖT�̏ꍇ�F-1�A����ȊO�F0
#define ENEMYNEAR 80 //�ߖT����

	int dx,dy;

	if(ship.on!=1) return 0;
	dx=ship.x-p->x;
	if(dx<0) dx=-dx;
	dx>>=8;
	if(dx>=(MAPDX-ENEMYNEAR)) dx-=MAPDX;
	else if(dx>ENEMYNEAR) return 0;
	dy=ship.y-p->y;
	if(dy<0) dy=-dy;
	dy>>=8;
	if(dy>=(MAPDY-ENEMYNEAR)) dy-=MAPDY;
	else if(dy>ENEMYNEAR) return 0;
	if((dx*dx+dy*dy)>ENEMYNEAR*ENEMYNEAR) return 0;
	return -1;
}

void enemyavoid(_Enemy *p){
//���@�������悤�ɓG�̐i�s�����ύX
	int dx,dy;
	unsigned char a1,a2,a3;

	if(ship.on!=1) return;
	dx=(ship.x-p->x)>>8;
	if(dx<-MAPDX/2) dx+=MAPDX;
	else if(dx>MAPDX/2) dx-=MAPDX;
	dy=(ship.y-p->y)>>8;
	if(dy<-MAPDY/2) dy+=MAPDY;
	else if(dy>MAPDY/2) dy-=MAPDY;

	a1=atan3(dx,dy); //�G�Ǝ��@�̊p�x
	a2=atan3(p->vx,p->vy); //�G�i�s�����p�x
	a3=a1-a2;
	//���@�����������ɐ��񂳂���
	if(a3<96){
		a2-=4;
		p->vx=Cos(a2);
		p->vy=Sin(a2);
	}
	if(a3>256-96){
		a2+=4;
		p->vx=Cos(a2);
		p->vy=Sin(a2);
	}
}

void enemyturnright(_Enemy *p){
// �󒆓G�����v����90�x��]
	short t1;
	t1=p->vx;
	p->vx=-(p->vy);
	p->vy=t1;
	p->bmp+=SQSIZE_ENEMY;
	if(p->bmp==Bmp_enemy[p->no*4+4]) p->bmp=Bmp_enemy[p->no*4];
}

void enemyturnleft(_Enemy *p){
// �󒆓G�𔽎��v����90�x��]
	short t1;
	t1=p->vx;
	p->vx=p->vy;
	p->vy=-t1;
	p->bmp-=SQSIZE_ENEMY;
	if(p->bmp<Bmp_enemy[p->no*4]) p->bmp=Bmp_enemy[p->no*4+3];
}

void moveenemy(){
	//�󒆓G�̈ړ�
	_Enemy *p;
	for(p=enemybuf;p<enemybuf+MAX_ENEMY;p++){
		//p->on 0:�����A1:�ʏ�A2:������
		if(p->on!=1) continue;
		//�ʏ�̏ꍇ
		p->x=NORMALIZEX(p->x + p->vx);
		p->y=NORMALIZEY(p->y + p->vy);
		p->count++;
		switch(p->no){
			case 0:
				//�������Ȃ��i���������^���j
				break;
			case 1:
				//���@�ɋ߂Â��Ɖ���s��
				if(enemynearcheck(p)) enemyavoid(p);
				if((p->count&7)==0){
					p->bmp+=SQSIZE_ENEMY;
					if(p->bmp==Bmp_enemy[1*4+4]) p->bmp=Bmp_enemy[1*4];
				}
				break;
			case 2:
				//�W�O�U�O�^��
				if(p->count&0x1f) break;
				if(p->count&0x20) enemyturnright(p);//90�x��������]
				else enemyturnleft(p);//90�x��������]
				break;
			case 3:
				if(p->count<208) p->bmp=Bmp_enemy[3*4];
				else p->bmp=Bmp_enemy[3*4+1+((p->count-208)>>4)];
				if(p->count==0 && enemynearcheck(p)){
					//�J�E���^��0�Ŏ��@���߂��ɂ���ꍇ4�����Ƀ~�T�C������
					addmissile(NORMALIZEX(p->x-8*256),p->y,-256*2,0,60,Bmp_missile2);
					addmissile(NORMALIZEX(p->x+8*256),p->y, 256*2,0,60,Bmp_missile2);
					addmissile(p->x,NORMALIZEY(p->y-8*256),0,-256*2,60,Bmp_missile2);
					addmissile(p->x,NORMALIZEY(p->y+8*256),0, 256*2,60,Bmp_missile2);
				}
				break;
			case 4:
				//�����g�^��
				p->vy=Cos(p->count)/2-256;
				p->bmp=Bmp_enemy[4*4+(p->count >>6)];
				break;
			case 5:
				//��]�^��
				p->vx=Sin(p->count);
				p->vy=Cos(p->count);
				if((p->count&7)==0){
					p->bmp+=SQSIZE_ENEMY;
					if(p->bmp==Bmp_enemy[5*4+4]) p->bmp=Bmp_enemy[5*4];
				}
				break;
			default:
				break;
		}
	}
}

void movemissile(){
//�~�T�C���ړ�
	_Missile *p;
	for(p=missilebuf;p<missilebuf+MAX_MISSILE;p++){
		if(!p->on) continue;
		if(--p->count==0){
			//�~�T�C������
			p->on=0;
			continue;
		}
		p->x=NORMALIZEX(p->x + p->vx);
		p->y=NORMALIZEY(p->y + p->vy);
	}
}

void movecannon(){
//�C�e�ړ�
	_Missile *p;
	int c;
	for(p=cannonbuf;p<cannonbuf+MAX_CANNON;p++){
		if(!p->on) continue;
		if(--p->count==0){
			//�C�e����
			p->on=0;
			continue;
		}
		p->x=NORMALIZEX(p->x + p->vx);
		p->y=NORMALIZEY(p->y + p->vy);
		c=p->count >>4;
		if(c>7) c=15-c;
		p->bmp=Bmp_cannon[c];
	}
}

void fire(){
//FIRE�{�^���Ŏ��@�̃~�T�C������
	if(ship.on!=1) return;
	if(keystatus2 & KEYFIRE){
		addmissile(NORMALIZEX(ship.x+Sin(ship.angle)*12),NORMALIZEY(ship.y-Cos(ship.angle)*12),
			Sin(ship.angle)*5,-Cos(ship.angle)*5,32,Bmp_missile1);
	}
}

void enemydeath(_Enemy *ep){
//�󒆓G���S����
	ep->on=2;//�G����
	ep->count=0; //�G�����摜�p�J�E���^
	enemyleft--; //�G�c����
	addscore(ep->no);
	sounddatap=sounddata2;//�G�������ݒ�
}

void gndenemydeath(_Enemy *ep){
//�n��G���S����
	ep->on=2;//�G����
	ep->count=0; //�G�����摜�p�J�E���^
	addscore(255);
	sounddatap=sounddata2;//�G�������ݒ�
}

void shipdeath(){
//���@���S����
	ship.on=2;//������
	ship.count=0; //���@�����摜�p�J�E���^
	sounddatap=sounddata3;//���@�������ݒ�
	sound2count=0;//�C�e�򗈉���~
}

void collisioncheck(){
//�e��Փ˃`�F�b�N
	_Enemy *ep;
	_Missile *mp;
	int dx,dy;

	//�~�T�C���Ƌ󒆓G�̏Փ˃`�F�b�N
	for(mp=missilebuf;mp<missilebuf+MAX_MISSILE;mp++){
		if(!mp->on) continue;
		for(ep=enemybuf;ep<enemybuf+MAX_ENEMY;ep++){
			if(ep->on!=1) continue;
			//X���W�`�F�b�N
			dx=mp->x - ep->x;
			if(dx<0) dx=-dx;
			if(dx>XSIZE_ENEMY/2*256 && dx<(MAPDX-XSIZE_ENEMY/2)*256) continue;
			//Y���W�`�F�b�N
			dy=mp->y - ep->y;
			if(dy<0) dy=-dy;
			if(dy>YSIZE_ENEMY/2*256 && dy<(MAPDY-YSIZE_ENEMY/2)*256) continue;

			// �~�T�C���ƓG���Փ˂������̏���
			enemydeath(ep);//�G���S����
			mp->on=0; //�~�T�C������
		}
	}
	//�C�e�Ƌ󒆓G�̏Փ˃`�F�b�N
	for(mp=cannonbuf;mp<cannonbuf+MAX_CANNON;mp++){
		if(!mp->on) continue;
		if(mp->count<112 || mp->count>=144) continue;//�C�e�̍��x�`�F�b�N
		for(ep=enemybuf;ep<enemybuf+MAX_ENEMY;ep++){
			if(ep->on!=1) continue;
			//X���W�`�F�b�N
			dx=mp->x - ep->x;
			if(dx<0) dx=-dx;
			if(dx>(XSIZE_CANNON/2+XSIZE_ENEMY/2)*256 && dx<(MAPDX-(XSIZE_CANNON/2+XSIZE_ENEMY/2))*256) continue;
			//Y���W�`�F�b�N
			dy=mp->y - ep->y;
			if(dy<0) dy=-dy;
			if(dy>(YSIZE_CANNON/2+YSIZE_ENEMY/2)*256 && dy<(MAPDY-(YSIZE_CANNON/2+YSIZE_ENEMY/2))*256) continue;

			// �C�e�Ƌ󒆓G���Փ˂������̏���
			enemydeath(ep);//�G���S����
			mp->on=0; //�C�e����
		}
	}
	//�C�e�ƒn��G�̏Փ˃`�F�b�N
	for(mp=cannonbuf;mp<cannonbuf+MAX_CANNON;mp++){
		if(!mp->on) continue;
		if(mp->count>1) continue;//�C�e�̍��x�`�F�b�N
		for(ep=gndenemybuf;ep<gndenemybuf+MAX_GNDENEMY;ep++){
			if(ep->on!=1) continue;
			//X���W�`�F�b�N
			dx=mp->x - ep->x;
			if(dx<0) dx=-dx;
			if(dx>(XSIZE_CANNON/2+XSIZE_GNDENEMY/2)*256 && dx<(MAPDX-(XSIZE_CANNON/2+XSIZE_GNDENEMY/2))*256) continue;
			//Y���W�`�F�b�N
			dy=mp->y - ep->y;
			if(dy<0) dy=-dy;
			if(dy>(YSIZE_CANNON/2+YSIZE_GNDENEMY/2)*256 && dy<(MAPDY-(YSIZE_CANNON/2+YSIZE_GNDENEMY/2))*256) continue;

			// �C�e�ƒn��G���Փ˂������̏���
			gndenemydeath(ep);//�n��G��������
			mp->on=0; //�C�e����
		}
	}

//�������玩�@�Փ˃`�F�b�N
	if(ship.on!=1) return; //���@���S��
	//���@�Ƌ󒆓G�̏Փ˃`�F�b�N
	for(ep=enemybuf;ep<enemybuf+MAX_ENEMY;ep++){
		if(ep->on!=1) continue;
		//X���W�`�F�b�N
		dx=ship.x-ep->x;
		if(dx<0) dx=-dx;
		if(dx>(XSIZE_SHIP/2+XSIZE_ENEMY/2-2)*256 && dx<(MAPDX-(XSIZE_SHIP/2+XSIZE_ENEMY/2-2))*256) continue;
		//Y���W�`�F�b�N
		dy=ship.y-ep->y;
		if(dy<0) dy=-dy;
		if(dy>(YSIZE_SHIP/2+YSIZE_ENEMY/2-2)*256 && dy<(MAPDY-(YSIZE_SHIP/2+YSIZE_ENEMY/2-2))*256) continue;

		// ���@�ƓG���Փ˂������̏���
		enemydeath(ep);//�G���S����
		shipdeath();//���@���S����
	}
	//���@�ƖC�e�̏Փ˃`�F�b�N
	for(mp=cannonbuf;mp<cannonbuf+MAX_CANNON;mp++){
		if(mp->on!=1) continue;
		if(mp->count<112 || mp->count>=144) continue;//�C�e�̍��x�`�F�b�N
		//X���W�`�F�b�N
		dx=ship.x-mp->x;
		if(dx<0) dx=-dx;
		if(dx>(XSIZE_SHIP/2+XSIZE_CANNON/2-2)*256 && dx<(MAPDX-(XSIZE_SHIP/2+XSIZE_CANNON/2-2))*256) continue;
		//Y���W�`�F�b�N
		dy=ship.y-mp->y;
		if(dy<0) dy=-dy;
		if(dy>(YSIZE_SHIP/2+YSIZE_CANNON/2-2)*256 && dy<(MAPDY-(YSIZE_SHIP/2+YSIZE_CANNON/2-2))*256) continue;

		// ���@�ƖC�e���Փ˂������̏���
		mp->on=0;//�C�e����
		shipdeath();//���@���S����
	}

	//���@�ƃ~�T�C���̏Փ˃`�F�b�N
	for(mp=missilebuf;mp<missilebuf+MAX_MISSILE;mp++){
		if(mp->on!=1) continue;
		//X���W�`�F�b�N
		dx=ship.x-mp->x;
		if(dx<0) dx=-dx;
		if(dx>(XSIZE_SHIP/2-2)*256 && dx<(MAPDX-(XSIZE_SHIP/2-2))*256) continue;
		//Y���W�`�F�b�N
		dy=ship.y-mp->y;
		if(dy<0) dy=-dy;
		if(dy>(YSIZE_SHIP/2-2)*256 && dy<(MAPDY-(YSIZE_SHIP/2-2))*256) continue;

		// ���@�ƃ~�T�C�����Փ˂������̏���
		mp->on=0;//�~�T�C������
		shipdeath();//���@���S����
	}
}

void gameover(){
//�Q�[���I��������
	PR3=0;//�T�E���h��~
#ifdef BOOTLOADER
	if(highscore>read_recscore(0)){
		write_recscore(highscore,0);//�u�[�g���[�_�Ή����ʗ̈�̃��R�[�h�Ƀn�C�X�R�A��������
	}
#else
	if(highscore>*(unsigned int *)FLASHPAGE){
		stop_composite();//�\����~
		writehighscore();//�n�C�X�R�A��������
		start_composite();//�\���J�n
	}
#endif
}

void changegamestatus(){
	unsigned char x,y,e;
	//�Q�[���̃X�e�[�^�X�X�V
	switch(gamestatus){
		case 0://�Q�[���X�^�[�g
		case 1://�X�e�[�W���\����
			getbmpbuf(50,80,72,8);
			printstr2(50,80,7,"STAGE");
			printnum2(50+8*6,80,7,stage);
			if(stage<=6){
				//�󒆓G���̂Ɠ��_�\��
				e=enemyposition[enemytable[stage-1][0]][1];
				getbmpbuf(50,90,12*8,8);
				printstr2(50,90,7,enemyname[e]);
				printnum2(50+7*8,90,7,scoretable[e]);
				printstr2(50+9*8,90,7,"PTS");
			}
			gamestatuscount--;
			if(gamestatuscount==0){
				if(music_on) startmusic(musictable[(stage-1)%(sizeof(musictable)/sizeof(musictable[0]))]);//BGM�J�n
				gamestatus=2;
			}
			break;
		case 2://�ʏ�Q�[����
			if(ship.on!=1){
				stopmusic();//�ȉ��t��~
				gamestatus=3;//���@���S
				gamestatuscount=180;//3�b��~
				break;
			}
			if(enemyleft==0){
				//�G�S��
				stopmusic();//�ȉ��t��~
				sound2count=0;//�C�e�򗈉���~
				gamestatus=4; //�X�e�[�W�N���A
				gamestatuscount=120; //2�b�Ԓ�~
			}
			break;
		case 3://���@���S
			if(--gamestatuscount) break;
			ships--;
			if(ships==0){
				gamestatus=5; //�Q�[���I�[�o�[
				gamestatuscount=300; //5�b�ԁuGAMEOVER�v�\��
				break;
			}
			if(enemyleft>0){
				gameinit3();//�X�e�[�W������
				gameinit4();//���@������
				gamestatus=1; //�X�e�[�W�ŏ�����
				gamestatuscount=180; //3�b�ԃX�e�[�W���\��
				break;
			}
			//���X�e�[�W��
			gamestatus=4;
			gamestatuscount=1;
		case 4://���X�e�[�W��
			if(--gamestatuscount) break;
			if(stage<99) stage++;
			if(stage%5==0 && ships<255) ships++;//5�X�e�[�W���ƂɎ��@+1
			gameinit3();//�X�e�[�W������
			gameinit4();//���@������
			gamestatus=1; //�X�e�[�W�ŏ���
			gamestatuscount=180; //3�b�ԃX�e�[�W���\��
			break;
		case 5:	//�Q�[���I�[�o�[
			//��ʉ�]�ʒu��߂��A������GAMEOVER�\��
			x=(vscanstartx>>8)+50;
			y=(vscanstarty>>8)+80;
			getbmpbuf(x,y,72,8);
			printstr2(x,y,(gcount&3)+5,"GAMEOVER");
			gamestatuscount--;
			if(gamestatuscount==0) gamestatus=6;
			if(ship.angle==0) break;
			// GAMEOVER�\���𐳏�Ȍ����ɖ߂�
			if(ship.angle<128) ship.angle--;
			else ship.angle++;
			break;
	}
}

void game(){
	gamestatus=0;//0:�Q�[���J�n�A1:�X�e�[�W���\�����A2:�Q�[�����A3:�v���C���[1���A4:�X�e�[�W�N���A�A5:�Q�[���I�[�o�[�\���A6:�I��
	if(music_on) gamestatuscount=500;
	else gamestatuscount=180;
	gameinit2();//�Q�[���J�n����
	gameinit3();//�X�e�[�W�N���A����
	gameinit4();//���@������
	while(gamestatus<6){
		//�f����ԏI���҂��i��60����1�b�̃E�F�C�g�j
		while(drawcount==0) asm("wait");
		drawcount=0;
		putlcdall();//�t����ʂ�VRAM�̓��e��]��
		sound();//�T�E���h����
		erasechars();//�w�i�ȊO�̕`�����
		keycheck();//�{�^���X�L����
		if(gamestatus==2 || gamestatus==3){
			fire();//���@�~�T�C�����ˏ���
			moveship();//���@�ړ�����
			movegndenemy();//�n��G�ړ������i�C�e���ˁj
			moveenemy();//�󒆓G�ړ�����
			movemissile();//�~�T�C���ړ�����
			movecannon();//�C�e�ړ�����
			collisioncheck();//�e��Փ˃`�F�b�N
		}
		scroll_drawground();//�X�N���[�����w�i�`��
		drawgndenemy();//�n��G�`��
		drawcannon();//�C�e�`��
		drawenemy();//�󒆓G�`��
		drawship();//���@�`��
		drawmissile();//�~�T�C���`��
		drawscore();//���_�`��
		changegamestatus();//�Q�[���̃X�e�[�^�X�X�V����
		pausecheck();//��~�{�^���`�F�b�N
		gcount++;//�S�̃J�E���^

	}
	gameover(); //�Q�[���I������
}

int main(void){
	/* �|�[�g�̏����ݒ� */
	TRISB = 0x0000;	// �S�ďo��
	TRISC = 0x0000;	// �S�ďo��
	TRISD = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// �{�^���ڑ��|�[�g���͐ݒ�
	TRISE = 0x0000;	// �S�ďo��
	TRISF = 0x0003;						// RF0,1�͓���
	TRISG = 0x0080;	// RG7�͓���
	ANSELB = 0x0000; // �S�ăf�W�^��
	ANSELD = 0x0000; // �S�ăf�W�^��
	ANSELG = 0x0000; // �S�ăf�W�^��
	CNPUDSET=KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// �v���A�b�v�ݒ�

   	// ���Ӌ@�\�s�����蓖��
	SDI2R = 1;							//RPG7��SDI2�����蓖��
	RPG8R = 6;							//RPG8��SDO2�����蓖��

    //�T�E���h�֌W�����ݒ�(R)
//    RPF4R=0x0B;    //RPF4��OC3�����蓖��
//    RPD10R=0x0B;   // RPD10��OC3�����蓖��
//    OC3R=0;
//    OC3CON=0x800B; // Tiemr3�x�[�X�AToggle���[�h�ŗL����
	//�T�E���h�֌W�����ݒ�(L)
	RPF5R = 0x0B;  //RPF5��OC4�����蓖��
	RPD11R=0x0B;   // RPD11��OC4�����蓖��
	OC4R=0;
	OC4CON=0x800B; // Tiemr3�x�[�X�AToggle���[�h�ŗL����
	PR3=0;// Timer3����
	T3CON=0x8040; // �v���X�P�[��1:16��Timer3�J�n

#ifdef BOOTLOADER
	init_recscore();//�u�[�g���[�_�Ή��̋��L�n�C�X�R�A�L�^�̈���������A���R�[�h���Ȃ���ΐV�K�쐬
	if(read_recscore(0)==0xffffffff){
		highscore=1000;
		write_recscore(highscore,0);//�n�C�X�R�A�����l10000�_
	}
	else highscore=read_recscore(0); //�u�[�g���[�_�Ή����ʗ̈悩�烌�R�[�h��1�ڂ�32�r�b�g��ǂݏo��
#else
	if(*(unsigned int *)FLASHPAGE==-1){ //�t���b�V���������Ƀn�C�X�R�A���������܂�Ă��邩�`�F�b�N
		highscore=1000;
		writehighscore();
	}
	else highscore=*(unsigned int *)FLASHPAGE;
#endif

	init_rotateLCD(); // �r�f�I�������N���A�A�t����ʉ�]���C�u����������

	gameinit(); //�Q�[���S�̏�����
	while(1){
		title();//�^�C�g����ʁA�X�^�[�g�{�^���Ŗ߂�
		game();//�Q�[�����C�����[�v
	}
}
