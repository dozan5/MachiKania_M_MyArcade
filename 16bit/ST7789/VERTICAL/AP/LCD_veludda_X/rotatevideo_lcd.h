// �x�N�g����VRAM�@�R���|�W�b�g�J���[�M���o�̓v���O�����@PIC32MX370F512H�p�w�b�_�[�t�@�C���@by K.Tanaka
// ��ʏ㕔�͌Œ�VRAM

#define X_RES	256 // �������𑜓x
#define Y_RES	224 // �c�����𑜓x
#define VRAM_X   256 // VRAM�̉������ő�l
#define VRAM_Y   256 // VRAM�̏c�����ő�l
#define TOPLINE 8 // ��ʏ㕔�̌Œ�\���s��

extern volatile unsigned short drawcount;		//�@1��ʕ\���I�����Ƃ�1�����B�A�v������0�ɂ���B
							// �Œ�1��͉�ʕ\���������Ƃ̃`�F�b�N�ƁA�A�v���̏���������ʊ��ԕK�v���̊m�F�ɗ��p�B
extern unsigned char VRAM[]; // �r�f�I������
extern unsigned char TOPVRAM[]; // ��ʏ㕔�̌Œ�\���p�r�f�I������
extern volatile short vscanv1_x,vscanv1_y,vscanv2_x,vscanv2_y;	//�f���\���X�L�����p�x�N�g��
extern volatile short vscanstartx,vscanstarty; //�f���\���X�L�����J�n���W
// (vstartx,vstarty):��ʍ���ɂȂ�VRAM��̍��W�i256�{�j
// (vscanv1_x,vscanv1_y):��ʉE�����̃X�L�����x�N�g���i256�{�j
// (vscanv2_x,vscanv2_y):��ʉ������̃X�L�����x�N�g���i256�{�j

extern void init_rotateLCD(void); //�t����ʉ�]���C�u����������
extern void clearscreen(void); //��ʃN���A
extern void set_palette(unsigned char n,unsigned char b,unsigned char r,unsigned char g); //�J���[�p���b�g�ݒ�
