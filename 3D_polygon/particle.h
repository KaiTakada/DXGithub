#ifndef _PARTICLE_H_		//���̃}�N����`������ĂȂ�������
#define _PARTICLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, int nType);

#endif
