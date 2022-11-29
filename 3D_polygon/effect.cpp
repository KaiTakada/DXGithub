//===========================================
//
//エフェクトの情報(particle.cpp)
//Author:Kai Takada
//
//===========================================
#include "main.h"
#include "bullet.h"
#include "effect.h"
#include "stdio.h"
#include "input.h"

//マクロ定義
#define EFFECT_MAX (32768)		//エフェクトの最大数
#define EFF_RADIUS (10.0f)		//エフェクトの半径
#define EFFECT_SPEED (0.1f)		//エフェクトのスピード
#define EFF_COLOR (0.1f)		//エフェクトの色

//エフェクトの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXCOLOR col;				//色
	float fRadius;				//半径(大きさ)
	int nMaxLife;				//半径×寿命
	int nLife;					//寿命
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	bool bUse;					//使用しているかどうか
}Effect;

//プロトタイプ宣言

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			//ポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
Effect g_aEffect[EFFECT_MAX];						//エフェクトの情報

//==============================
//エフェクトの初期化処理
//==============================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\Texture\\effect000.jpg",			//テクスチャのファイル名
		&g_pTextureEffect);

	//エフェクトの情報を初期化する
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 100;
		g_aEffect[nCntEffect].nLife = 120;
		g_aEffect[nCntEffect].nMaxLife = 120;
		g_aEffect[nCntEffect].bUse = false;			//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_3D *pVtx;		//

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-EFF_RADIUS, EFF_RADIUS, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(EFF_RADIUS, EFF_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EFF_RADIUS, -EFF_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(EFF_RADIUS, -EFF_RADIUS, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.1f, 0.1f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ文進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの終了処理
void UninitEffect(void)
{
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;					//バッファの破棄
	}
}

//エフェクトの更新処理
void UpdateEffect(void)
{
	int nCntEffect;
	Effect *pEffect = &g_aEffect[0];
	
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)
		{//エフェクトが使用されている

			if ((pEffect->nLife % 8) == 0)
			{//頂点座標の更新(ポリゴンサイズの更新)
				pEffect->fRadius *= ((float)pEffect->nLife / (float)pEffect->nMaxLife);
			}

			//座標の移動
			pEffect->pos.x += pEffect->rot.x * EFFECT_SPEED;
			pEffect->pos.y += pEffect->rot.y * EFFECT_SPEED;
			pEffect->pos.z += pEffect->rot.z * EFFECT_SPEED;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-pEffect->fRadius, pEffect->fRadius, 0.0f);		//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(pEffect->fRadius, pEffect->fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-pEffect->fRadius, -pEffect->fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->fRadius, -pEffect->fRadius, 0.0f);

			//頂点カラーの更新(ポリゴンの色の更新)
			pEffect->col.r -= EFF_COLOR * 0.01f;
			pEffect->col.g -= EFF_COLOR * 0.01f;
			pEffect->col.b -= EFF_COLOR * 0.01f;
			pEffect->col.a -= EFF_COLOR * 0.01f;

			//頂点カラーの設定
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			//寿命のカウントダウン
			if (pEffect->nLife <= 0)
			{//寿命を過ぎたら
				pEffect->bUse = false;			//使用していない状態にする
			}
			else
			{//寿命削り
				pEffect->nLife--;
			}
		}
		pVtx += 4;								//頂点データのポインタを4つ文進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの描画処理
void DrawEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;									//ビューマトリックス取得用

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntDraw = 0; nCntDraw < EFFECT_MAX; nCntDraw++)
	{
		if (g_aEffect[nCntDraw].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntDraw].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntDraw].mtxWorld, NULL, &mtxView);
			g_aEffect[nCntDraw].mtxWorld._41 = 0.0f;
			g_aEffect[nCntDraw].mtxWorld._42 = 0.0f;
			g_aEffect[nCntDraw].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntDraw].pos.x, g_aEffect[nCntDraw].pos.y, g_aEffect[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntDraw].mtxWorld, &g_aEffect[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntDraw].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				nCntDraw * 4,									//描画する最初のインデックス
				2);												//描画するプリミティブ数
		}
	}
	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1][256];
	int nCnterEffect = 0;
	LPD3DXFONT Font = GetFont();
	for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == true)
		{
			nCnterEffect++;
		}
	}

	//文字列に代入
	sprintf(&aStr[0][0], "\n\n\n\n\n\n\n\n\n\nエフェクトの数：%d", nCnterEffect);

	//テキスト描画
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius)
{
	int nCntEffect;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//弾が使用されていない
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nMaxLife = nLife;
			g_aEffect[nCntEffect].bUse = true;				//使用している状態にする

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);		//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}
		pVtx += 4;		//頂点データのポインタを4つ文進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
