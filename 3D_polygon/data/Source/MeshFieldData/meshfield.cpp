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
#define MESHFIELD_SIZE (400.0f)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshfield;							//�ʒu
D3DXVECTOR3 g_rotMeshfield;							//�ړ���
D3DXMATRIX g_mtxWorldMeshfield;						//���[���h�}�g���b�N�X

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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	0.0f);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	pVtx[2].pos = D3DXVECTOR3(0.0f,				0.0f,	0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,				0.0f,	MESHFIELD_SIZE);
	pVtx[4].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	0.0f);		//(x,y,z)
	pVtx[5].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	pVtx[6].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	pVtx[7].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	pVtx[8].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	pVtx[9].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f,			0.0f,	-MESHFIELD_SIZE);		//(x,y,z)
	pVtx[11].pos = D3DXVECTOR3(0.0f,			0.0f,	0.0f);
	pVtx[12].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	pVtx[13].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f , 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.0f , 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f , 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.5f , 0.0f);
	pVtx[4].tex = D3DXVECTOR2(1.0f , 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[10].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[12].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.5f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshfield->Unlock();
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
		g_pVtxBuffMeshfield = NULL;					//�o�b�t�@�̔j��
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

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,												//�`�悷��ŏ��̃C���f�b�N�X
		12);											//�`�悷��v���~�e�B�u��
}
