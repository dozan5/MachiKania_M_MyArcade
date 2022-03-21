// veludda.h by K.Tanaka

// ���̓{�^���̃|�[�g�A�r�b�g��`
#define KEYPORT PORTD
#define KEYDOWN 0x0001
#define KEYLEFT 0x0002
#define KEYUP 0x0004
#define KEYRIGHT 0x0008
#define KEYSTART 0x0010
#define KEYFIRE 0x0020

#define MAPDX (256*4) //�S�̃}�b�v�̉��T�C�Y�i�r�b�g�}�b�v�f�[�^��4����1�̂݁j
#define MAPDY (128*4) //�S�̃}�b�v�̏c�T�C�Y�i�r�b�g�}�b�v�f�[�^��4����1�̂݁j
#define NORMALIZEX(x) ((x)&(MAPDX*256-1)) //��΍��W�̉E�[����͂ݏo�������������[����̈ʒu�ɂ���}�N��
#define NORMALIZEY(y) ((y)&(MAPDY*256-1)) //��΍��W�̉��[����͂ݏo������������[����̈ʒu�ɂ���}�N��
//#define MAPDXMASK (MAPDX*256-1) //��΍��W�̉E�[����͂ݏo�������������[�ɂ��邽�߂̃}�X�N�l
//#define MAPDYMASK (MAPDY*256-1) //��΍��W�̉��[����͂ݏo������������[�ɂ��邽�߂̃}�X�N�l
#define ADX1 (128/4*3) //���@��X�ʒu�����ʍ��[�܂ł̋���
#define ADY1 (180/4*3) //���@��Y�ʒu�����ʏ�[�܂ł̋���
#define ADX2 (X_RES/4*3-ADX1) //���@��X�ʒu�����ʉE�[�܂ł̋���
#define ADY2 ((Y_RES-TOPLINE)/4*3-ADY1) //���@��Y�ʒu�����ʉ��[�܂ł̋���

#define MAX_GNDENEMY 50 //�n��G�ő吔
#define MAX_ENEMY 50 //�󒆓G�ő吔
#define MAX_MISSILE 20 //�~�T�C���ő吔
#define MAX_CANNON 5 //�C�e�ő吔

#define XSIZE_GNDENEMY 14 //�n��G�摜���T�C�Y
#define YSIZE_GNDENEMY 14 //�n��G�摜�c�T�C�Y
#define SQSIZE_GNDENEMY (XSIZE_GNDENEMY*YSIZE_GNDENEMY)
#define XSIZE_ENEMY 12 //�󒆓G�摜���T�C�Y
#define YSIZE_ENEMY 12 //�󒆓G�摜�c�T�C�Y
#define SQSIZE_ENEMY (XSIZE_ENEMY*YSIZE_ENEMY)
#define XSIZE_SHIP 16 //���@�摜���T�C�Y
#define YSIZE_SHIP 16 //���@�摜�c�T�C�Y
#define SQSIZE_SHIP (XSIZE_SHIP*YSIZE_SHIP)
#define XSIZE_EXPLODE 12 //�G�����摜���T�C�Y
#define YSIZE_EXPLODE 12 //�G�����摜�c�T�C�Y
#define SQSIZE_EXPLODE (XSIZE_EXPLODE*YSIZE_EXPLODE)
#define XSIZE_EXPLODE2 16 //���@�����摜���T�C�Y
#define YSIZE_EXPLODE2 16 //���@�����摜�c�T�C�Y
#define SQSIZE_EXPLODE2 (XSIZE_EXPLODE2*YSIZE_EXPLODE2)
#define XSIZE_MISSILE 4 //�~�T�C���摜���T�C�Y
#define YSIZE_MISSILE 4 //�~�T�C���摜�c�T�C�Y
#define SQSIZE_MISSILE (XSIZE_MISSILE*YSIZE_MISSILE)
#define XSIZE_CANNON 8 //�C�e�摜���T�C�Y
#define YSIZE_CANNON 8 //�C�e�摜�c�T�C�Y
#define SQSIZE_CANNON (XSIZE_CANNON*YSIZE_CANNON)
#define XSIZE_LOGO 209 //�^�C�g�����S�摜���T�C�Y
#define YSIZE_LOGO 33 //�^�C�g�����S�摜�c�T�C�Y

#define SHIPS 5 //���@�̏����c��
#define CANNONFIRE 80 //�C�e���˔��f����
#define CONTINUEMAX 3 //�R���e�B�j���[�\��

//�󒆓G�A�n��G�p�\���̒�`
typedef struct {
	char on; // �L���^�����^������
	int x; // x���W*256
	int y; // y���W*256
	short vx; // x�����ړ����x*256
	short vy; // y�����ړ����x*256
	unsigned char no; // �G�ԍ�
	unsigned char count; // �J�E���^
	const unsigned char *bmp; // �摜�ւ̃|�C���^
} _Enemy;

//�~�T�C���A�C�e�p�\���̒�`
typedef struct {
	char on; // �L���^����
	int x; // x���W*256
	int y; // y���W*256
	short vx; // x�����ړ����x*256
	short vy; // y�����ړ����x*256
	unsigned char count; // �J�E���^
	const unsigned char *bmp; // �摜�ւ̃|�C���^
} _Missile;

//�O���ϐ��錾
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

extern unsigned char mapdata[],paldata2[];//�}�b�v�f�[�^�ƃp���b�g�f�[�^

extern const unsigned short sounddata[];//�����f�[�^
extern const unsigned short sounddata1[],sounddata2[],sounddata3[];//���ʉ��f�[�^
extern const unsigned char musicdata1[],musicdata2[],musicdata3[],musicdata4[];//�ȃf�[�^
