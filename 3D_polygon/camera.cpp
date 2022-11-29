//============================
//
//�J�����̏���(camera.cpp)
// Author: Kai Takada
//
//============================
#include "main.h"
#include "camera.h"
#include "model.h"
#include "input.h"
#include "string.h"

//�}�N����`
#define CAMERA_SPEED (1.0f)				//�ړ��X�s�[�h
#define CAMERA_ROTSPEED (0.05f)			//��]�X�s�[�h
#define CAMERA_DISTANCE (300)			//���_�E�����_�̋���
#define CAMERA_INITPOSY (150.0f)		//���_�E�����_��y�����l
#define CAMERA_RADJT (60.0f)			//�����_�̂��炵�l

//�v���g�^�C�v�錾
void CameraMove(void);
void CameraVR(void);
void RotCamera(void);
void CameraPosRRotate(void);
void CameraPosVRotate(void);
void CameraPosRControll(void);
void CameraPosVControll(void);

//�O���[�o���ϐ���`
Camera g_camera;					//�J�����̏��
bool g_bVR = false;					//���_�E�����_�̐؂�ւ�
bool g_bVRButtonDown = false;		//���_�E�����_���E���삵�Ă��邩�ǂ���
bool g_bUButtonDown = false;		//���_�E�����_�̏㉺���삵�Ă��邩�ǂ���
float g_fVRLength;					//���_�E�����_�̋���(xz)
float g_fCameraAngle;				//���_�E�����_�̊p�x

//============================
//�J�����̏���������
//============================
void InitCamera(void)
{
	g_camera.fDis = CAMERA_DISTANCE;
	g_camera.posV = D3DXVECTOR3( 0.0f, CAMERA_INITPOSY, -g_camera.fDis);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posVDest = g_camera.posV;
	g_camera.posRDest = g_camera.posR;
	g_camera.posRAdjt = CAMERA_RADJT;
	g_camera.move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
//�J�����̏I������
//============================
void UninitCamera(void)
{

}

//============================
//�J�����̍X�V����
//============================
void UpdateCamera(void)
{
	//2�_�Ԃ̋���
	//RotCamera();

	//�J�����ړ�
	CameraMove();

	//���_�E�����_�̑���
	CameraVR();

	//CameraPosRRotate();
	
	CameraPosVRotate();
	
	CameraPosRControll();
	
	CameraPosVControll();
}

//============================
//�J�����̐ݒ菈��
//============================
void SetCamera(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,
								10000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,&g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,			//���_
						&g_camera.posR,			//�����_
						&g_camera.vecU);		//������x�N�g��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//�p�x�̎���
void RotCamera(void)
{
	//z���̋�����x���̋����̍��v(���ʂł̋���)
	g_fVRLength = (g_camera.posV.z - g_camera.posR.z)
				+ (g_camera.posV.x - g_camera.posR.x);

	if (g_fVRLength < 0)
	{
		g_fVRLength *= -1;
	}
}

//=======================================
//============  �J�����ړ�  =============
//=======================================
void CameraMove(void)
{
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//�ォ���L�[���������Ƃ�
			g_camera.posV.x += sinf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//y
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//�ォ�E�L�[���������Ƃ�
			g_camera.posV.x += sinf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//y
		}
		else
		{//��L�[�݂̂��������Ƃ�
			g_camera.posV.x += sinf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//y
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//�ォ���L�[���������Ƃ�
			g_camera.posV.x += sinf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//y
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//�ォ�E�L�[���������Ƃ�
			g_camera.posV.x += sinf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//y
		}
		else
		{//��L�[�݂̂��������Ƃ�
			g_camera.posV.x += sinf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//y
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{//���L�[�݂̂��������Ƃ�
		g_camera.posV.x += sinf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posV.z += cosf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//y
		g_camera.posR.x += sinf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posR.z += cosf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//y
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//�E�L�[�݂̂��������Ƃ�
		g_camera.posV.x += sinf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posV.z += cosf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//y
		g_camera.posR.x += sinf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posR.z += cosf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//y
	}

	g_camera.move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//�ړ��ʂ��X�V(����������)
	g_camera.move.x += (0 - g_camera.move.x) * 0.15f;
	g_camera.move.y += (0 - g_camera.move.y) * 0.15f;
}

//=======================================
//========  ���_�E�����_�̑���  =========
//=======================================
void CameraVR(void)
{
	g_bVRButtonDown = false;

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F2�L�[�������ꂽ
		g_bVR = g_bVR ? false : true;

		//D3DXVECTOR3 PosTemp = g_camera.posV;
		//g_camera.posV = g_camera.posR;
		//g_camera.posR = PosTemp;
	}

	//�����_�ƃ��f���̍��W�̍�
	g_camera.posRDest = D3DXVECTOR3(GetModelpos().x + sinf(GetModelRot().y + D3DX_PI) * g_camera.posRAdjt,
									GetModelpos().y,
									GetModelpos().z + cosf(GetModelRot().y + D3DX_PI) * g_camera.posRAdjt);
	
	//�ڕW�̎n�_��ݒ�
	g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * -g_camera.fDis;	//(x)

	D3DXVECTOR3 RDiff = D3DXVECTOR3(g_camera.posRDest.x - g_camera.posR.x, 0.0f, g_camera.posRDest.z - g_camera.posR.z);
	D3DXVECTOR3 VDiff = D3DXVECTOR3(g_camera.posVDest.x - g_camera.posV.x, 0.0f, g_camera.posVDest.z - g_camera.posV.z);

	//�����_���X�V(�ڋ߂�����)
	g_camera.posR += RDiff * 0.05f;

	//�����_���X�V(�ڋ߂�����)
	g_camera.posV += VDiff * 0.05f;
}

//===========================
//���_����
//===========================
void CameraPosVRotate(void)
{
	if (GetKeyboardPress(DIK_Z) == true || GetKeyboardPress(DIK_C) == true)
	{
		if (GetKeyboardPress(DIK_Z) == true)
		{
			g_camera.rot.y -= CAMERA_ROTSPEED;
		}
		else if (GetKeyboardPress(DIK_C) == true)
		{
			g_camera.rot.y += CAMERA_ROTSPEED;
		}
	}

	//���_�̐���
	g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y) * -g_camera.fDis;	//(x)
	g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y) * -g_camera.fDis;

	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}
}

//===========================
//�����_����
//===========================
void CameraPosRRotate(void)
{
	if (GetKeyboardPress(DIK_Q) == true || GetKeyboardPress(DIK_E) == true)
	{
		if (GetKeyboardPress(DIK_Q) == true)
		{
			g_camera.rot.y -= CAMERA_ROTSPEED;
		}
		else if (GetKeyboardPress(DIK_E) == true)
		{
			g_camera.rot.y += CAMERA_ROTSPEED;
		}
	}

	//�����_�̐���
	g_camera.posR.x = GetModelposDest().x + sinf(g_camera.rot.y);
	g_camera.posR.z = GetModelposDest().z + cosf(g_camera.rot.y);

	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}
}

//===========================
//�����_�݂̂̑���
//===========================
void CameraPosRControll(void)
{
	if (GetKeyboardPress(DIK_Y) == true)
	{//���_�̂ݏ㉺
		g_camera.posV.y += CAMERA_SPEED;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//���_�̂ݏ㉺
		g_camera.posV.y -= CAMERA_SPEED;
	}
}

//===========================
//���_�݂̂̑���
//===========================
void CameraPosVControll(void)
{
	if (GetKeyboardPress(DIK_T) == true)
	{//���_�̂ݏ㉺
		g_camera.posR.y += CAMERA_SPEED;
	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//���_�̂ݏ㉺
		g_camera.posR.y -= CAMERA_SPEED;
	}
}

//===========================
//��荞�ݏ���
//===========================
void CameraStoke(void)
{
	g_camera.rotDest.y = GetModelRot().y + D3DX_PI;

	//����
	float fDiff = 0.0f;
	fDiff = g_camera.rotDest.y - g_camera.rot.y;

	//PI����͂ݏo�Ă����ꍇ�̕␳
	if (fDiff < -D3DX_PI)
	{
		fDiff += D3DX_PI * 2;
	}
	else if (fDiff > D3DX_PI)
	{
		fDiff += -D3DX_PI * 2;
	}
	
	g_camera.rot.y += fDiff * 0.01f;

	//PI����͂ݏo�Ă����ꍇ�̕␳
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y += -D3DX_PI * 2;
	}
}

//���_���W��n��
D3DXVECTOR3 GetCameraPosV(void)
{
	return g_camera.posV;
}

//�����_���W��n��
D3DXVECTOR3 GetCameraPosR(void)
{
	return g_camera.posR;
}

//������x�N�g����n��
D3DXVECTOR3 GetCameraVecU(void)
{
	return g_camera.vecU;
}

//������n��
D3DXVECTOR3 GetCamerarot(void)
{
	return g_camera.rot;
}

//�J�����̈ړ��ʂ�n��
D3DXVECTOR3 GetCameramove(void)
{
	return g_camera.move;
}

//���_�E�����_�X�C�b�`��n��
bool GetVRswitch(void)
{
	return g_bVR;
}

//��_�Ԃ̋�����n��
float GetVRDis(void)
{
	return g_camera.fDis;
}
