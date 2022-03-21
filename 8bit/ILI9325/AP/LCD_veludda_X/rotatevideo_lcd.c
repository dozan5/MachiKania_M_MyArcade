// �x�N�g����VRAM�@LCD�o�̓v���O�����@PIC32MX370F512H�p�@by K.Tanaka
// ��ʏ㕔�ɌŒ�\���̈悠��
//
// plib.h��p���߁AXC32 v1.33�ȍ~�ɂ͖��Ή�
//
// �o�̓o�X PORTB�iRB0-7�j
// VRAM�𑜓x256�~256�h�b�g�{�ŏ㕔�Œ�8�s
// ��ʏo�͉𑜓x256�~216�h�b�g�{�ŏ㕔8�s
// 256�F�����\���A1�o�C�g��1�h�b�g��\��
// �J���[�p���b�g�Ή�

// (vstartx,vstarty):��ʍ���ɂȂ�VRAM��̍��W�i256�{�j
// (vscanv1_x,vscanv1_y):��ʉE�����̃X�L�����x�N�g���i256�{�j
// (vscanv2_x,vscanv2_y):��ʉ������̃X�L�����x�N�g���i256�{�j


#include "rotatevideo_lcd.h"
#include "LCDdriver.h"
#include <plib.h>

#define nop()	asm volatile("nop")
#define nop5()	nop();nop();nop();nop();nop();
#define nop10()	nop5();nop5();

// �O���[�o���ϐ���`
unsigned char VRAM[VRAM_X*VRAM_Y] __attribute__ ((aligned (4))); //VRAM
unsigned char TOPVRAM[VRAM_X*TOPLINE] __attribute__ ((aligned (4))); //��ʏ㕔�̌Œ�VRAM
volatile short vscanv1_x,vscanv1_y,vscanv2_x,vscanv2_y;	//�f���\���X�L�����p�x�N�g��
volatile short vscanstartx,vscanstarty; //�f���\���X�L�����J�n���W
volatile short vscanx,vscany; //�f���\���X�L�������������W

volatile unsigned short drawcount;	//�@1��ʕ\���I�����Ƃ�1�����B�A�v������0�ɂ���B
					// �Œ�1��͉�ʕ\���������Ƃ̃`�F�b�N�ƁA�A�v���̏���������ʊ��ԕK�v���̊m�F�ɗ��p�B

//�J���[�M���g�`�e�[�u��
//256�F���̃J���[�p���b�g
unsigned short ClTable[256];

/**********************
*  Timer2 ���荞�ݏ����֐�
***********************/
void __ISR(8, ipl4) T2Handler(void)
{
	drawcount++;
	IFS0bits.T2IF = 0;			// T2���荞�݃t���O�N���A
}

void lineoutput(unsigned short x,unsigned short y,unsigned short vx,unsigned short vy,unsigned char *vp)
{
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
	asm volatile("addiu	$t5,$zero,0x200");//LCD's WR PORT bit
//	asm volatile("addiu	$t3,$zero,256");// loop counter
	asm volatile("addiu	$t3,$zero,240");// loop counter
asm volatile("label_loop1:");
	asm volatile("ext	$v0,$a0,8,8");
	asm volatile("rotr	$v1,$a1,8");
	asm volatile("ins	$v0,$v1,8,8");
	asm volatile("addu	$v0,$v0,$t2");
	asm volatile("lbu	$v1,0($v0)");
	asm volatile("sll	$v1,$v1,1");
	asm volatile("addu	$v1,$v1,$t0");
	asm volatile("lhu	$t4,0($v1)");
	asm volatile("sb	$t4,0($t1)"); // (byte)LATB=t4
	asm volatile("addu	$a0,$a0,$a2");
	asm volatile("addu	$a1,$a1,$a3");
	asm volatile("sw	$t5,4($t1)"); // LATBCLR=t5
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("sw	$t5,8($t1)"); // LATBSET=t5
	asm volatile("srl	$t4,$t4,8");
	asm volatile("sb	$t4,0($t1)"); // (byte)LATB=t4
	asm volatile("addiu	$t3,$t3,-1");
	asm volatile("sw	$t5,4($t1)"); // LATBCLR=t5
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("sw	$t5,8($t1)"); // LATBSET=t5
	asm volatile("bnez	$t3,label_loop1");
}

//�t���ɉ�ʃf�[�^��]��
void putlcdall(void){
	unsigned short x,y,i;
	for(i=0;i<TOPLINE;i++){
//		LCD_SetCursor(32,i+8);
		LCD_SetCursor(0,i+8);
		LCD_WriteIndex(0x22);
		LCD_RS_HI;
		lineoutput(0,i<<8,0x100,0,TOPVRAM);
	}
	x=vscanstartx;
	y=vscanstarty;
	for(i=0;i<216;i++){
//		LCD_SetCursor(32,i+16);
		LCD_SetCursor(0,i+16);
		LCD_WriteIndex(0x22);
		LCD_RS_HI;
		lineoutput(x,y,vscanv1_x,vscanv1_y,VRAM);
		x+=vscanv2_x;
		y+=vscanv2_y;
	}
}

// ��ʃN���A
void clearscreen(void)
{
	unsigned int *vp;
	int i;
	vp=(unsigned int *)VRAM;
	for(i=0;i<VRAM_X*VRAM_Y/4;i++) *vp++=0;
	vp=(unsigned int *)TOPVRAM;
	for(i=0;i<VRAM_X*TOPLINE/4;i++) *vp++=0;
}

void set_palette(unsigned char n,unsigned char b,unsigned char r,unsigned char g)
{
	// �J���[�p���b�g�ݒ�
	// n:�p���b�g�ԍ�0-255�Ar,g,b:0-255
	// R5G6B5�`���ŕۑ��A��������ʂƉ��ʂ����ւ�

	unsigned short c;
	c=((r&0xf8)<<8)+((g&0xfc)<<3)+((b&0xf8)>>3);
	ClTable[n]=c<<8;
	ClTable[n]+=c>>8;
}

// �t����ʉ�]���C�u����������
void init_rotateLCD(void)
{
	unsigned int i;
	clearscreen();
	//�J���[�p���b�g������
	for(i=0;i<8;i++){
		set_palette(i,255*(i&1),255*((i>>1)&1),255*(i>>2));
	}
	for(i=0;i<8;i++){
		set_palette(i+8,128*(i&1),128*((i>>1)&1),128*(i>>2));
	}
	for(i=16;i<256;i++){
		set_palette(i,255,255,255);
	}
	//VRAM�X�L�����x�N�g��������
	vscanv1_x=256;
	vscanv1_y=0;
	vscanv2_x=0;
	vscanv2_y=256;
	vscanstartx=0;
	vscanstarty=0;

	OSCCONCLR=0x10; // WAIT���߂̓A�C�h�����[�h
	INTEnableSystemMultiVectoredInt();
	SYSTEMConfig(96000000,SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE); //�L���b�V���L�����i���ӃN���b�N�ɂ͓K�p���Ȃ��j
	BMXCONCLR=0x40;	// RAM�A�N�Z�X�E�F�C�g0

	// Timer5�����ݒ�i�}�C�N���b�f�B���C�p�j
	T5CON = 0x0010;				// �v���X�P�[��1:2�A�^�C�}5��~���
	IPC5bits.T5IP = 4;			// ���荞�݃��x��4
	IFS0bits.T5IF = 0;
	IEC0bits.T5IE = 1;			// �^�C�}5���荞�ݗL����
	LCD_Clear(0);

	// Timer2�����ݒ�i60����1�b�J�E���^�j
	T2CON = 0x0050;				// �v���X�P�[��1:32�A�^�C�}2��~���
	IPC2bits.T2IP = 4;			// ���荞�݃��x��4
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;			// �^�C�}2���荞�ݗL����
	PR2=50000;				// 60����1�b
	T2CONSET = 0x8000;			// �^�C�}2�X�^�[�g
}
