//============================
//
//�n�ʂ̏���(meshfield.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "meshfield.h"

//�}�N����`
#define MESHFIELD_SIZE (1600.0f)	//���a�̒���
#define MESHFIELD_MAX (9)			//���_��
#define MESHFIELD_BLOCK_X (2)		//�u���b�N��x
#define MESHFIELD_BLOCK_Z (2)		//�u���b�N��z
#define MESHFIELD_VTX_X (3)			//���_��x
#define MESHFIELD_VTX_Z (3)			//���_��z
#define MESHFIELD_IDX (14)			//�C���f�b�N�X��
#define MESHFIELD_PLGN (12)			//�|���S����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshfield;							//�ʒu
D3DXVECTOR3 g_rotMeshfield;							//�ړ���
D3DXMATRIX g_mtxWorldMeshfield;						//���[���h�}�g���b�N�X
int g_nVtx = (MESHFIELD_BLOCK_X + 1) * (MESHFIELD_BLOCK_Z + 1);									//���_��
int g_nIdx = (1 + MESHFIELD_BLOCK_X * 2) * MESHFIELD_BLOCK_Z + (MESHFIELD_BLOCK_Z - 1) * 4;		//�C���f�b�N�X��
int g_nPlgn = (MESHFIELD_BLOCK_X * MESHFIELD_BLOCK_Z * 2) + (MESHFIELD_BLOCK_Z - 1) * 4;		//�|���S����

//�n�ʂ̏���������
void InitMeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stone000.jpg",				//�e�N�X�`���̃t�@�C����
		&g_pTextureMeshfield);

	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VTX_X * MESHFIELD_VTX_Z,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshfield,
								NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	float fDevineX = MESHFIELD_SIZE / MESHFIELD_BLOCK_X;
	float fDevineZ = MESHFIELD_SIZE / MESHFIELD_BLOCK_Z;
	float fMaxX = -MESHFIELD_SIZE;
	float fMaxZ = MESHFIELD_SIZE;
	D3DXVECTOR3 pos[9] = {};

	for (int nCntPosX = 0; nCntPosX < MESHFIELD_VTX_X; nCntPosX++)
	{
		for (int nCntPosZ = 0; nCntPosZ < MESHFIELD_VTX_Z; nCntPosZ++)
		{
			pVtx[nCntPosX * MESHFIELD_VTX_X + nCntPosZ].pos = D3DXVECTOR3(fMaxX, 0.0f, fMaxZ);		//(x,y,z)
			pos[nCntPosX * MESHFIELD_VTX_X + nCntPosZ] = D3DXVECTOR3(fMaxX, 0.0f, fMaxZ);

			fMaxX += MESHFIELD_SIZE;

			if (fMaxX > MESHFIELD_SIZE)
			{
				fMaxX = -MESHFIELD_SIZE;
			}
		}
		fMaxZ -= MESHFIELD_SIZE;
	}

	////���_���W�̐ݒ�
	//pVtx[0].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);		//(x,y,z)
	//pVtx[1].pos = D3DXVECTOR3(0.0f,				0.0f,	MESHFIELD_SIZE);
	//pVtx[2].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	//pVtx[3].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	0.0f);
	//pVtx[4].pos = D3DXVECTOR3(0.0f,				0.0f,	0.0f);		//(x,y,z)
	//pVtx[5].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	0.0f);
	//pVtx[6].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	//pVtx[7].pos = D3DXVECTOR3(0.0f,				0.0f,	-MESHFIELD_SIZE);
	//pVtx[8].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);

	//�@���x�N�g���̐ݒ�
	//���_�J���[�̐ݒ�
	for (int nCntcol = 0;nCntcol < MESHFIELD_MAX;nCntcol++)
	{
		pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCntcol].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f , 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f , 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f , 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f , 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f , 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f , 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f , 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f , 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshfield->Unlock();

	WORD *pIdx;		//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshfield->Lock(0,0,(void**)&pIdx,0);

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

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshfield->Unlock();
}

//�n�ʂ̏I������
void UninitMeshfield(void)
{
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;					//���_�o�b�t�@�̔j��
	}
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;					//�C���f�b�N�X�o�b�t�@�̔j��
	}
}

//�n�ʂ̍X�V����
void UpdateMeshfield(void)
{

}

//�n�ʂ̕`�揈��
void DrawMeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

														//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshfield);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
									0,
									0,
									9,							//�p�ӂ������_�̐�
									0,
									12);						//�`�悷��v���~�e�B�u�̐�
}
