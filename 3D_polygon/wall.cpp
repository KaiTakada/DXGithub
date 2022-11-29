//============================
//
//ポリゴンの処理(wall.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "wall.h"

//マクロ定義
#define WALL_SIZEX (400.0f)
#define WALL_SIZEY (100.0f)
#define MAX_WALL (4)			//最大壁配置数

//壁の構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//移動量
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	bool bUse;					//使用しているか
}Wall;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//テクスチャへのポインタ 
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファへのポインタ
Wall g_wall[MAX_WALL];

//壁の初期化処理
void InitWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\cat000.jpg",				//テクスチャのファイル名
		&g_pTextureWall);

	for (int nCntInit = 0;nCntInit < MAX_WALL;nCntInit++)
	{//初期化する
		g_wall[nCntInit].pos = D3DXVECTOR3(0.0f, 0.0f, WALL_SIZEX);
		g_wall[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wall[nCntInit].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0,0,(void**)&pVtx,0);

	for (int nCntWall = 0;nCntWall < MAX_WALL;nCntWall++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-WALL_SIZEX, WALL_SIZEY * 2, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(WALL_SIZEX, WALL_SIZEY * 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-WALL_SIZEX, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(WALL_SIZEX, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -WALL_SIZEX), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, WALL_SIZEX), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(WALL_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(-WALL_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//壁の終了処理
void UninitWall(void)
{
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;					//バッファの破棄
	}
}

//壁の更新処理
void UpdateWall(void)
{

}

//壁の描画処理
void DrawWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntDraw = 0; nCntDraw < MAX_WALL; nCntDraw++)
	{
		if (g_wall[nCntDraw].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_wall[nCntDraw].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_wall[nCntDraw].rot.y, g_wall[nCntDraw].rot.x, g_wall[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_wall[nCntDraw].mtxWorld, &g_wall[nCntDraw].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_wall[nCntDraw].pos.x, g_wall[nCntDraw].pos.y, g_wall[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_wall[nCntDraw].mtxWorld, &g_wall[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_wall[nCntDraw].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(nCntDraw, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(nCntDraw, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				nCntDraw * 4,									//描画する最初のインデックス
				2);												//描画するプリミティブ数
		}
	}
}

//壁の設置処理
void SetWall(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	for (int nCntSet = 0;nCntSet < MAX_WALL;nCntSet++)
	{
		if (g_wall[nCntSet].bUse == false)
		{
			g_wall[nCntSet].pos = pos;
			g_wall[nCntSet].rot = rot;
			g_wall[nCntSet].bUse = true;
			break;
		}
	}
}
