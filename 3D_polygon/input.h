#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

//���C�u�����̃����N
#pragma comment(lib,"xinput.lib")				//Gamepad���͏����ɕK�v

//�{�^���̗񋓌^
typedef enum
{
	PADBUTTON_0 = 0,			//��{�^��
	PADBUTTON_1,				//���{�^��
	PADBUTTON_2,				//���{�^��
	PADBUTTON_3,				//�E�{�^��
	PADBUTTON_4,				//�X�^�[�g�{�^��
	PADBUTTON_5,				//�o�b�N�{�^��
	PADBUTTON_6,				//L�X�e�B�b�N��������
	PADBUTTON_7,				//R�X�e�B�b�N��������
	PADBUTTON_8,				//LB�{�^��
	PADBUTTON_9,				//RB�{�^��
	PADBUTTON_10,				//�X�e�B�b�N�g���K�[�H
	PADBUTTON_11,				//�X�e�B�b�N�g���K�[�H
	PADBUTTON_12,				//A�{�^��
	PADBUTTON_13,				//B�{�^��
	PADBUTTON_14,				//Y�{�^��
	PADBUTTON_15,				//X�{�^��
	PADBUTTON_16,				//L�X�e�B�b�N���E�v���X���
	PADBUTTON_17,				//L�X�e�B�b�N�΂ߏ㉺�v���X
	PADBUTTON_18,				//�������H
	PADBUTTON_MAX
}PADBUTTON;

//�v���g�^�C�v�錾
HRESULT InitKeyboard(HINSTANCE hInstance,HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);

HRESULT InitGamepad(void);
void UninitGamepad(void);
void UpdateGamepad(void);
bool GetPadPress(PADBUTTON button,int nCntPad);
bool GetPadTrigger(PADBUTTON button,int nCntPad);

SHORT GetGameStickLXPress(PADBUTTON Key, int nPlayer);
SHORT GetGameStickLYPress(PADBUTTON Key, int nPlayer);
SHORT GetGameStickLXTrigger(PADBUTTON Key, int nPlayer);
SHORT GetGameStickLYTrigger(PADBUTTON Key, int nPlayer);

#endif