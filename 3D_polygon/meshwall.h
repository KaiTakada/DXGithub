#ifndef _MESHWALL_H_								//���̃}�N����`������ĂȂ�������
#define _MESHWALL_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//���b�V���E�H�[���̍\����
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 rot;							//�ړ���
	D3DXVECTOR3 size;							//�傫��
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	bool bUse;
}Meshwall;

//�v���g�^�C�v�錾
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
Meshwall GetMeshWall(void);

#endif
