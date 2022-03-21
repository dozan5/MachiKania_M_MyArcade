// SD�J�[�h�u�[�g���[�_�[�@PIC32MX370F512H�p�@by K.Tanaka

// SD�J�[�h�ڑ��|�[�g
// SPI DO RG8  (O)
// SPI DI RG7  (I)
// SPI CLKO RG6  (O)
// CS  RF3  (O)
// WE  RF1  0
// CD  RF0  0

#include <plib.h>
#include "LCDdriver.h"
#include "FSIO.h"
#include "NVMem.h"
#include "sd_bootloader.h"
#include "Bootloader.h"
#include <string.h>
#include "HardwareProfile.h"

//�O�t���N���X�^�� with PLL (20/3�{)
//�N���X�^����3.579545�~4��14.31818MHz
// DEVCFG3
#pragma config FSRSSEL = PRIORITY_7     // Shadow Register Set Priority Select (SRS Priority 7)
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)

// DEVCFG2
#pragma config FPLLIDIV = DIV_3         // PLL Input Divider (3x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = XT
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is Disabled)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// ���̓{�^���̃|�[�g�A�r�b�g��`
#define KEYPORT PORTD
#define KEYDOWN 0x0001  // RD0
#define KEYLEFT 0x0002  // RD1
#define KEYUP 0x0004    // RD2
#define KEYRIGHT 0x0008 // RD3
#define KEYSTART 0x0010 // RD4
#define KEYFIRE 0x0020  // RD5

#define clearscreen() LCD_Clear(0)

#define MAXFILENUM 75

/******************************************************************************
Macros used in this file
*******************************************************************************/
#define AUX_FLASH_BASE_ADRS				(0x7FC000)
#define AUX_FLASH_END_ADRS				(0x7FFFFF)
#define DEV_CONFIG_REG_BASE_ADDRESS 	(0xF80000)
#define DEV_CONFIG_REG_END_ADDRESS   	(0xF80012)
#define FILENAME_FLASH_ADDRESS (APP_FLASH_END_ADDRESS-15)

/******************************************************************************
Global Variables
*******************************************************************************/
FSFILE * myFile;
BYTE myData[512];
size_t numBytes;
UINT pointer = 0;
UINT readBytes;

UINT8 asciiBuffer[1024];
UINT8 asciiRec[200];
UINT8 hexRec[100];
UINT8 filenames[MAXFILENUM][13];
T_REC record;
SearchRec sr;

UINT16 oldkey=0;

//unsigned char *cursor;
//unsigned char cursorcolor;

extern const unsigned char FontData[];
unsigned short palette[256];
unsigned char cursorx,cursory,cursorc;

/****************************************************************************
Function prototypes
*****************************************************************************/
void JumpToApp(void);
BOOL ValidAppPresent(void);
BOOL loadApp(UINT8 *imagefile);

void set_palette(unsigned char n,unsigned char b,unsigned char r,unsigned char g){
//�O���t�B�b�N�p�J���[�p���b�g�ݒ�
	palette[n]=((r>>3)<<11)+((g>>2)<<5)+(b>>3);
}
void init_graphic(void){
	//�O���t�B�b�NLCD�g�p�J�n
	int i;
	//�J���[�p���b�g������
	for(i=0;i<8;i++){
		set_palette(i,255*(i&1),255*((i>>1)&1),255*(i>>2));
	}

   	OSCCONCLR=0x10; // WAIT���߂̓A�C�h�����[�h
	INTEnableSystemMultiVectoredInt();
	SYSTEMConfig(96000000,SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE); //�L���b�V���L�����i���ӃN���b�N�ɂ͓K�p���Ȃ��j
	BMXCONCLR=0x40;	// RAM�A�N�Z�X�E�F�C�g0

	INTEnableSystemMultiVectoredInt();
	T4CON=0;
	IPC4bits.T4IP = 4;			// ���荞�݃��x��4
	IFS0bits.T4IF = 0;
	IEC0bits.T4IE = 1;			// �^�C�}4���荞�ݗL����
	PR4=3125;               	// 60����1�b
	T4CON = 0x0070;				// �v���X�P�[��1:256

	LCD_Clear(0);
}

UINT16 keycheck(void){
	//�{�^����ԓǂݍ���
	//������Ă���Ίe�{�^���ɊY������bit��1�ɂ���
	//��x�{�^���𗣂��Ȃ��ƁA���������Ƃɂ͂Ȃ�Ȃ�
	UINT16 k1,k2;
	k1=~KEYPORT & (KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
	k2=k1 & ~oldkey;
	oldkey=k1;
	return k2;
}

void setcursor(unsigned char x,unsigned char y,unsigned char c){
	//�J�[�\�������W(x,y)�ɃJ���[�ԍ�c�ɐݒ�
	cursorx=x*8;
	cursory=y*8;
	cursorc=c;
}

void setcursorcolor(unsigned char c){
	//�J�[�\���ʒu���̂܂܂ŃJ���[�ԍ���c�ɐݒ�
	cursorc=c;
}

void putfont(int x,int y,unsigned char c,int bc,unsigned char n)
//8*8�h�b�g�̃A���t�@�x�b�g�t�H���g�\��
//���W(x,y)�A�J���[�p���b�g�ԍ�c
//bc:�o�b�N�O�����h�J���[�A�����̏ꍇ����
//n:�����ԍ�
{
	int i,j;
	unsigned char d;
	int skip;
	unsigned short c1;
	const unsigned char *p;
	if(x<=-8 || x>=LCD_X_RES || y<=-8 || y>=LCD_Y_RES) return; //��ʊO
	if(y<0){ //��ʏ㕔�ɐ؂��ꍇ
		i=0;
		p=FontData+n*8-y;
	}
	else{
		i=y;
		p=FontData+n*8;
	}
	c1=palette[c];
	if(bc>=0) bc=palette[bc];
	for(;i<y+8;i++){
		if(i>=LCD_Y_RES) return; //��ʉ����ɐ؂��ꍇ
		d=*p++;
		if(x<0){ //��ʍ��ɐ؂��ꍇ�͎c�镔���̂ݕ`��
			j=0;
			d<<=-x;
		}
		else{
			j=x;
		}
		if(bc<0){
			skip=1;
			for(;j<x+8;j++){
				if(j>=LCD_X_RES){ //��ʉE�ɐ؂��ꍇ
					break;
				}
				if(d&0x80){
					if(skip){
						LCD_SetCursor(j,i);
//						LCD_WriteIndex(0x22);
						skip=0;
					}
					LCD_WriteData(c1);
				}
				else skip=1;
				d<<=1;
			}
		}
		else{
			LCD_SetCursor(j,i);
//			LCD_WriteIndex(0x22);
			for(;j<x+8;j++){
				if(j>=LCD_X_RES){ //��ʉE�ɐ؂��ꍇ
					break;
				}
				if(d&0x80){
					LCD_WriteData(c1);
				}
				else{
					LCD_WriteData(bc);
				}
				d<<=1;
			}
		}
	}
}

void printchar(unsigned char n){
	//�J�[�\���ʒu�Ƀe�L�X�g�R�[�hn��1�����\��

	if(n=='\n'){
		cursorx=0;
		cursory+=8;
	}
	else{
		// n��0x20?0x7f�̂ݗL���B�t�H���g�f�[�^��0x00?0x5f�Ƃ��ĕۗL
		if(n<0x20 || n>=0x80) n='?'-0x20;
		else n-=0x20;
		putfont(cursorx,cursory,cursorc,0,n);
		cursorx+=8;
		if(cursorx>=LCD_X_RES){
			cursorx=0;
			cursory+=8;
		}
	}
	if(cursory>=LCD_Y_RES) cursory=0;
}

void printstr(unsigned char *s){
	//�J�[�\���ʒu�ɕ�����s��\��
	while(*s) printchar(*s++);
}

void waitanykey(void){
	//�ǂꂩ�̃{�^�����������܂ő҂�
	printstr("Push Any Button\n");
	while(keycheck()==0) ;
}

void writestringflash(void *p,UINT8 *s){
	//�t���b�V���̃A�h���Xp�ɕ�����s���������݁i�t���b�V���͏����ς݂ł��邱�Ɓj
	UINT d;
	UINT8 *bufp;
	while(1){
		bufp=(UINT8 *)&d;
		while(bufp<(UINT8 *)&d+4){
			*bufp=*s;
			bufp++;
			if(*s==0) break;
			s++;
		}
		while(bufp<(UINT8 *)&d+4) *bufp++=0;
		NVMemWriteWord(p,d);
		if(*(bufp-1)==0) break;
		p+=4;
	}
}

int main(void){
	int filenum,i;
	UINT8 *p1,*p2;
	UINT16 k;
	unsigned char x,y;

	/* �|�[�g�̏����ݒ� */
	TRISB = 0x0000;						// �S�ďo��
	TRISC = 0x0000;						// �S�ďo��
	TRISD = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// �{�^���ڑ��|�[�g���͐ݒ�
	TRISE = 0x0000;						// �S�ďo��
	TRISF = 0x0003;						// RF0,1�͓���
	TRISG = 0x0080;						// RG7�͓���
	ANSELB = 0x0000;                    // �S�ăf�W�^��
	ANSELD = 0x0000;                    // �S�ăf�W�^��
	ANSELG = 0x0000;                    // �S�ăf�W�^��
	CNPUDSET= KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// �v���A�b�v�ݒ�

	SDI2R = 1;							//RPG7��SDI2�����蓖��
	RPG8R = 6;							//RPG8��SDO2�����蓖��

   	LCD_Init();

 	for(i=0;i<100;i++) asm volatile("nop"); // �{�^���ڑ��|�[�g�̃v���A�b�v���L���ɂȂ�̂�҂�
	if(keycheck()==0) JumpToApp(); // �����{�^���������Ă��Ȃ���΁A���̂܂܃A�v���ɃW�����v

	init_graphic();

	while(1){
		clearscreen();//��ʏ���
		setcursor(0,0,7);
		printstr("Init File System...");
		// Initialize the File System
	   	if(!FSInit())
	   	{
		   	//�G���[�̏ꍇ
		   	setcursorcolor(4);
		   	printstr("Error\n");
			printstr("Insert Correct Card and\n");
			waitanykey();
			continue;
		}
		printstr("OK\n");

		//HEX�t�@�C���̈ꗗ��SD�J�[�h����ǂݏo��
		filenum=0;
		if(FindFirst("*.hex",ATTR_MASK,&sr)){
			setcursorcolor(4);
			printstr("No HEX File Found\n");
			printstr("Insert Correct Card and\n");
			waitanykey();
			continue;
		}
		do{
			//filenames[]��HEX�t�@�C�����̈ꗗ��ǂݍ���
			p1=sr.filename;
			p2=filenames[filenum];
			while(*p1!=0) *p2++=*p1++;
			*p2=0;
			filenum++;
		}
		while(!FindNext(&sr) && filenum<MAXFILENUM);

		//HEX�t�@�C���ꗗ����ʂɕ\��
		clearscreen();
		setcursor(0,0,4);
   		printstr("MachiKania Boot:Mov");
		setcursorcolor(5);
		printchar('>');
		setcursorcolor(4);
		printstr("/Push FIRE");

		if(ValidAppPresent()){
			setcursorcolor(6);
			printstr(" Not Load & Jump to [");
			//���݃t���b�V���ɏ������܂�Ă���A�v������\��
			p1=(UINT8 *)FILENAME_FLASH_ADDRESS;
			while(*p1!='.') printchar(*p1++);
			printchar(']');
		}
		for(i=0;i<filenum;i++){
			//�t�@�C�����ꗗ�̕\���i".HEX"�͏ȗ��j
			x=(i%3)*10+1;
			y=i/3+2;
			setcursor(x,y,7);
			p1=filenames[i];
			while(*p1!='.') printchar(*p1++);
		}

		//HEX�t�@�C���̑I��
		if(ValidAppPresent()){
			i=-1;
			setcursor(0,1,5);
		}
		else{
			i=0;
			setcursor(0,2,5);
		}
		printchar('>');
		cursorx-=8;
		T4CONSET=0x8000;//�^�C�}4�J�n
		while(1){
			drawcount=0;
			while(drawcount==0) asm("wait"); //60����1�b�E�F�C�g
			k=keycheck();
			if(k &(KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT)){
				printchar(' ');
				switch(k){
					case KEYUP:
						if(i>=3) i-=3;
						else if(ValidAppPresent()) i=-1;
						break;
					case KEYDOWN:
						if(i==-1) i=0;
						else if(i+3<filenum) i+=3;
						break;
					case KEYLEFT:
						if(i>0) i--;
						break;
					case KEYRIGHT:
						if(i+1<filenum) i++;
				}
				if(i>=0) setcursor((i%3)*10,i/3+2,5);
				else setcursor(0,1,5);
				printchar('>');
				cursorx-=8;
			}
			else if(k==KEYFIRE) break;
		}
		T4CONCLR=0x8000;//�^�C�}2��~
		clearscreen();
		if(i>=0){
			if(loadApp(filenames[i])==FALSE){
				//���[�h���ɉ��炩�̃G���[����
				setcursorcolor(2);
				printstr("Load Error\n");
				waitanykey();
				continue;
			}
			//�t�@�C�����L�^�̈��HEX�t�@�C��������������
			writestringflash((void *)FILENAME_FLASH_ADDRESS,filenames[i]);
		}
		JumpToApp();//���[�U�[�A�v���ɃW�����v�B�A�v�������݂��Ȃ��ꍇ�̂ݖ߂��Ă���
		setcursorcolor(2);
		printstr("Application Not Found\n");
		waitanykey();
	}
}
BOOL loadApp(UINT8 *imagefile){
	int i,result;
	int c,s;
	setcursor(0,1,7);
	myFile = FSfopen(imagefile,"r");
	if(myFile == NULL)// Make sure the file is present.
	{
		return FALSE;
	}

	// Erase Flash (Block Erase the program Flash)
	printstr("Erasing Flash Memory\n\n");
	EraseFlash();
	printstr("Loading ");
	setcursorcolor(4);
	printstr(imagefile);
	printchar('\n');printchar('\n');
	setcursorcolor(6);
	// Initialize the state-machine to read the records.
	record.status = REC_NOT_FOUND;
	c=0;
	s=0;
	while(1){
		// For a faster read, read 512 bytes at a time and buffer it.
		readBytes = FSfread((void *)&asciiBuffer[pointer],1,512,myFile);
		s+=readBytes;
		if(s > myFile->size*c/30){
			printchar('*');
			c++;
		}
		if(readBytes == 0){
			// Nothing to read. Come out of this loop
			// Something fishy. The hex file has ended abruptly, looks like there was no "end of hex record".
			FSfclose(myFile);
			EraseFlash();//�r���܂ŏ������񂾉\��������̂ŏ���
			return FALSE;
		}
		for(i = 0; i < (readBytes + pointer); i ++){
			// This state machine seperates-out the valid hex records from the read 512 bytes.
			switch(record.status){
				case REC_FLASHED:
				case REC_NOT_FOUND:
					if(asciiBuffer[i] == ':'){
						// We have a record found in the 512 bytes of data in the buffer.
						record.start = &asciiBuffer[i];
						record.len = 0;
						record.status = REC_FOUND_BUT_NOT_FLASHED;
					}
					break;
				case REC_FOUND_BUT_NOT_FLASHED:
					if((asciiBuffer[i] == 0x0A) || (asciiBuffer[i] == 0xFF)){
						// We have got a complete record. (0x0A is new line feed and 0xFF is End of file)
						// Start the hex conversion from element
						// 1. This will discard the ':' which is
						// the start of the hex record.
						ConvertAsciiToHex(&record.start[1],hexRec);
						result=WriteHexRecord2Flash(hexRec);
						if(result==WRITEHEXEND){
							FSfclose(myFile);
							return TRUE;//���[�h����
						}
						else if(result==WRITEHEXERROR){
							FSfclose(myFile);
							EraseFlash();//�r���܂ŏ������񂾉\��������̂ŏ���
							return FALSE;//�G���[�I��
						}
						record.status = REC_FLASHED;
					}
					break;
			}
			// Move to next byte in the buffer.
			record.len ++;
		}

		if(record.status == REC_FOUND_BUT_NOT_FLASHED){
			// We still have a half read record in the buffer. The next half part of the record is read
			// when we read 512 bytes of data from the next file read.
			memcpy(asciiBuffer, record.start, record.len);
			pointer = record.len;
			record.status = REC_NOT_FOUND;
		}
		else{
			pointer = 0;
		}
	}
}

/********************************************************************
* Function: 	JumpToApp()
*
* Precondition:
*
* Input: 		None.
*
* Output:
*
* Side Effects:	No return from here.
*
* Overview: 	Jumps to application.
*
*
* Note:		 	None.
********************************************************************/
void JumpToApp(void)
{
	void (*fptr)(void);
	if(!ValidAppPresent()) return;
	while((~KEYPORT & KEYFIRE)) ; //FIRE�{�^���𗣂��܂ő҂�
	fptr = (void (*)(void))USER_APP_RESET_ADDRESS;
	fptr();
}

/********************************************************************
* Function: 	ConvertAsciiToHex()
*
* Precondition:
*
* Input: 		Ascii buffer and hex buffer.
*
* Output:
*
* Side Effects:	No return from here.
*
* Overview: 	Converts ASCII to Hex.
*
*
* Note:		 	None.
********************************************************************/
void ConvertAsciiToHex(UINT8* asciiRec, UINT8* hexRec)
{
	UINT8 i = 0;
	UINT8 k = 0;
	UINT8 hex;


	while((asciiRec[i] >= 0x30) && (asciiRec[i] <= 0x66))
	{
		// Check if the ascci values are in alpha numeric range.

		if(asciiRec[i] < 0x3A)
		{
			// Numerical reperesentation in ASCII found.
			hex = asciiRec[i] & 0x0F;
		}
		else
		{
			// Alphabetical value.
			hex = 0x09 + (asciiRec[i] & 0x0F);
		}

		// Following logic converts 2 bytes of ASCII to 1 byte of hex.
		k = i%2;

		if(k)
		{
			hexRec[i/2] |= hex;

		}
		else
		{
			hexRec[i/2] = (hex << 4) & 0xF0;
		}
		i++;
	}

}

// PIC32MX1xx/2xx�ȊO�̓t���b�V���̃y�[�W��4KB�P��
#define FLASH_PAGE_SIZE 0x1000
/********************************************************************
* Function: 	EraseFlash()
*
* Precondition:
*
* Input: 		None.
*
* Output:
*
* Side Effects:	No return from here.
*
* Overview: 	Erases Flash (Block Erase).
*
*
* Note:		 	None.
********************************************************************/
void EraseFlash(void)
{
    void * pFlash;
    UINT result;
    INT i;

    pFlash = (void*)APP_FLASH_BASE_ADDRESS;
    for( i = 0; i < ((APP_FLASH_END_ADDRESS - APP_FLASH_BASE_ADDRESS + 1)/FLASH_PAGE_SIZE); i++ )
    {
	     result = NVMemErasePage( pFlash + (i*FLASH_PAGE_SIZE) );
        // Assert on NV error. This must be caught during debug phase.

        if(result != 0)
        {
           // We have a problem. This must be caught during the debug phase.
           printstr("Flash Erase Error");
           while(1) asm("wait");
        }
    }
}

int WriteHexRecord2Flash(UINT8* HexRecord)
{
/*
	HEX���R�[�h1�s�����t���b�V���ɏ�������
	�߂�l
		WRITEHEXNOTEND:����I���A�ŏI�s�ł͂Ȃ�
		WRITEHEXEND:����I���A�ŏI�s
		WRITEHEXERROR:�G���[
*/
	static T_HEX_RECORD HexRecordSt;
	UINT8 Checksum = 0;
	UINT8 i;
	UINT WrData;
	UINT RdData;
	void* ProgAddress;
	UINT result;

	HexRecordSt.RecDataLen = HexRecord[0];
	HexRecordSt.RecType = HexRecord[3];
	HexRecordSt.Data = &HexRecord[4];

	// Hex Record checksum check.
	for(i = 0; i < HexRecordSt.RecDataLen + 5; i++)
	{
		Checksum += HexRecord[i];
	}

    if(Checksum != 0)
    {
	    //Error. Hex record Checksum mismatch.
	    printstr("Checksum Error\n");
	    return WRITEHEXERROR;
	}
	else
	{
		// Hex record checksum OK.
		switch(HexRecordSt.RecType)
		{
			case DATA_RECORD:  //Record Type 00, data record.
				HexRecordSt.Address.byte.MB = 0;
				HexRecordSt.Address.byte.UB = 0;
				HexRecordSt.Address.byte.HB = HexRecord[1];
				HexRecordSt.Address.byte.LB = HexRecord[2];

				// Derive the address.
				HexRecordSt.Address.Val = HexRecordSt.Address.Val + HexRecordSt.ExtLinAddress.Val + HexRecordSt.ExtSegAddress.Val;

				while(HexRecordSt.RecDataLen) // Loop till all bytes are done.
				{

					// Convert the Physical address to Virtual address.
					ProgAddress = (void *)PA_TO_KVA0(HexRecordSt.Address.Val);

					// Make sure we are not writing boot area and device configuration bits.
					if(((ProgAddress >= (void *)APP_FLASH_BASE_ADDRESS) && (ProgAddress <= (void *)APP_FLASH_END_ADDRESS))
					   && ((ProgAddress < (void*)DEV_CONFIG_REG_BASE_ADDRESS) || (ProgAddress > (void*)DEV_CONFIG_REG_END_ADDRESS)))
					{
						if(HexRecordSt.RecDataLen < 4)
						{

							// Sometimes record data length will not be in multiples of 4. Appending 0xFF will make sure that..
							// we don't write junk data in such cases.
							WrData = 0xFFFFFFFF;
							memcpy(&WrData, HexRecordSt.Data, HexRecordSt.RecDataLen);
						}
						else
						{
							memcpy(&WrData, HexRecordSt.Data, 4);
						}
						// Write the data into flash.
						result = NVMemWriteWord(ProgAddress, WrData);
						// Assert on error. This must be caught during debug phase.
						if(result != 0)
						{
							printstr("Write Error");
   							while(1) asm("wait");
   						}
					}

					// Increment the address.
					HexRecordSt.Address.Val += 4;
					// Increment the data pointer.
					HexRecordSt.Data += 4;
					// Decrement data len.
					if(HexRecordSt.RecDataLen > 3)
					{
						HexRecordSt.RecDataLen -= 4;
					}
					else
					{
						HexRecordSt.RecDataLen = 0;
					}
				}
				break;

			case EXT_SEG_ADRS_RECORD:  // Record Type 02, defines 4th to 19th bits of the data address.
			    HexRecordSt.ExtSegAddress.byte.MB = 0;
				HexRecordSt.ExtSegAddress.byte.UB = HexRecordSt.Data[0];
				HexRecordSt.ExtSegAddress.byte.HB = HexRecordSt.Data[1];
				HexRecordSt.ExtSegAddress.byte.LB = 0;
				// Reset linear address.
				HexRecordSt.ExtLinAddress.Val = 0;
				break;

			case EXT_LIN_ADRS_RECORD:   // Record Type 04, defines 16th to 31st bits of the data address.
				HexRecordSt.ExtLinAddress.byte.MB = HexRecordSt.Data[0];
				HexRecordSt.ExtLinAddress.byte.UB = HexRecordSt.Data[1];
				HexRecordSt.ExtLinAddress.byte.HB = 0;
				HexRecordSt.ExtLinAddress.byte.LB = 0;
				// Reset segment address.
				HexRecordSt.ExtSegAddress.Val = 0;
				break;

			case END_OF_FILE_RECORD:  //Record Type 01, defines the end of file record.
				HexRecordSt.ExtSegAddress.Val = 0;
				HexRecordSt.ExtLinAddress.Val = 0;
				// Disable any interrupts here before jumping to the application.
				return WRITEHEXEND; //�ŏI�s
				// JumpToApp();
				break;

			default:
				HexRecordSt.ExtSegAddress.Val = 0;
				HexRecordSt.ExtLinAddress.Val = 0;
				break;
		}
	}
	return WRITEHEXNOTEND; //�ŏI�s�ł͂Ȃ�
}

/********************************************************************
* Function: 	ValidAppPresent()
*
* Precondition:
*
* Input: 		None.
*
* Output:		TRUE: If application is valid.
*
* Side Effects:	None.
*
* Overview: 	Logic: Check application vector has
				some value other than "0xFFFFFF"
*
*
* Note:		 	None.
********************************************************************/
BOOL ValidAppPresent(void)
{
	volatile UINT32 *AppPtr;

	AppPtr = (UINT32*)USER_APP_RESET_ADDRESS;

	if(*AppPtr == 0xFFFFFFFF)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

