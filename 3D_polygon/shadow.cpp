//============================
//
//�e�̏���(shadow.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "shadow.h"

//�}�N����`
#define SHADOW_SIZE (10.0f)		//�e�̔��a�̏����l
#define SHADOW_MAX (256)		//�e�̐�

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//���ݎg���Ă���܂��H
}Shadow;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Shadow g_aShadow[SHADOW_MAX];						//�e�̏��

//�e�̏���������
void InitShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",				//�e�N�X�`���̃t�@�C����
		&g_pTextureShadow);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * SHADOW_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntInit = 0; nCntInit < SHADOW_MAX; nCntInit++)
	{
		g_aShadow[nCntInit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������
		g_aShadow[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������
		g_aShadow[nCntInit].bUse = false;	//����������

	//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-SHADOW_SIZE, 0.0f, SHADOW_SIZE);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE);
		pVtx[2].pos = D3DXVECTOR3(-SHADOW_SIZE, 0.0f, -SHADOW_SIZE);
		pVtx[3].pos = D3DXVECTOR3(SHADOW_SIZE, 0.0f, -SHADOW_SIZE);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//�e�̏I������
void UninitShadow(void)
{
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;					//�o�b�t�@�̔j��
	}
}

//�e�̍X�V����
void UpdateShadow(void)
{

}

//�e�̕`�揈��
void DrawShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntDraw = 0; nCntDraw < SHADOW_MAX; nCntDraw++)
	{
		if (g_aShadow[nCntDraw].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntDraw].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aShadow[nCntDraw].rot.y, g_aShadow[nCntDraw].rot.x, g_aShadow[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntDraw].mtxWorld, &g_aShadow[nCntDraw].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aShadow[nCntDraw].pos.x, g_aShadow[nCntDraw].pos.y, g_aShadow[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntDraw].mtxWorld, &g_aShadow[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntDraw].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�e�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				nCntDraw * 4,									//�`�悷��ŏ��̃C���f�b�N�X
				2);												//�`�悷��v���~�e�B�u��
		}
	}

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1][256];
	int nCnterShadow = 0;
	LPD3DXFONT Font = GetFont();
	for (int nCnt = 0; nCnt < SHADOW_MAX; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == true)
		{
			nCnterShadow++;
		}
	}

	//������ɑ��
	sprintf(&aStr[0][0], "\n\n\n\n\n\n\n\n\n\n\n�e�̐��F%d", nCnterShadow);

	//�e�L�X�g�`��
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//�ݒ菈��
int SetShadow(void)
{
	for (int nCntSet = 0;nCntSet < SHADOW_MAX;nCntSet++)
	{
		if (g_aShadow[nCntSet].bUse == false)
		{
 			g_aShadow[nCntSet].bUse = true;
			return nCntSet;
		}
	}

	return -1;
}

//�ʒu�ݒ菈��
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos.x = pos.x;
	g_aShadow[nIdxShadow].pos.z = pos.z;
}

//�e�̎g�p��ԕύX
void SetShadowEnable(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}
