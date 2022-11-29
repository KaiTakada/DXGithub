//============================
//
//�G�̏���(enemy.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "enemy.h"
#include "camera.h"
#include "bullet.h"
#include "shadow.h"
#include "input.h"

//�}�N����`
#define ENEMY_MAX (2)							//�G�̐�
#define ENEMY_TECMAX (10)						//�e�N�X�`���̐�
#define ENEMY_SPEED (0.25f)						//�G�̈ړ��X�s�[�h
#define ADULTWALL (1600.0f - 15.0f)				//��l�̕ǒ��"
#define STALK_COUNT (100)						//��荞�݃J�E���^�[

//�G�̗񋓌^
typedef enum
{
	ENEMYTYPE_CHAIR = 0,
	ENEMYTYPE_BATTAN,
	ENEMYTYPE_MAX
}ENEMYTYPE;

//�G�̍\����
typedef struct
{
	D3DXVECTOR3 pos;							//���݂̈ʒu
	D3DXVECTOR3 posDest;						//�\��̈ʒu
	D3DXVECTOR3 move;							//�ړ���
	D3DXVECTOR3 rot;							//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3 vtxMinModel, vtxMaxModel;		//�G�̍ŏ��l�E�ő�l
	DWORD dwNumMatEnemy = 0;					//�}�e���A���̐�
	int nIdxShadowEnemy = -1;					//�Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
}Enemy;

//x�t�@�C����
const char *c_apFilenameEnemy[] =
{
	"data\\MODEL\\chair000.x",
	"data\\MODEL\\battan.x",
};

//�O���[�o���ϐ��錾
LPD3DXMESH g_pMeshEnemy[ENEMY_MAX] = {};		//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatEnemy[ENEMY_MAX] = {};		//�}�e���A���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEnemy[ENEMY_MAX][ENEMY_TECMAX] = {};		//�e�N�X�`���ւ̃|�C���^
Enemy g_enemy[ENEMY_MAX];	//�G�̏��

//�G�̏���������
void InitEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntInit = 0; nCntInit < ENEMY_MAX; nCntInit++)
	{
		g_enemy[0].pos = D3DXVECTOR3(-200.0f, 10.0f, 0.0f);		//����������
		g_enemy[1].pos = D3DXVECTOR3(-300.0f, 0.0f, 200.0f);		//����������
		g_enemy[nCntInit].posDest = D3DXVECTOR3(0.0f, 10.0f, 100.0f);	//����������
		g_enemy[nCntInit].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������
		g_enemy[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������
		g_enemy[nCntInit].vtxMinModel = {};
		g_enemy[nCntInit].vtxMaxModel = {};


		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(c_apFilenameEnemy[nCntInit],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatEnemy[nCntInit],
			NULL,
			&g_enemy[nCntInit].dwNumMatEnemy,
			&g_pMeshEnemy[nCntInit]);

		//���_�����擾
		nNumVtx = g_pMeshEnemy[nCntInit]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y�̎擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy[nCntInit]->GetFVF());

		D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[nCntInit]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_enemy[nCntInit].dwNumMatEnemy; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C�������݂���
			 //�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,				//�e�N�X�`���̃t�@�C����
					&g_pTextureEnemy[nCntInit][nCntMat]);
			}
		}

		//���_�o�b�t�@�����b�N
		g_pMeshEnemy[nCntInit]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

			//���ׂĂ̒��_���r���ēG�̍ŏ��l�E�ő�l�𔲂��o��
			if (g_enemy[nCntInit].vtxMaxModel.x < vtx.x)
			{
				g_enemy[nCntInit].vtxMaxModel.x = vtx.x;
			}
			if (g_enemy[nCntInit].vtxMaxModel.y < vtx.y)
			{
				g_enemy[nCntInit].vtxMaxModel.y = vtx.y;
			}
			if (g_enemy[nCntInit].vtxMaxModel.z < vtx.z)
			{
				g_enemy[nCntInit].vtxMaxModel.z = vtx.z;
			}

			if (g_enemy[nCntInit].vtxMinModel.x > vtx.x)
			{
				g_enemy[nCntInit].vtxMinModel.x = vtx.x;
			}
			if (g_enemy[nCntInit].vtxMinModel.y > vtx.y)
			{
				g_enemy[nCntInit].vtxMinModel.y = vtx.y;
			}
			if (g_enemy[nCntInit].vtxMinModel.z > vtx.z)
			{
				g_enemy[nCntInit].vtxMinModel.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}
		//���_�o�b�t�@���A�����b�N
		g_pMeshEnemy[nCntInit]->UnlockVertexBuffer();

		//�e��ݒ�
		g_enemy[nCntInit].nIdxShadowEnemy = SetShadow();
	}
}

//�G�̏I������
void UninitEnemy(void)
{
	//if (g_pTextureEnemy != NULL)
	//{
	//	g_pTextureEnemy->Release();
	//	g_pTextureEnemy = NULL;					//�e�N�X�`���̔j��
	//}
	for (int nCntUninit = 0;nCntUninit < ENEMY_MAX;nCntUninit++)
	{
		if (g_pMeshEnemy[nCntUninit] != NULL)
		{
			g_pMeshEnemy[nCntUninit]->Release();
			g_pMeshEnemy[nCntUninit] = NULL;					//���b�V���̔j��
		}
		if (g_pBuffMatEnemy[nCntUninit] != NULL)
		{
			g_pBuffMatEnemy[nCntUninit]->Release();
			g_pBuffMatEnemy[nCntUninit] = NULL;					//�o�b�t�@�̔j��
		}
	}
}

//�G�̍X�V����
void UpdateEnemy(void)
{
	for (int nCntUpdate = 0; nCntUpdate < ENEMY_MAX; nCntUpdate++)
	{
		//�e�̈ʒu��ݒ�
		SetPositionShadow(g_enemy[nCntUpdate].nIdxShadowEnemy, g_enemy[nCntUpdate].pos);
	}
}
	

//�G�̕`�揈��
void DrawEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntDraw = 0; nCntDraw < ENEMY_MAX; nCntDraw++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_enemy[nCntDraw].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_enemy[nCntDraw].rot.y, g_enemy[nCntDraw].rot.x, g_enemy[nCntDraw].rot.z);

		D3DXMatrixMultiply(&g_enemy[nCntDraw].mtxWorld, &g_enemy[nCntDraw].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_enemy[nCntDraw].pos.x, g_enemy[nCntDraw].pos.y, g_enemy[nCntDraw].pos.z);

		D3DXMatrixMultiply(&g_enemy[nCntDraw].mtxWorld, &g_enemy[nCntDraw].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_enemy[nCntDraw].mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[nCntDraw]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_enemy[nCntDraw].dwNumMatEnemy; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEnemy[nCntDraw][nCntMat]);

			//�G(�p�[�c)�̕`��
			g_pMeshEnemy[nCntDraw]->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//�����蔻��
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	for (int nCntCol = 0;nCntCol < ENEMY_MAX;nCntCol++)
	{
		if ((g_enemy[nCntCol].vtxMaxModel.x > posOld.x || g_enemy[nCntCol].vtxMinModel.x < posOld.x ) &&
			(g_enemy[nCntCol].vtxMaxModel.x < pPos->x && g_enemy[nCntCol].vtxMinModel.x > pPos->x))
		{//z��
			pPos->x = posOld.x;
		}
		if ((g_enemy[nCntCol].vtxMaxModel.z > posOld.x || g_enemy[nCntCol].vtxMinModel.x < posOld.z) &&
			g_enemy[nCntCol].vtxMaxModel.z < pPos->x && g_enemy[nCntCol].vtxMinModel.x > pPos->z)
		{//x��
			pPos->z = posOld.z;
		}
	}
}
