//============================
//
//カメラの処理(camera.cpp)
// Author: Kai Takada
//
//============================
#include "main.h"
#include "camera.h"
#include "model.h"
#include "input.h"
#include "string.h"

//マクロ定義
#define CAMERA_SPEED (1.0f)				//移動スピード
#define CAMERA_ROTSPEED (0.05f)			//回転スピード
#define CAMERA_DISTANCE (300)			//視点・注視点の距離
#define CAMERA_INITPOSY (150.0f)		//視点・注視点のy初期値
#define CAMERA_RADJT (60.0f)			//注視点のずらし値

//プロトタイプ宣言
void CameraMove(void);
void CameraVR(void);
void RotCamera(void);
void CameraPosRRotate(void);
void CameraPosVRotate(void);
void CameraPosRControll(void);
void CameraPosVControll(void);

//グローバル変数定義
Camera g_camera;					//カメラの情報
bool g_bVR = false;					//視点・注視点の切り替え
bool g_bVRButtonDown = false;		//視点・注視点左右操作しているかどうか
bool g_bUButtonDown = false;		//視点・注視点の上下操作しているかどうか
float g_fVRLength;					//視点・注視点の距離(xz)
float g_fCameraAngle;				//視点・注視点の角度

//============================
//カメラの初期化処理
//============================
void InitCamera(void)
{
	g_camera.fDis = CAMERA_DISTANCE;
	g_camera.posV = D3DXVECTOR3( 0.0f, CAMERA_INITPOSY, -g_camera.fDis);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posVDest = g_camera.posV;
	g_camera.posRDest = g_camera.posR;
	g_camera.posRAdjt = CAMERA_RADJT;
	g_camera.move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
//カメラの終了処理
//============================
void UninitCamera(void)
{

}

//============================
//カメラの更新処理
//============================
void UpdateCamera(void)
{
	//2点間の距離
	//RotCamera();

	//カメラ移動
	CameraMove();

	//視点・注視点の操作
	CameraVR();

	//CameraPosRRotate();
	
	CameraPosVRotate();
	
	CameraPosRControll();
	
	CameraPosVControll();
}

//============================
//カメラの設定処理
//============================
void SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,
								10000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,&g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,			//視点
						&g_camera.posR,			//注視点
						&g_camera.vecU);		//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//角度の取り方
void RotCamera(void)
{
	//z軸の距離とx軸の距離の合計(平面での距離)
	g_fVRLength = (g_camera.posV.z - g_camera.posR.z)
				+ (g_camera.posV.x - g_camera.posR.x);

	if (g_fVRLength < 0)
	{
		g_fVRLength *= -1;
	}
}

//=======================================
//============  カメラ移動  =============
//=======================================
void CameraMove(void)
{
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//上かつ左キーを押したとき
			g_camera.posV.x += sinf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.25f * -D3DX_PI) * CAMERA_SPEED;		//y
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//上かつ右キーを押したとき
			g_camera.posV.x += sinf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.25f * D3DX_PI) * CAMERA_SPEED;		//y
		}
		else
		{//上キーのみを押したとき
			g_camera.posV.x += sinf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.0f * D3DX_PI) * CAMERA_SPEED;		//y
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//上かつ左キーを押したとき
			g_camera.posV.x += sinf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + -0.75f * D3DX_PI) * CAMERA_SPEED;		//y
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//上かつ右キーを押したとき
			g_camera.posV.x += sinf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 0.75f * D3DX_PI) * CAMERA_SPEED;		//y
		}
		else
		{//上キーのみを押したとき
			g_camera.posV.x += sinf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posV.z += cosf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//y
			g_camera.posR.x += sinf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//x
			g_camera.posR.z += cosf(g_camera.rot.y + 1.0f * D3DX_PI) * CAMERA_SPEED;		//y
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{//左キーのみを押したとき
		g_camera.posV.x += sinf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posV.z += cosf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//y
		g_camera.posR.x += sinf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posR.z += cosf(g_camera.rot.y + -0.5f * D3DX_PI) * CAMERA_SPEED;		//y
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//右キーのみを押したとき
		g_camera.posV.x += sinf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posV.z += cosf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//y
		g_camera.posR.x += sinf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//x
		g_camera.posR.z += cosf(g_camera.rot.y + 0.5f * D3DX_PI) * CAMERA_SPEED;		//y
	}

	g_camera.move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//移動量を更新(減衰させる)
	g_camera.move.x += (0 - g_camera.move.x) * 0.15f;
	g_camera.move.y += (0 - g_camera.move.y) * 0.15f;
}

//=======================================
//========  視点・注視点の操作  =========
//=======================================
void CameraVR(void)
{
	g_bVRButtonDown = false;

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F2キーが押された
		g_bVR = g_bVR ? false : true;

		//D3DXVECTOR3 PosTemp = g_camera.posV;
		//g_camera.posV = g_camera.posR;
		//g_camera.posR = PosTemp;
	}

	//注視点とモデルの座標の差
	g_camera.posRDest = D3DXVECTOR3(GetModelpos().x + sinf(GetModelRot().y + D3DX_PI) * g_camera.posRAdjt,
									GetModelpos().y,
									GetModelpos().z + cosf(GetModelRot().y + D3DX_PI) * g_camera.posRAdjt);
	
	//目標の始点を設定
	g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * -g_camera.fDis;	//(x)

	D3DXVECTOR3 RDiff = D3DXVECTOR3(g_camera.posRDest.x - g_camera.posR.x, 0.0f, g_camera.posRDest.z - g_camera.posR.z);
	D3DXVECTOR3 VDiff = D3DXVECTOR3(g_camera.posVDest.x - g_camera.posV.x, 0.0f, g_camera.posVDest.z - g_camera.posV.z);

	//注視点を更新(接近させる)
	g_camera.posR += RDiff * 0.05f;

	//注視点を更新(接近させる)
	g_camera.posV += VDiff * 0.05f;
}

//===========================
//視点旋回
//===========================
void CameraPosVRotate(void)
{
	if (GetKeyboardPress(DIK_Z) == true || GetKeyboardPress(DIK_C) == true)
	{
		if (GetKeyboardPress(DIK_Z) == true)
		{
			g_camera.rot.y -= CAMERA_ROTSPEED;
		}
		else if (GetKeyboardPress(DIK_C) == true)
		{
			g_camera.rot.y += CAMERA_ROTSPEED;
		}
	}

	//視点の旋回
	g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y) * -g_camera.fDis;	//(x)
	g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y) * -g_camera.fDis;

	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}
}

//===========================
//注視点旋回
//===========================
void CameraPosRRotate(void)
{
	if (GetKeyboardPress(DIK_Q) == true || GetKeyboardPress(DIK_E) == true)
	{
		if (GetKeyboardPress(DIK_Q) == true)
		{
			g_camera.rot.y -= CAMERA_ROTSPEED;
		}
		else if (GetKeyboardPress(DIK_E) == true)
		{
			g_camera.rot.y += CAMERA_ROTSPEED;
		}
	}

	//注視点の旋回
	g_camera.posR.x = GetModelposDest().x + sinf(g_camera.rot.y);
	g_camera.posR.z = GetModelposDest().z + cosf(g_camera.rot.y);

	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}
}

//===========================
//注視点のみの操作
//===========================
void CameraPosRControll(void)
{
	if (GetKeyboardPress(DIK_Y) == true)
	{//視点のみ上下
		g_camera.posV.y += CAMERA_SPEED;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//視点のみ上下
		g_camera.posV.y -= CAMERA_SPEED;
	}
}

//===========================
//視点のみの操作
//===========================
void CameraPosVControll(void)
{
	if (GetKeyboardPress(DIK_T) == true)
	{//視点のみ上下
		g_camera.posR.y += CAMERA_SPEED;
	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//視点のみ上下
		g_camera.posR.y -= CAMERA_SPEED;
	}
}

//===========================
//回り込み処理
//===========================
void CameraStoke(void)
{
	g_camera.rotDest.y = GetModelRot().y + D3DX_PI;

	//差分
	float fDiff = 0.0f;
	fDiff = g_camera.rotDest.y - g_camera.rot.y;

	//PIからはみ出ていた場合の補正
	if (fDiff < -D3DX_PI)
	{
		fDiff += D3DX_PI * 2;
	}
	else if (fDiff > D3DX_PI)
	{
		fDiff += -D3DX_PI * 2;
	}
	
	g_camera.rot.y += fDiff * 0.01f;

	//PIからはみ出ていた場合の補正
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y += -D3DX_PI * 2;
	}
}

//視点座標を渡す
D3DXVECTOR3 GetCameraPosV(void)
{
	return g_camera.posV;
}

//注視点座標を渡す
D3DXVECTOR3 GetCameraPosR(void)
{
	return g_camera.posR;
}

//上方向ベクトルを渡す
D3DXVECTOR3 GetCameraVecU(void)
{
	return g_camera.vecU;
}

//向きを渡す
D3DXVECTOR3 GetCamerarot(void)
{
	return g_camera.rot;
}

//カメラの移動量を渡す
D3DXVECTOR3 GetCameramove(void)
{
	return g_camera.move;
}

//視点・注視点スイッチを渡す
bool GetVRswitch(void)
{
	return g_bVR;
}

//二点間の距離を渡す
float GetVRDis(void)
{
	return g_camera.fDis;
}
