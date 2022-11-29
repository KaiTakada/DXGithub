#ifndef _MAIN_H_								//���̃}�N����`������ĂȂ�������
#define _MAIN_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

#include <windows.h>
#include "d3dx9.h"								//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)			//�r���h���̌x���Ώ��}�N���p
#include "dinput.h"								//���͏����ɕK�v
#include "xaudio2.h"							//�T�E���h�����ɕK�v

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")					//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")				//�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")				//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")				//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")				//���͏����ɕK�v

//�}�N����`
#define SCREEN_WIDTH (1280)						//�E�B���h�E�̕�
#define SCREEN_HEIGHT (720)						//�E�B���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//���_�t�H�[�}�b�g[2D]
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//���_�t�H�[�}�b�g[3D]

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;							//���_���W
	float rhw;									//
	D3DCOLOR col;								//���_�J���[
	D3DXVECTOR2 tex;							//�e�N�X�`�����W
}VERTEX_2D;

//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;							//���_���W
	D3DXVECTOR3 nor;							//�@���x�N�g��
	D3DCOLOR col;								//���_�J���[
	D3DXVECTOR2 tex;							//�e�N�X�`�����W
}VERTEX_3D;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void DrawFPS(void);
LPD3DXFONT GetFont(void);

#endif