//============================
//
//影の処理(shadow.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "shadow.h"

//マクロ定義
#define SHADOW_SIZE (10.0f)		//影の半径の初期値
#define SHADOW_MAX (256)		//影の数

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//現在使われております？
}Shadow;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
Shadow g_aShadow[SHADOW_MAX];						//影の情報

//影の初期化処理
void InitShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",				//テクスチャのファイル名
		&g_pTextureShadow);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * SHADOW_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntInit = 0; nCntInit < SHADOW_MAX; nCntInit++)
	{
		g_aShadow[nCntInit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する
		g_aShadow[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する
		g_aShadow[nCntInit].bUse = false;	//初期化する

	//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-SHADOW_SIZE, 0.0f, SHADOW_SIZE);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE);
		pVtx[2].pos = D3DXVECTOR3(-SHADOW_SIZE, 0.0f, -SHADOW_SIZE);
		pVtx[3].pos = D3DXVECTOR3(SHADOW_SIZE, 0.0f, -SHADOW_SIZE);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//影の終了処理
void UninitShadow(void)
{
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;					//バッファの破棄
	}
}

//影の更新処理
void UpdateShadow(void)
{

}

//影の描画処理
void DrawShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntDraw = 0; nCntDraw < SHADOW_MAX; nCntDraw++)
	{
		if (g_aShadow[nCntDraw].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntDraw].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aShadow[nCntDraw].rot.y, g_aShadow[nCntDraw].rot.x, g_aShadow[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntDraw].mtxWorld, &g_aShadow[nCntDraw].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aShadow[nCntDraw].pos.x, g_aShadow[nCntDraw].pos.y, g_aShadow[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntDraw].mtxWorld, &g_aShadow[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntDraw].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				nCntDraw * 4,									//描画する最初のインデックス
				2);												//描画するプリミティブ数
		}
	}

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1][256];
	int nCnterShadow = 0;
	LPD3DXFONT Font = GetFont();
	for (int nCnt = 0; nCnt < SHADOW_MAX; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == true)
		{
			nCnterShadow++;
		}
	}

	//文字列に代入
	sprintf(&aStr[0][0], "\n\n\n\n\n\n\n\n\n\n\n影の数：%d", nCnterShadow);

	//テキスト描画
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//設定処理
int SetShadow(void)
{
	for (int nCntSet = 0;nCntSet < SHADOW_MAX;nCntSet++)
	{
		if (g_aShadow[nCntSet].bUse == false)
		{
 			g_aShadow[nCntSet].bUse = true;
			return nCntSet;
		}
	}

	return -1;
}

//位置設定処理
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos.x = pos.x;
	g_aShadow[nIdxShadow].pos.z = pos.z;
}

//影の使用状態変更
void SetShadowEnable(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}
