#ifndef _SHADOW_H_								//���̃}�N����`������ĂȂ�������
#define _SHADOW_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//�v���g�^�C�v�錾
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);		//�ݒ菈��
void SetPositionShadow(int nIdxShadow,D3DXVECTOR3 pos);		//�ʒu�ݒ菈��
void SetShadowEnable(int nIdxShadow);							//�g�p�؂�ւ�

#endif