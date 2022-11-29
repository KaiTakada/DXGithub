//===========================================
//
//�G�t�F�N�g�̏��(particle.cpp)
//Author:Kai Takada
//
//===========================================
#include "main.h"
#include "bullet.h"
#include "effect.h"
#include "stdio.h"
#include "input.h"

//�}�N����`
#define EFFECT_MAX (32768)		//�G�t�F�N�g�̍ő吔
#define EFF_RADIUS (10.0f)		//�G�t�F�N�g�̔��a
#define EFFECT_SPEED (0.1f)		//�G�t�F�N�g�̃X�s�[�h
#define EFF_COLOR (0.1f)		//�G�t�F�N�g�̐F

//�G�t�F�N�g�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	D3DXCOLOR col;				//�F
	float fRadius;				//���a(�傫��)
	int nMaxLife;				//���a�~����
	int nLife;					//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;					//�g�p���Ă��邩�ǂ���
}Effect;

//�v���g�^�C�v�錾

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			//�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[EFFECT_MAX];						//�G�t�F�N�g�̏��

//==============================
//�G�t�F�N�g�̏���������
//==============================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\Texture\\effect000.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureEffect);

	//�G�t�F�N�g�̏�������������
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 100;
		g_aEffect[nCntEffect].nLife = 120;
		g_aEffect[nCntEffect].nMaxLife = 120;
		g_aEffect[nCntEffect].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_3D *pVtx;		//

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-EFF_RADIUS, EFF_RADIUS, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(EFF_RADIUS, EFF_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EFF_RADIUS, -EFF_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(EFF_RADIUS, -EFF_RADIUS, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//�G�t�F�N�g�̏I������
void UninitEffect(void)
{
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;					//�o�b�t�@�̔j��
	}
}

//�G�t�F�N�g�̍X�V����
void UpdateEffect(void)
{
	int nCntEffect;
	Effect *pEffect = &g_aEffect[0];
	
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)
		{//�G�t�F�N�g���g�p����Ă���

			if ((pEffect->nLife % 8) == 0)
			{//���_���W�̍X�V(�|���S���T�C�Y�̍X�V)
				pEffect->fRadius *= ((float)pEffect->nLife / (float)pEffect->nMaxLife);
			}

			//���W�̈ړ�
			pEffect->pos.x += pEffect->rot.x * EFFECT_SPEED;
			pEffect->pos.y += pEffect->rot.y * EFFECT_SPEED;
			pEffect->pos.z += pEffect->rot.z * EFFECT_SPEED;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-pEffect->fRadius, pEffect->fRadius, 0.0f);		//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(pEffect->fRadius, pEffect->fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-pEffect->fRadius, -pEffect->fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->fRadius, -pEffect->fRadius, 0.0f);

			//���_�J���[�̍X�V(�|���S���̐F�̍X�V)
			pEffect->col.r -= EFF_COLOR * 0.01f;
			pEffect->col.g -= EFF_COLOR * 0.01f;
			pEffect->col.b -= EFF_COLOR * 0.01f;
			pEffect->col.a -= EFF_COLOR * 0.01f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			//�����̃J�E���g�_�E��
			if (pEffect->nLife <= 0)
			{//�������߂�����
				pEffect->bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			}
			else
			{//�������
				pEffect->nLife--;
			}
		}
		pVtx += 4;								//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//�G�t�F�N�g�̕`�揈��
void DrawEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;									//�r���[�}�g���b�N�X�擾�p

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntDraw = 0; nCntDraw < EFFECT_MAX; nCntDraw++)
	{
		if (g_aEffect[nCntDraw].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntDraw].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCntDraw].mtxWorld, NULL, &mtxView);
			g_aEffect[nCntDraw].mtxWorld._41 = 0.0f;
			g_aEffect[nCntDraw].mtxWorld._42 = 0.0f;
			g_aEffect[nCntDraw].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntDraw].pos.x, g_aEffect[nCntDraw].pos.y, g_aEffect[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntDraw].mtxWorld, &g_aEffect[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntDraw].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				nCntDraw * 4,									//�`�悷��ŏ��̃C���f�b�N�X
				2);												//�`�悷��v���~�e�B�u��
		}
	}
	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1][256];
	int nCnterEffect = 0;
	LPD3DXFONT Font = GetFont();
	for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == true)
		{
			nCnterEffect++;
		}
	}

	//������ɑ��
	sprintf(&aStr[0][0], "\n\n\n\n\n\n\n\n\n\n�G�t�F�N�g�̐��F%d", nCnterEffect);

	//�e�L�X�g�`��
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//�G�t�F�N�g�̐ݒ菈��
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius)
{
	int nCntEffect;

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nMaxLife = nLife;
			g_aEffect[nCntEffect].bUse = true;				//�g�p���Ă����Ԃɂ���

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);		//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
