#ifndef _MODEL_H_								//���̃}�N����`������ĂȂ�������
#define _MODEL_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//�v���g�^�C�v�錾
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
D3DXVECTOR3 GetModelpos(void);
D3DXVECTOR3 GetModelposDest(void);
D3DXVECTOR3 GetModelRot(void);

#endif
