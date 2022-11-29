//============================
//
//ポリゴンの処理(polygon.cpp)
// Author: Kai Takada
//
//============================
#include "main.h"
#include "light.h"

//マクロ定義
#define MAX_LIGHT (3)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureLight = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLight = NULL;	//頂点バッファへのポインタ
D3DLIGHT9 g_light[MAX_LIGHT];		//ライトの情報

//ライトの初期化処理
void InitLight(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{//ライトの拡散校と方向を設定
		case 0:
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;

		case 1:
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			break;

		case 2:
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトルを正規化する(1.0fにする)
		g_light[nCntLight].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//ライトの終了処理
void UninitLight(void)
{

}

//ライトの更新処理
void UpdateLight(void)
{

}
