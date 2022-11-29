//============================
//
//�n�ʂ̏���(meshCylinder.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "meshcylinder.h"

//�}�N����`
#define MESHCYLINDER_MAX (1)			//�V�����_�[�̐�
#define MESHCYLWALL_MAX (8)				//�ǂ̐�
#define MESHCYLINDER_SIZEX (400.0f)		//���a�̒���
#define MESHCYLINDER_SIZEY (200.0f)		//���a�̒���
#define MESHCYLINDER_HEIGHT (100.0f)	//�c�̒���
#define MESHCYLINDER_SIZE (100.0f)		//�V�����_�[�̔��a
#define MESHCYLINDER_VTXMAX (18)		//���_��
#define MESHCYLINDER_BLOCK_X (2)		//�u���b�N��x
#define MESHCYLINDER_BLOCK_Y (2)		//�u���b�N��y
#define MESHCYLINDER_VTX_X (9)			//���_��x
#define MESHCYLINDER_VTX_Y (2)			//���_��y
#define MESHCYLINDER_IDX (18)			//�C���f�b�N�X��
#define MESHCYLINDER_PLGN (12)			//�|���S����

//���b�V���E�H�[���̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//���W
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;
}Meshcylinder;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Meshcylinder g_posMeshCylinder[MESHCYLINDER_MAX];		//���S���W

//int g_nVtx = (MESHCYLINDER_BLOCK_X + 1) * (MESHCYLINDER_BLOCK_Z + 1);										//���_��
//int g_nIdx = (1 + MESHCYLINDER_BLOCK_X * 2) * MESHCYLINDER_BLOCK_Z + (MESHCYLINDER_BLOCK_Z - 1) * 4;		//�C���f�b�N�X��
//int g_nPlgn = (MESHCYLINDER_BLOCK_X * MESHCYLINDER_BLOCK_Z * 2) + (MESHCYLINDER_BLOCK_Z - 1) * 4;			//�|���S����


//�n�ʂ̏���������
void InitMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\murabitoUV012.png",				//�e�N�X�`���̃t�@�C����
		&g_pTextureMeshCylinder);

	g_posMeshCylinder[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������
	g_posMeshCylinder[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������
	g_posMeshCylinder[0].bUse = true;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHCYLINDER_VTX_X * MESHCYLINDER_VTX_Y,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHCYLINDER_IDX * MESHCYLINDER_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//float fDevineX = MESHCYLINDER_SIZEX / MESHCYLINDER_BLOCK_X;
	//float fDevineY = MESHCYLINDER_SIZEY / MESHCYLINDER_BLOCK_Y;

	//for (int nCntPos = 0; nCntPos < MESHCYLINDER_MAX; nCntPos++)
	//{
	//	for (int nCntPos = 0; nCntPos < 2; nCntPos++)
	//	{
	//		pVtx[nCntPos].pos = D3DXVECTOR3(MESHCYLINDER_SIZEX, 0.0f, fDevineY);		//(x,y,z)
	//		fDevineY += fDevineY;
	//	}
	//}

	float frot = D3DX_PI / MESHCYLWALL_MAX;

	for (int nCntInit = 0; nCntInit < MESHCYLINDER_MAX; nCntInit++)
	{
		//g_posMeshCylinder[0].x + sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE;	//(x)

		//���_���W�̐ݒ�(x,y,z)
		//��
		pVtx[0].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[1].pos = D3DXVECTOR3( sinf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[2].pos = D3DXVECTOR3( sinf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[3].pos = D3DXVECTOR3( sinf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[4].pos = D3DXVECTOR3( sinf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[5].pos = D3DXVECTOR3( sinf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[6].pos = D3DXVECTOR3( sinf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[7].pos = D3DXVECTOR3( sinf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[8].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		//��
		pVtx[9].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[10].pos = D3DXVECTOR3( sinf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[11].pos = D3DXVECTOR3( sinf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[12].pos = D3DXVECTOR3( sinf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[13].pos = D3DXVECTOR3( sinf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[14].pos = D3DXVECTOR3( sinf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[15].pos = D3DXVECTOR3( sinf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[16].pos = D3DXVECTOR3( sinf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[17].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		//�@���x�N�g���̐ݒ�
		//���_�J���[�̐ݒ�
		for (int nCntcol = 0; nCntcol < MESHCYLINDER_VTXMAX; nCntcol++)
		{
			//D3DXVECTOR3 pos = {};
			//pos -= pVtx[nCntcol].pos;
			pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//(x,y,z)

			D3DXVec3Normalize(&pVtx[nCntcol].nor, &pVtx[nCntcol].nor);		//�x�N�g���𐳋K������(1.0f�ɂ���)

			pVtx[nCntcol].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)

		//�e�N�X�`�����W�̐ݒ�
		pVtx[ 0].tex = D3DXVECTOR2(0.0f,	 0.0f);
		pVtx[ 1].tex = D3DXVECTOR2(0.125f,	 0.0f);
		pVtx[ 2].tex = D3DXVECTOR2(0.25f,	 0.0f);
		pVtx[ 3].tex = D3DXVECTOR2(0.375f,	 0.0f);
		pVtx[ 4].tex = D3DXVECTOR2(0.5f,	 0.0f);
		pVtx[ 5].tex = D3DXVECTOR2(0.625f,	 0.0f);
		pVtx[ 6].tex = D3DXVECTOR2(0.75f,	 0.0f);
		pVtx[ 7].tex = D3DXVECTOR2(0.875f,	 0.0f);
		pVtx[ 8].tex = D3DXVECTOR2(1.0f,	 0.0f);

		pVtx[9 ].tex = D3DXVECTOR2(0.0f,	 1.0f);
		pVtx[10].tex = D3DXVECTOR2(0.125f,	 1.0f);
		pVtx[11].tex = D3DXVECTOR2(0.25f,	 1.0f);
		pVtx[12].tex = D3DXVECTOR2(0.375f,	 1.0f);
		pVtx[13].tex = D3DXVECTOR2(0.5f,	 1.0f);
		pVtx[14].tex = D3DXVECTOR2(0.625f,	 1.0f);
		pVtx[15].tex = D3DXVECTOR2(0.75f,	 1.0f);
		pVtx[16].tex = D3DXVECTOR2(0.875f,	 1.0f);
		pVtx[17].tex = D3DXVECTOR2(1.0f,	 1.0f);

		pVtx += MESHCYLINDER_VTXMAX;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;		//�C���f�b�N�X���ւ̃|�C���^

					//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MESHCYLINDER_MAX; nCntIdx++)
	{
		//���_�ԍ��f�[�^�̐ݒ�
		//�O��
		//pIdx[0] = 0;
		//pIdx[1] = 9;
		//pIdx[2] = 1;
		//pIdx[3] = 10;
		//pIdx[4] = 2;
		//pIdx[5] = 11;
		//pIdx[6] = 3;
		//pIdx[7] = 12;
		//pIdx[8] = 4;
		//pIdx[9] = 13;
		//pIdx[10] = 5;
		//pIdx[11] = 14;
		//pIdx[12] = 6;
		//pIdx[13] = 15;
		//pIdx[14] = 7;
		//pIdx[15] = 16;
		//pIdx[16] = 8;
		//pIdx[17] = 17;

		//����
		pIdx[0] = 9;
		pIdx[1] = 0;
		pIdx[2] = 10;
		pIdx[3] = 1;
		pIdx[4] = 11;
		pIdx[5] = 2;
		pIdx[6] = 12;
		pIdx[7] = 3;
		pIdx[8] = 13;
		pIdx[9] = 4;
		pIdx[10] = 14;
		pIdx[11] = 5;
		pIdx[12] = 15;
		pIdx[13] = 6;
		pIdx[14] = 16;
		pIdx[15] = 7;
		pIdx[16] = 17;
		pIdx[17] = 8;

		pIdx += MESHCYLINDER_IDX;
	}
	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCylinder->Unlock();

	//SetMeshCylinder(D3DXVECTOR3(0.0f, 0.0f, MESHCYLINDER_SIZEX), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetMeshCylinder(D3DXVECTOR3(0.0f, 0.0f, -MESHCYLINDER_SIZEX), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetMeshCylinder(D3DXVECTOR3(MESHCYLINDER_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetMeshCylinder(D3DXVECTOR3(-MESHCYLINDER_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
}

//�n�ʂ̏I������
void UninitMeshCylinder(void)
{
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;					//���_�o�b�t�@�̔j��
	}
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;					//�C���f�b�N�X�o�b�t�@�̔j��
	}
}

//�n�ʂ̍X�V����
void UpdateMeshCylinder(void)
{

}

//�n�ʂ̕`�揈��
void DrawMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

	for (int nCntDraw = 0; nCntDraw < MESHCYLINDER_MAX; nCntDraw++)
	{
		if (g_posMeshCylinder[nCntDraw].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_posMeshCylinder[nCntDraw].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_posMeshCylinder[nCntDraw].rot.y, g_posMeshCylinder[nCntDraw].rot.x, g_posMeshCylinder[nCntDraw].rot.z);

			D3DXMatrixMultiply(&g_posMeshCylinder[nCntDraw].mtxWorld, &g_posMeshCylinder[nCntDraw].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_posMeshCylinder[nCntDraw].pos.x, g_posMeshCylinder[nCntDraw].pos.y, g_posMeshCylinder[nCntDraw].pos.z);

			D3DXMatrixMultiply(&g_posMeshCylinder[nCntDraw].mtxWorld, &g_posMeshCylinder[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_posMeshCylinder[nCntDraw].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshCylinder);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				0,
				0,
				18,							//�p�ӂ������_�̐�
				0,
				17);						//�`�悷��v���~�e�B�u�̐�
		}
	}
}

////�ǂ̐ݒu����
//void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nDevine)
//{
//	for (int nCntSet = 0; nCntSet < MESHCYLINDER_MAX; nCntSet++)
//	{
//		if (g_MeshCylinder[nCntSet].bUse == false)
//		{
//			g_MeshCylinder[nCntSet].pos = pos;
//			g_MeshCylinder[nCntSet].rot = rot;
//			g_MeshCylinder[nCntSet].bUse = true;
//			break;
//		}
//	}
//}
