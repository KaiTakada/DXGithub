#ifndef _CAMERA_H_								//���̃}�N����`������ĂȂ�������
#define _CAMERA_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//�J�����̗񋓌^
typedef enum
{
	CAMERA_V = 0,					//���_
	CAMERA_R,						//�����_
	CAMERA_MAX
}CAMERA;

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV;				//���_
	D3DXVECTOR3 posR;				//�����_
	D3DXVECTOR3 posVDest;			//�\�莋�_
	D3DXVECTOR3 posRDest;			//�\�蒍���_
	float posRAdjt;					//�����_���炵
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 vecU;				//������x�N�g��
	D3DXMATRIX mtxProjection;		//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X
	D3DXVECTOR3 rot;				//����
	D3DXVECTOR3 rotDest;			//�\�����
	float fAngle;					//�p�x
	float fDis;						//����
}Camera;

//�v���g�^�C�v�錾
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
D3DXVECTOR3 GetCameraPosV(void);
D3DXVECTOR3 GetCameraPosR(void);
D3DXVECTOR3 GetCameraVecU(void);
D3DXVECTOR3 GetCamerarot(void);
D3DXVECTOR3 GetCameramove(void);
bool GetVRswitch(void);
float GetVRDis(void);
void CameraStoke(void);

#endif
