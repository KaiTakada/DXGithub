//============================================
//�p�[�e�B�J���̏���(particle.cpp)
//Author:Kai Takada
//============================================
#include "main.h"
#include "particle.h"
#include "effect.h"
#include <time.h>

//�}�N����`
#define PARTICLE_MAX (256)			//�p�[�e�B�J���̍ő吔
#define PARTICLE_EFFECT (16)		//�G�t�F�N�g�̐�����
#define PARTICLE_RADIUS (10.0f)		//�G�t�F�N�g�̔��a
#define PARTICLE_LIFE (10)			//�p�[�e�B�N���̎���
#define PARTICLE_EFFECTLIFE (180)	//�G�t�F�N�g�̎���

//�p�[�e�B�N���̗񋓌^
typedef enum
{
	PARTICLETYPE_EXPLOSION = 0,		//�����̎��p
	PARTICLETYPE_MAX,
}PARTICLETYPE;

//�p�[�e�B�N���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//���W
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR col;			//�F
	float fRadius;			//���a
	int nLife;				//����
	int nEffectLife;		//����
	bool bUse;				//�g���Ă��邩�H
}Particle;

//�O���[�o���ϐ��錾
Particle g_aParticle[PARTICLE_MAX];						//�p�[�e�B�N���̏��

//�p�[�e�B�J���̏���������
void InitParticle(void)
{
	srand((unsigned int)time(NULL));

	//�G�t�F�N�g�̏�������������
	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].fRadius = PARTICLE_RADIUS;
		g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;
		g_aParticle[nCntParticle].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//�p�[�e�B�J���̏I������
void UninitParticle(void)
{

}

//�p�[�e�B�J���̍X�V����
void UpdateParticle(void)
{
	for (int nCntUpdate = 0; nCntUpdate < PARTICLE_MAX; nCntUpdate++)
	{
		if (g_aParticle[nCntUpdate].bUse == true)
		{//�p�[�e�B�N�����g�p����Ă���
			//�p�[�e�B�N���̐���
			for (int nCntParticle = 0; nCntParticle < PARTICLE_EFFECT; nCntParticle++)
			{
				g_aParticle[nCntParticle].pos;
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
				g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
				g_aParticle[nCntParticle].col;
				g_aParticle[nCntParticle].fRadius;
				g_aParticle[nCntParticle].nLife;
				g_aParticle[nCntParticle].nEffectLife;

				//�G�t�F�N�g�̐ݒ�
				SetEffect(g_aParticle[nCntUpdate].pos, g_aParticle[nCntUpdate].move, g_aParticle[nCntUpdate].col, g_aParticle[nCntUpdate].nEffectLife, g_aParticle[nCntUpdate].fRadius);
			}

			//�������
			g_aParticle[nCntUpdate].nLife--;

			if (g_aParticle[nCntUpdate].nLife <= 0)
			{//�������s�����玀��
				g_aParticle[nCntUpdate].bUse = false;
			}
		}
	}
}

//�p�[�e�B�J���̕`�揈��
void DrawParticle(void)
{

}

//�p�[�e�B�J���̐ݒ菈��
void SetParticle(D3DXVECTOR3 pos, int nType)
{
	for (int nCntSet = 0;nCntSet < PARTICLE_MAX;nCntSet++)
	{
		if (g_aParticle[nCntSet].bUse == false)
		{
			g_aParticle[nCntSet].pos = pos;

			switch (nType)
			{
			case PARTICLETYPE_EXPLOSION:
				g_aParticle[nCntSet].move = D3DXVECTOR3(1.0f,1.0f,1.0f);
				g_aParticle[nCntSet].col = D3DXCOLOR(0.5f, 0.1f, 0.1f, 1.0f);
				g_aParticle[nCntSet].fRadius = PARTICLE_RADIUS;
				g_aParticle[nCntSet].nLife = PARTICLE_LIFE;
				g_aParticle[nCntSet].nEffectLife = PARTICLE_EFFECTLIFE;
				break;
			}

			g_aParticle[nCntSet].bUse = true;
			break;
		}
	}
}
