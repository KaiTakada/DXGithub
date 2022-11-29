//============================
//
//�|���S���̏���(wall.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "wall.h"

//�}�N����`
#define WALL_SIZEX (400.0f)
#define WALL_SIZEY (100.0f)
#define MAX_WALL (4)			//�ő�ǔz�u��

//�ǂ̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//�ړ���
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;					//�g�p���Ă��邩
}Wall;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//�e�N�X�`���ւ̃|�C���^ 
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Wall g_wall[MAX_WALL];

//�ǂ̏���������
void InitWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\cat000.jpg",				//�e�N�X�`���̃t�@�C����
		&g_pTextureWall);

	for (int nCntInit = 0;nCntInit < MAX_WALL;nCntInit++)
	{//����������
		g_wall[nCntInit].pos = D3DXVECTOR3(0.0f, 0.0f, WALL_SIZEX);
		g_wall[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wall[nCntInit].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0,0,(void**)&pVtx,0);

	for (int nCntWall = 0;nCntWall < MAX_WALL;nCntWall++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-WALL_SIZEX, WALL_SIZEY * 2, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(WALL_SIZEX, WALL_SIZEY * 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-WALL_SIZEX, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(WALL_SIZEX, 0.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -WALL_SIZEX), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, WALL_SIZEX), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(WALL_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(-WALL_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//�ǂ̏I������
void UninitWall(void)
{
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;					//�o�b�t�@�̔j��
	}
}

//�ǂ̍X�V����
void UpdateWall(void)
{

}

//�ǂ̕`�揈��
void DrawWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntDraw = 0; nCntDraw < MAX_WALL; nCntDraw++)
	{
		if (g_wall[nCntDraw].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_wall[nCntDraw].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_wall[nCntDraw].rot.y, g_wall[nCntDraw].rot.x, g_wall[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_wall[nCntDraw].mtxWorld, &g_wall[nCntDraw].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_wall[nCntDraw].pos.x, g_wall[nCntDraw].pos.y, g_wall[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_wall[nCntDraw].mtxWorld, &g_wall[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_wall[nCntDraw].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(nCntDraw, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(nCntDraw, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				nCntDraw * 4,									//�`�悷��ŏ��̃C���f�b�N�X
				2);												//�`�悷��v���~�e�B�u��
		}
	}
}

//�ǂ̐ݒu����
void SetWall(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	for (int nCntSet = 0;nCntSet < MAX_WALL;nCntSet++)
	{
		if (g_wall[nCntSet].bUse == false)
		{
			g_wall[nCntSet].pos = pos;
			g_wall[nCntSet].rot = rot;
			g_wall[nCntSet].bUse = true;
			break;
		}
	}
}
