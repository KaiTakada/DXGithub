//============================
//
//�r���{�[�h�̏���(bullet.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "bullet.h"
#include "shadow.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "meshwall.h"

//�}�N����`
#define BULLET_SPEED (1.0f)		//�e�̃X�s�[�h
#define BULLET_SIZE (5.0f)		//�e�̔��a
#define BULLET_MAX (256)		//�e�̍ő吔

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//���݂̍��W
	D3DXVECTOR3 posOld;		//�O��̍��W
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 vecMove;	//�ړ��x�N�g��
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nLife;				//����
	BULLETTYPE type;		//�e�̎��
	bool bUse;				//�g�p�����ۂ�
	int nIdxShadow = -1;	//�Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
}Bullet;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[BULLET_MAX];

//�e�̏���������
void InitBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",				//�e�N�X�`���̃t�@�C����
		&g_pTextureBullet);

	for (int nCntGro = 0;nCntGro < BULLET_MAX;nCntGro++)
	{//����������
		g_aBullet[nCntGro].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntGro].posOld = g_aBullet[nCntGro].pos;
		g_aBullet[nCntGro].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntGro].vecMove = g_aBullet[nCntGro].rot;
		g_aBullet[nCntGro].nLife = 300;
		g_aBullet[nCntGro].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntGro].bUse = false;
	}
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BULLET_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntInit = 0;nCntInit < BULLET_MAX;nCntInit++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE, BULLET_SIZE, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE, -BULLET_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE, -BULLET_SIZE, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//�e�̏I������
void UninitBullet(void)
{
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;					//�o�b�t�@�̔j��
	}
}

//�e�̍X�V����
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				SetEffect(g_aBullet[nCntBullet].pos,D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR(0.3f, 0.4f, 1.0f, 1.0f), 120, 10.0f);
				//CollisionEnemy(pBullet);
			}

			//���_���W�̍X�V
			g_aBullet[nCntBullet].pos.x += sinf(g_aBullet[nCntBullet].rot.y + 1.0f * D3DX_PI) * BULLET_SPEED;		//x
			g_aBullet[nCntBullet].pos.z += cosf(g_aBullet[nCntBullet].rot.y + 1.0f * D3DX_PI) * BULLET_SPEED;		//y

			//�ǂƂ̓����蔻��
			CollisionWall(nCntBullet);

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			g_aBullet[nCntBullet].nLife--;
			//�����̃J�E���g�_�E��
			if (g_aBullet[nCntBullet].nLife <= 0)
			{//�������߂�����
				g_aBullet[nCntBullet].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			}
			//else if (g_aBullet[nCntBullet].pos.x >= 400.0f || g_aBullet[nCntBullet].pos.x <= -400.0f ||
			//	g_aBullet[nCntBullet].pos.z >= 400.0f || g_aBullet[nCntBullet].pos.z <= -400.0f)
			//{//��l�̕ǂɓ���������
			//	g_aBullet[nCntBullet].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			//}

			if (g_aBullet[nCntBullet].bUse == false)
			{//���񂾂�
				SetShadowEnable(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos);
				SetParticle(g_aBullet[nCntBullet].pos, 0);
			}
		}
	}
}

//�e�̕`�揈��
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;									//�r���[�}�g���b�N�X�擾�p

	for (int nCntDraw = 0; nCntDraw < BULLET_MAX; nCntDraw++)
	{
		if (g_aBullet[nCntDraw].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntDraw].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCntDraw].mtxWorld, NULL, &mtxView);
			g_aBullet[nCntDraw].mtxWorld._41 = 0.0f;
			g_aBullet[nCntDraw].mtxWorld._42 = 0.0f;
			g_aBullet[nCntDraw].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntDraw].pos.x, g_aBullet[nCntDraw].pos.y, g_aBullet[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntDraw].mtxWorld, &g_aBullet[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntDraw].mtxWorld);

			//Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				0,												//�`�悷��ŏ��̃C���f�b�N�X
				2);												//�`�悷��v���~�e�B�u��

			//Z�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
		}
	}

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1][256];
	int nCnterBullet = 0;
	LPD3DXFONT Font = GetFont();
	for (int nCnt = 0;nCnt < BULLET_MAX;nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			nCnterBullet++;
		}
	}

	//������ɑ��
	sprintf(&aStr[0][0], "\n\n\n\n\n\n\n\n\n�e�̐��F%d", nCnterBullet);

	//�e�L�X�g�`��
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================
//�e�̐ݒ菈��
//=============================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;				//���W����
			g_aBullet[nCntBullet].rot = rot;				//��������
			g_aBullet[nCntBullet].type = type;				//��ނ���
			g_aBullet[nCntBullet].bUse = true;				//�g�p���Ă����Ԃɂ���

			switch (g_aBullet[nCntBullet].type)
			{
			case BULLETTYPE_PLAYER:
				g_aBullet[nCntBullet].nLife = 300;
				break;
			}
			//�e��ݒ�
			g_aBullet[nCntBullet].nIdxShadow = SetShadow();
			break;
		}
	}
}

//�ǂƂ̓����蔻��
void CollisionWall(int nCntBullet)
{
	//�}�N��
#define WALL_MAX (4)

////�ǂ̏������炤
//Meshwall *pMeshWall = &GetMeshWall();

	D3DXVECTOR3 Wallpos0;
	D3DXVECTOR3 Wallpos1;
	D3DXVECTOR3 vecTopos;
	D3DXVECTOR3 vecLine;

	for (int nCntcpy = 0; nCntcpy < WALL_MAX; nCntcpy++)
	{
		switch (nCntcpy)
		{
		case 0:
			Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
			Wallpos1 = D3DXVECTOR3(-400.0f,0.0f,400.0f);
			
			vecLine = Wallpos1 - Wallpos0;
			vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

			if ((vecLine.z * vecTopos.x)
				- (vecLine.x * vecTopos.z) >= 0.0f)
			{
				g_aBullet[nCntBullet].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			}
			break;

		//case 1:
		//	Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
		//	Wallpos1 = D3DXVECTOR3(-400.0f, 0.0f, 400.0f);

		//	vecLine = Wallpos1 - Wallpos0;
		//	vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

		//	if ((vecLine.z * vecTopos.x)
		//		- (vecLine.x * vecTopos.z) >= 0.0f)
		//	{
		//		g_aBullet[nCntBullet].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
		//	}
		//	break;

		//case 2:
		//	Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
		//	Wallpos1 = D3DXVECTOR3(-400.0f, 0.0f, 400.0f);

		//	vecLine = Wallpos1 - Wallpos0;
		//	vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

		//	if ((vecLine.z * vecTopos.x)
		//		- (vecLine.x * vecTopos.z) >= 0.0f)
		//	{
		//		g_aBullet[nCntBullet].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
		//	}
		//	break;
		//
		//case 3:
		//	Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
		//	Wallpos1 = D3DXVECTOR3(-400.0f, 0.0f, 400.0f);

		//	vecLine = Wallpos1 - Wallpos0;
		//	vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

		//	if ((vecLine.z * vecTopos.x)
		//		- (vecLine.x * vecTopos.z) >= 0.0f)
		//	{
		//		g_aBullet[nCntBullet].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
		//	}
		//	break;
		}

	}
}
