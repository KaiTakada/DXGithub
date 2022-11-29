//============================
//
//�n�ʂ̏���(meshWall.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "meshwall.h"

//�}�N����`
#define MESHWALL_MAX (4)			//�ǂ̐�
#define MESHWALL_SIZEX (1600.0f)	//���a�̒���
#define MESHWALL_SIZEY (200.0f)		//���a�̒���
#define MESHWALL_VTXMAX (9)			//���_��
#define MESHWALL_BLOCK_X (2)		//�u���b�N��x
#define MESHWALL_BLOCK_Y (2)		//�u���b�N��y
#define MESHWALL_VTX_X (3)			//���_��x
#define MESHWALL_VTX_Y (3)			//���_��y
#define MESHWALL_IDX (14)			//�C���f�b�N�X��
#define MESHWALL_PLGN (12)			//�|���S����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Meshwall g_MeshWall[MESHWALL_MAX];					//���b�V���E�H�[���̏��
//int g_nVtx = (MESHWALL_BLOCK_X + 1) * (MESHWALL_BLOCK_Z + 1);									//���_��
//int g_nIdx = (1 + MESHWALL_BLOCK_X * 2) * MESHWALL_BLOCK_Z + (MESHWALL_BLOCK_Z - 1) * 4;		//�C���f�b�N�X��
//int g_nPlgn = (MESHWALL_BLOCK_X * MESHWALL_BLOCK_Z * 2) + (MESHWALL_BLOCK_Z - 1) * 4;			//�|���S����


//�n�ʂ̏���������
void InitMeshWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall001.jpg",				//�e�N�X�`���̃t�@�C����
		&g_pTextureMeshWall);

	for (int nCntGrb = 0; nCntGrb < MESHWALL_MAX; nCntGrb++)
	{
		g_MeshWall[nCntGrb].pos = D3DXVECTOR3(0.0f, MESHWALL_SIZEY, MESHWALL_SIZEY * nCntGrb);	//����������
		g_MeshWall[nCntGrb].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������
		g_MeshWall[nCntGrb].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������
		g_MeshWall[nCntGrb].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWALL_VTX_X * MESHWALL_VTX_Y * MESHWALL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHWALL_IDX * MESHWALL_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	//float fDevineX = MESHWALL_SIZEX / MESHWALL_BLOCK_X;
	//float fDevineY = MESHWALL_SIZEY / MESHWALL_BLOCK_Y;

	//for (int nCntPos = 0; nCntPos < MESHWALL_MAX; nCntPos++)
	//{
	//	for (int nCntPos = 0; nCntPos < 2; nCntPos++)
	//	{
	//		pVtx[nCntPos].pos = D3DXVECTOR3(MESHWALL_SIZEX, 0.0f, fDevineY);		//(x,y,z)
	//		fDevineY += fDevineY;
	//	}
	//}

	for (int nCntInit = 0; nCntInit < MESHWALL_MAX; nCntInit++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-MESHWALL_SIZEX,	MESHWALL_SIZEY, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(0.0f,				MESHWALL_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(MESHWALL_SIZEX,	MESHWALL_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(-MESHWALL_SIZEX,	MESHWALL_SIZEY * 0.5, 0.0f);
		pVtx[4].pos = D3DXVECTOR3(0.0f,				MESHWALL_SIZEY * 0.5, 0.0f);		//(x,y,z)
		pVtx[5].pos = D3DXVECTOR3(MESHWALL_SIZEX,	MESHWALL_SIZEY * 0.5, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(-MESHWALL_SIZEX,	0.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(0.0f,				0.0f, 0.0f);
		pVtx[8].pos = D3DXVECTOR3(MESHWALL_SIZEX,	0.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		//���_�J���[�̐ݒ�
		for (int nCntcol = 0; nCntcol < MESHWALL_VTXMAX; nCntcol++)
		{
			pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntcol].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)

		//pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	
		pVtx += MESHWALL_VTXMAX;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshWall->Unlock();

	WORD *pIdx;		//�C���f�b�N�X���ւ̃|�C���^

					//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MESHWALL_MAX; nCntIdx++)
	{
		//���_�ԍ��f�[�^�̐ݒ�
		pIdx[0] =  3;
		pIdx[1] =  0;
		pIdx[2] =  4;
		pIdx[3] =  1;
		pIdx[4] =  5;
		pIdx[5] =  2;
		pIdx[6] =  2;
		pIdx[7] =  6;
		pIdx[8] =  6;
		pIdx[9] =  3;
		pIdx[10] = 7;
		pIdx[11] = 4;
		pIdx[12] = 8;
		pIdx[13] = 5;
	
		pIdx += MESHWALL_IDX;
	}
	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshWall->Unlock();

	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, MESHWALL_SIZEX), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -MESHWALL_SIZEX), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetMeshWall(D3DXVECTOR3(MESHWALL_SIZEX, 0.0f,  0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	SetMeshWall(D3DXVECTOR3(-MESHWALL_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
}

//�n�ʂ̏I������
void UninitMeshWall(void)
{
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;					//���_�o�b�t�@�̔j��
	}
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;					//�C���f�b�N�X�o�b�t�@�̔j��
	}
}

//�n�ʂ̍X�V����
void UpdateMeshWall(void)
{

}

//�n�ʂ̕`�揈��
void DrawMeshWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

	for (int nCntDraw = 0; nCntDraw < MESHWALL_MAX; nCntDraw++)
	{
		if (g_MeshWall[nCntDraw].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_MeshWall[nCntDraw].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_MeshWall[nCntDraw].rot.y, g_MeshWall[nCntDraw].rot.x, g_MeshWall[nCntDraw].rot.z);

			D3DXMatrixMultiply(&g_MeshWall[nCntDraw].mtxWorld, &g_MeshWall[nCntDraw].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_MeshWall[nCntDraw].pos.x, g_MeshWall[nCntDraw].pos.y, g_MeshWall[nCntDraw].pos.z);

			D3DXMatrixMultiply(&g_MeshWall[nCntDraw].mtxWorld, &g_MeshWall[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshWall[nCntDraw].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				0,
				0,
				9,							//�p�ӂ������_�̐�
				0,
				12);						//�`�悷��v���~�e�B�u�̐�
		}
	}
}

//�ǂ̐ݒu����
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntSet = 0; nCntSet < MESHWALL_MAX; nCntSet++)
	{
		if (g_MeshWall[nCntSet].bUse == false)
		{
			g_MeshWall[nCntSet].pos = pos;
			g_MeshWall[nCntSet].rot = rot;
			g_MeshWall[nCntSet].bUse = true;
			break;
		}
	}
}

//�ǂ̏���n��
Meshwall GetMeshWall(void)
{
	return g_MeshWall[0];
}
