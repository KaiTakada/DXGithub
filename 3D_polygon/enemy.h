#ifndef _ENEMY_H_								//���̃}�N����`������ĂȂ�������
#define _ENEMY_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);

#endif
