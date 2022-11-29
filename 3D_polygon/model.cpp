//============================
//
//モデルの処理(model.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "model.h"
#include "camera.h"
#include "bullet.h"
#include "shadow.h"
#include "enemy.h"
#include "input.h"

//マクロ定義
#define MODEL_SPEED (0.25f)						//プレイヤーの移動スピード
#define ADULTWALL (1600.0f - 15.0f)				//大人の壁定期"
#define STALK_COUNT (100)						//回り込みカウンター

//モデルの構造体
typedef struct
{
	D3DXVECTOR3 pos;							//現在の位置
	D3DXVECTOR3 posOld;							//前回の位置
	D3DXVECTOR3 posDest;						//予定の位置
	D3DXVECTOR3 move;							//移動量
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
}Model;

//グローバル変数宣言
LPD3DXMESH g_pMeshModel = NULL;					//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;			//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureModel[5] = {};		//テクスチャへのポインタ
DWORD g_dwNumMatModel = 0;						//マテリアルの数
int g_nIdxShadowPlayer = -1;					//対象の影のインデックス(番号)
Model g_model;									//モデルの情報
int g_nStalkCounter;							//カメラの回り込みカウント
bool g_bStalk;									//カメラの回り込みのON/OFF

//プレイヤーの初期化処理
void InitModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_model.pos = D3DXVECTOR3(0.0f, 10.0f, 100.0f);			//初期化する
	g_model.posOld = g_model.pos;							//初期化する
	g_model.posDest = D3DXVECTOR3(0.0f, 10.0f, 100.0f);		//初期化する
	g_model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//初期化する
	g_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//初期化する

	g_bStalk = false;

//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\head.x",
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_pBuffMatModel,
						NULL,
						&g_dwNumMatModel,
						&g_pMeshModel);

	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0;nCntMat < (int)g_dwNumMatModel;nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,				//テクスチャのファイル名
				&g_pTextureModel[nCntMat]);
		}
	}

	//影を設定
	g_nIdxShadowPlayer = SetShadow();
}

//プレイヤーの終了処理
void UninitModel(void)
{
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;					//テクスチャ破棄
	}
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;					//バッファの破棄
	}
}

//プレイヤーの更新処理
void UpdateModel(void)
{
	float fRotMove = g_model.rot.y;	//現在の向き
	float fRotDest = g_model.rot.y;	//目標の向き
	float fRotDiff = 0.0f;			//差分
	g_model.posOld = g_model.pos;							//更新する

	if (GetKeyboardPress(DIK_S) == true)
	{
		D3DXVECTOR3 CameraRot = GetCamerarot();

		if (GetKeyboardPress(DIK_A) == true)
		{//上かつ左キーを押したとき
			g_model.move.x += sinf(CameraRot.y + -0.75f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + -0.75f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 0.25f * D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//上かつ右キーを押したとき
			g_model.move.x += sinf(CameraRot.y + 0.75f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 0.75f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + -0.25f * D3DX_PI;
		}
		else
		{//上キーのみを押したとき
			g_model.move.x += sinf(CameraRot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 0.0f * D3DX_PI;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		D3DXVECTOR3 CameraRot = GetCamerarot();

		if (GetKeyboardPress(DIK_A) == true)
		{//下かつ左キーを押したとき
			g_model.move.x += sinf(CameraRot.y + -0.25f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + -0.25f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 0.75f * D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//下かつ右キーを押したとき
			g_model.move.x += sinf(CameraRot.y + 0.25f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 0.25f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + -0.75f * D3DX_PI;
		}
		else
		{//下キーのみを押したとき
			g_model.move.x += sinf(CameraRot.y + 0.0f * D3DX_PI) * MODEL_SPEED;		//x
			g_model.move.z += cosf(CameraRot.y + 0.0f * D3DX_PI) * MODEL_SPEED;		//y
			fRotDest = CameraRot.y + 1.0f * D3DX_PI;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{//左キーのみを押したとき
		D3DXVECTOR3 CameraRot = GetCamerarot();

		g_model.move.x += sinf(CameraRot.y + -0.5f * D3DX_PI) * MODEL_SPEED;
		g_model.move.z += cosf(CameraRot.y + -0.5f * D3DX_PI) * MODEL_SPEED;
		fRotDest = CameraRot.y + 0.5f * D3DX_PI;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//右キーのみを押したとき
		D3DXVECTOR3 CameraRot = GetCamerarot();

		g_model.move.x += sinf(CameraRot.y + 0.5f * D3DX_PI) * MODEL_SPEED;
		g_model.move.z += cosf(CameraRot.y + 0.5f * D3DX_PI) * MODEL_SPEED;
		fRotDest = CameraRot.y + -0.5f * D3DX_PI;
	}

	if (GetKeyboardPress(DIK_SPACE) == true)
	{//[ - ]キーが押された
		SetBullet(g_model.pos, g_model.rot,BULLETTYPE_PLAYER);
	}

	if (GetKeyboardTrigger(DIK_F6) == true)
	{//F6キーが押された
		g_bStalk = g_bStalk ? false : true;
		g_nStalkCounter = 0;
	}

	fRotDiff = fRotDest - fRotMove;

	//角度の値を修正する
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}
	else if (fRotDiff > D3DX_PI)
	{
		fRotDiff += -D3DX_PI * 2;
	}

	g_model.rot.y += fRotDiff * 0.1f;		//移動方向(角度)の補正

	//角度の値を修正する
	if (g_model.rot.y < -D3DX_PI)
	{
		g_model.rot.y += D3DX_PI * 2;
	}
	else if (g_model.rot.y > D3DX_PI)
	{
		g_model.rot.y += -D3DX_PI * 2;
	}

	//位置を更新
	g_model.pos += g_model.move;
	//g_model.posDest = g_model.pos + g_model.move;
	//g_model.posDest.x += sinf(g_model.rot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//x
	//g_model.posDest.z += cosf(g_model.rot.y + 1.0f * D3DX_PI) * MODEL_SPEED;		//y

	//移動量を更新(減衰させる)
	g_model.move.x += (0 - g_model.move.x) * 0.1f;
	g_model.move.z += (0 - g_model.move.z) * 0.1f;

	if (g_bStalk == true)
	{
		if (g_model.move.x < 1.0f &&
			g_model.move.y < 1.0f &&
			g_model.move.z < 1.0f &&
			g_model.move.x > -1.0f &&
			g_model.move.y > -1.0f &&
			g_model.move.z > -1.0f)
		{//動きがなくなったら
			g_nStalkCounter++;

			if (g_nStalkCounter >= STALK_COUNT)
			{
				CameraStoke();
			}
		}
		else
		{
			g_nStalkCounter = 0;
		}
	}

	CollisionEnemy(&g_model.pos,g_model.posOld);


	if (g_model.pos.x > ADULTWALL)
	{//大人の壁に当たったら
		g_model.pos.x = ADULTWALL;			
	}
	if (g_model.pos.x < -ADULTWALL)
	{//大人の壁に当たったら
		g_model.pos.x = -ADULTWALL;			
	}
	if (g_model.pos.z > ADULTWALL)
	{//大人の壁に当たったら
		g_model.pos.z = ADULTWALL;			
	}
	if (g_model.pos.z < -ADULTWALL)
	{//大人の壁に当たったら
		g_model.pos.z = -ADULTWALL;
	}

	//影の位置を設定
	SetPositionShadow(g_nIdxShadowPlayer, g_model.pos);
}
	

//プレイヤーの描画処理
void DrawModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_model.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_model.rot.y, g_model.rot.x, g_model.rot.z);

	D3DXMatrixMultiply(&g_model.mtxWorld, &g_model.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_model.pos.x, g_model.pos.y, g_model.pos.z);

	D3DXMatrixMultiply(&g_model.mtxWorld, &g_model.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_model.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	char sStalk[5];

	switch (g_bStalk)
	{
	case true:
		strcpy(sStalk, "ON");
		break;

	case false:
		strcpy(sStalk, "OFF");
		break;
	}

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[5][256];
	LPD3DXFONT Font = GetFont();

	//文字列に代入
	sprintf(&aStr[0][0], "モデルの位置：%.2f %.2f %.2f", g_model.pos.x, g_model.pos.y, g_model.pos.z);
	sprintf(&aStr[1][0], "\nモデルの向き：%.2f %.2f %.2f", g_model.rot.x, g_model.rot.y, g_model.rot.z);
	sprintf(&aStr[2][0], "\n\nモデルの移動量:%.2f %.2f %.2f", g_model.move.x, g_model.move.y, g_model.move.z);
	sprintf(&aStr[3][0], "\n\n\nモデルの操作<←/↑/↓/→>");
	sprintf(&aStr[4][0], "\n\n\n\nカメラの回り込み<F6>(%s)",sStalk);

	//テキスト描画
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[1][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[2][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[3][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Font->DrawText(NULL, &aStr[4][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//モデルの座標を渡す
D3DXVECTOR3 GetModelpos(void)
{
	return g_model.pos;
}

//モデルの予定座標を渡す
D3DXVECTOR3 GetModelposDest(void)
{
	return g_model.posDest;
}

//モデルの予定座標を渡す
D3DXVECTOR3 GetModelRot(void)
{
	return g_model.rot;
}
