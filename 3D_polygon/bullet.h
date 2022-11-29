#ifndef _BULLET_H_								//���̃}�N����`������ĂȂ�������
#define _BULLET_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//�e�̎�ނ̗񋓌^
typedef enum
{
	BULLETTYPE_PLAYER = 0,						//�v���C���[�̒e
	BULLETTYPE_MAX
}BULLETTYPE;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BULLETTYPE type);
void CollisionWall(int nCnt);

#endif