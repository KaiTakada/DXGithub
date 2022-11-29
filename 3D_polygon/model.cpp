//============================
//
//���f���̏���(model.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "model.h"
#include "camera.h"
#include "bullet.h"
#include "shadow.h"
#include "enemy.h"
#include "input.h"

//�}�N����`
#define MODEL_SPEED (0.25f)						//�v���C���[�̈ړ��X�s�[�h
#define ADULTWALL (1600.0f - 15.0f)				//��l�̕ǒ��"
#define STALK_COUNT (100)						//��荞�݃J�E���^�[

//���f���̍\����
typedef struct
{
	D3DXVECTOR3 pos;							//���݂̈ʒu
	D3DXVECTOR3 posOld;							//�O��̈ʒu
	D3DXVECTOR3 posDest;						//�\��̈ʒu
	D3DXVECTOR3 move;							//�ړ���
	D3DXVECTOR3 rot;							//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
}Model;

//�O���[�o���ϐ��錾
LPD3DXMESH g_pMeshModel = NULL;					//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;			//�}�e���A���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureModel[5] = {};		//�e�N�X�`���ւ̃|�C���^
DWORD g_dwNumMatModel = 0;						//�}�e���A���̐�
int g_nIdxShadowPlayer = -1;					//�Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
Model g_model;									//���f���̏��
int g_nStalkCounter;							//�J�����̉�荞�݃J�E���g
bool g_bStalk;									//�J�����̉�荞�݂�ON/OFF

//�v���C���[�̏���������
void InitModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_model.pos = D3DXVECTOR3(0.0f, 10.0f, 100.0f);			//����������
	g_model.posOld = g_model.pos;							//����������
	g_model.posDest = D3DXVECTOR3(0.0f, 10.0f, 100.0f);		//����������
	g_model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������
	g_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������

	g_bStalk = false;

//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\head.x",
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_pBuffMatModel,
						NULL,
						&g_dwNumMatModel,
						&g_pMeshModel);

	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0;nCntMat < (int)g_dwNumMatModel;nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,				//�e�N�X�`���̃t�@�C����
				&g_pTextureModel[nCntMat]);
		}
	}

	//�e��ݒ�
	g_nIdxShadowPlayer = SetShadow();
}

//�v���C���[�̏I������
void UninitModel(void)
{
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;					//�e�N�X�`���j��
	}
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;					//�o�b�t�@�̔j��
	}
}

//�v���C���[�̍X�V����
void UpdateModel(void)
{
	float fRotMove = g_model.rot.y;	//���݂̌���
	float fRotDest = g_model.rot.y;	//�ڕW�̌���
	float fRotDiff = 0.0f;			//����
	g_model.posOld = g_model.pos;							//�X�V����

	if (GetKeyboardPress(DIK_S) == true)
	{
		D3DXVECTOR3 CameraRot = GetCamerarot();

		if (GetKeyboardPress(DIK_A) == true)
		{//�ォ���L�[���������Ƃ�
			g_model.move.x += sinf(CameraRot.y + -0.75f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + -0.75f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 0.25f * D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//�ォ�E�L�[���������Ƃ�
			g_model.move.x += sinf(CameraRot.y + 0.75f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 0.75f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + -0.25f * D3DX_PI;
		}
		else
		{//��L�[�݂̂��������Ƃ�
			g_model.move.x += sinf(CameraRot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 0.0f * D3DX_PI;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		D3DXVECTOR3 CameraRot = GetCamerarot();

		if (GetKeyboardPress(DIK_A) == true)
		{//�������L�[���������Ƃ�
			g_model.move.x += sinf(CameraRot.y + -0.25f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + -0.25f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 0.75f * D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//�����E�L�[���������Ƃ�
			g_model.move.x += sinf(CameraRot.y + 0.25f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 0.25f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + -0.75f * D3DX_PI;
		}
		else
		{//���L�[�݂̂��������Ƃ�
			g_model.move.x += sinf(CameraRot.y + 0.0f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 0.0f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 1.0f * D3DX_PI;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{//���L�[�݂̂��������Ƃ�
		D3DXVECTOR3 CameraRot = GetCamerarot();

		g_model.move.x += sinf(CameraRot.y + -0.5f * D3DX_PI) * MODEL_SPEED;
		g_model.move.z += cosf(CameraRot.y + -0.5f * D3DX_PI) * MODEL_SPEED;
		fRotDest = CameraRot.y + 0.5f * D3DX_PI;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//�E�L�[�݂̂��������Ƃ�
		D3DXVECTOR3 CameraRot = GetCamerarot();

		g_model.move.x += sinf(CameraRot.y + 0.5f * D3DX_PI) * MODEL_SPEED;
		g_model.move.z += cosf(CameraRot.y + 0.5f * D3DX_PI) * MODEL_SPEED;
		fRotDest = CameraRot.y + -0.5f * D3DX_PI;
	}

	if (GetKeyboardPress(DIK_SPACE) == true)
	{//[ - ]�L�[�������ꂽ
		SetBullet(g_model.pos, g_model.rot,BULLETTYPE_PLAYER);
	}

	if (GetKeyboardTrigger(DIK_F6) == true)
	{//F6�L�[�������ꂽ
		g_bStalk = g_bStalk ? false : true;
		g_nStalkCounter = 0;
	}

	fRotDiff = fRotDest - fRotMove;

	//�p�x�̒l���C������
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}
	else if (fRotDiff > D3DX_PI)
	{
		fRotDiff += -D3DX_PI * 2;
	}

	g_model.rot.y += fRotDiff * 0.1f;		//�ړ�����(�p�x)�̕␳

	//�p�x�̒l���C������
	if (g_model.rot.y < -D3DX_PI)
	{
		g_model.rot.y += D3DX_PI * 2;
	}
	else if (g_model.rot.y > D3DX_PI)
	{
		g_model.rot.y += -D3DX_PI * 2;
	}

	//�ʒu���X�V
	g_model.pos += g_model.move;
	//g_model.posDest = g_model.pos + g_model.move;
	//g_model.posDest.x += sinf(g_model.rot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//x
	//g_model.posDest.z += cosf(g_model.rot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//y

	//�ړ��ʂ��X�V(����������)
	g_model.move.x += (0 - g_model.move.x) * 0.1f;
	g_model.move.z += (0 - g_model.move.z) * 0.1f;

	if (g_bStalk == true)
	{
		if (g_model.move.x < 1.0f &&
			g_model.move.y < 1.0f &&
			g_model.move.z < 1.0f &&
			g_model.move.x > -1.0f &&
			g_model.move.y > -1.0f &&
			g_model.move.z > -1.0f)
		{//�������Ȃ��Ȃ�����
			g_nStalkCounter++;

			if (g_nStalkCounter >= STALK_COUNT)
			{
				CameraStoke();
			}
		}
		else
		{
			g_nStalkCounter = 0;
		}
	}

	CollisionEnemy(&g_model.pos,g_model.posOld);


	if (g_model.pos.x > ADULTWALL)
	{//��l�̕ǂɓ���������
		g_model.pos.x = ADULTWALL;			
	}
	if (g_model.pos.x < -ADULTWALL)
	{//��l�̕ǂɓ���������
		g_model.pos.x = -ADULTWALL;			
	}
	if (g_model.pos.z > ADULTWALL)
	{//��l�̕ǂɓ���������
		g_model.pos.z = ADULTWALL;			
	}
	if (g_model.pos.z < -ADULTWALL)
	{//��l�̕ǂɓ���������
		g_model.pos.z = -ADULTWALL;
	}

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nIdxShadowPlayer, g_model.pos);
}
	

//�v���C���[�̕`�揈��
void DrawModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_model.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_model.rot.y, g_model.rot.x, g_model.rot.z);

	D3DXMatrixMultiply(&g_model.mtxWorld, &g_model.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_model.pos.x, g_model.pos.y, g_model.pos.z);

	D3DXMatrixMultiply(&g_model.mtxWorld, &g_model.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_model.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureModel[nCntMat]);

		//���f��(�p�[�c)�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	char sStalk[5];

	switch (g_bStalk)
	{
	case true:
		strcpy(sStalk, "ON");
		break;

	case false:
		strcpy(sStalk, "OFF");
		break;
	}

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[5][256];
	LPD3DXFONT Font = GetFont();

	//������ɑ��
	sprintf(&aStr[0][0], "���f���̈ʒu�F%.2f %.2f %.2f", g_model.pos.x, g_model.pos.y, g_model.pos.z);
	sprintf(&aStr[1][0], "\n���f���̌����F%.2f %.2f %.2f", g_model.rot.x, g_model.rot.y, g_model.rot.z);
	sprintf(&aStr[2][0], "\n\n���f���̈ړ���:%.2f %.2f %.2f", g_model.move.x, g_model.move.y, g_model.move.z);
	sprintf(&aStr[3][0], "\n\n\n���f���̑���<��/��/��/��>");
	sprintf(&aStr[4][0], "\n\n\n\n�J�����̉�荞��<F6>(%s)",sStalk);

	//�e�L�X�g�`��
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[1][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[2][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[3][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[4][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//���f���̍��W��n��
D3DXVECTOR3 GetModelpos(void)
{
	return g_model.pos;
}

//���f���̗\����W��n��
D3DXVECTOR3 GetModelposDest(void)
{
	return g_model.posDest;
}

//���f���̗\����W��n��
D3DXVECTOR3 GetModelRot(void)
{
	return g_model.rot;
}
