#include "main.h"
#include "Explosion.h"
#include "input.h"
#include "bullet.h"

//�}�N����`
#define EXPLOSION_MAX (128)		//�����̍ő吔
#define EXPLOSION_SIZE (10.0f)		//�����̕�(���a)

//�����\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	int nCounterAnim;		//�A�j���[�V�����J�E���^�[
	int nPatternAnim;		//�A�j���[�V�����p�^�[��No.
	bool bUse;				//�g�p���Ă��邩�ǂ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
}Explosion;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;			//�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[EXPLOSION_MAX];					//�e�̏��

//�v���C���[�̏���������
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\Texture\\Explosion000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureExplosion);

	//�e�̏�������������
	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EXPLOSION_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_3D *pVtx;		//���_�o�b�t�@�ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-EXPLOSION_SIZE, EXPLOSION_SIZE, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(EXPLOSION_SIZE, EXPLOSION_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EXPLOSION_SIZE, -EXPLOSION_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(EXPLOSION_SIZE, -EXPLOSION_SIZE, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

		pVtx += 4;											//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//�v���C���[�̏I������
void UninitExplosion(void)
{
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;					//�o�b�t�@�̔j��
	}
}

//�����̍X�V����
void UpdateExplosion(void)
{
	VERTEX_3D *pVtx;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă���

			//�J�E���^��i�߂ăp�^�[��No.���X�V����
			g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 8;

			g_aExplosion[nCntExplosion].nCounterAnim++;		//�J�E���^�[�����Z

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 8) == 0)
			{
				//�p�^�[��No.�̍X�V����
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 10;

				//�e�N�X�`���̍X�V
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % 8) * 0.125f, 0.0f);				//(x,y)
				pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % 8) * 0.125f + 0.125f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % 8) * 0.125f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % 8) * 0.125f + 0.125f, 1.0f);
			}

			if (g_aExplosion[nCntExplosion].nPatternAnim > 7)
			{//���p�^�[�����𒴂�����
				g_aExplosion[nCntExplosion].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//�����̕`�揈��
void DrawExplosion(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;									//�r���[�}�g���b�N�X�擾�p

	for (int nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�e���g�p����Ă���
		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExplosion[nCntExplosion].mtxWorld, NULL, &mtxView);
			g_aExplosion[nCntExplosion].mtxWorld._41 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._42 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			//Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				nCntExplosion * 4,								//�`�悷��ŏ��̃C���f�b�N�X
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
}

//�e�̐ݒ菈��
void SetExplosion(D3DXVECTOR3 pos)
{
	VERTEX_3D*pVtx;
	int nCntExplosion;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aExplosion[nCntExplosion].pos = pos;

			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);					//(x,y)
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);


			g_aExplosion[nCntExplosion].bUse = true;				//�g�p���Ă����Ԃɂ���
			break;
		}
		pVtx += 4;											//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}