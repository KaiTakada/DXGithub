#ifndef _CAMERA_H_								//このマクロ定義がされてなかったら
#define _CAMERA_H_								//二重インクルード防止のマクロを定義する

//カメラの列挙型
typedef enum
{
	CAMERA_V = 0,					//視点
	CAMERA_R,						//注視点
	CAMERA_MAX
}CAMERA;

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;				//視点
	D3DXVECTOR3 posR;				//注視点
	D3DXVECTOR3 posVDest;			//予定視点
	D3DXVECTOR3 posRDest;			//予定注視点
	float posRAdjt;					//注視点ずらし
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 vecU;				//上方向ベクトル
	D3DXMATRIX mtxProjection;		//プロジェクションマトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス
	D3DXVECTOR3 rot;				//向き
	D3DXVECTOR3 rotDest;			//予定向き
	float fAngle;					//角度
	float fDis;						//距離
}Camera;

//プロトタイプ宣言
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
