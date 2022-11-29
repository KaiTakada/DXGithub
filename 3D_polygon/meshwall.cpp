//============================
//
//地面の処理(meshWall.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "meshwall.h"

//マクロ定義
#define MESHWALL_MAX (4)			//壁の数
#define MESHWALL_SIZEX (1600.0f)	//半径の長さ
#define MESHWALL_SIZEY (200.0f)		//半径の長さ
#define MESHWALL_VTXMAX (9)			//頂点数
#define MESHWALL_BLOCK_X (2)		//ブロック数x
#define MESHWALL_BLOCK_Y (2)		//ブロック数y
#define MESHWALL_VTX_X (3)			//頂点数x
#define MESHWALL_VTX_Y (3)			//頂点数y
#define MESHWALL_IDX (14)			//インデックス数
#define MESHWALL_PLGN (12)			//ポリゴン数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;	//インデックスバッファへのポインタ
Meshwall g_MeshWall[MESHWALL_MAX];					//メッシュウォールの情報
//int g_nVtx = (MESHWALL_BLOCK_X + 1) * (MESHWALL_BLOCK_Z + 1);									//頂点数
//int g_nIdx = (1 + MESHWALL_BLOCK_X * 2) * MESHWALL_BLOCK_Z + (MESHWALL_BLOCK_Z - 1) * 4;		//インデックス数
//int g_nPlgn = (MESHWALL_BLOCK_X * MESHWALL_BLOCK_Z * 2) + (MESHWALL_BLOCK_Z - 1) * 4;			//ポリゴン数


//地面の初期化処理
void InitMeshWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall001.jpg",				//テクスチャのファイル名
		&g_pTextureMeshWall);

	for (int nCntGrb = 0; nCntGrb < MESHWALL_MAX; nCntGrb++)
	{
		g_MeshWall[nCntGrb].pos = D3DXVECTOR3(0.0f, MESHWALL_SIZEY, MESHWALL_SIZEY * nCntGrb);	//初期化する
		g_MeshWall[nCntGrb].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する
		g_MeshWall[nCntGrb].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する
		g_MeshWall[nCntGrb].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWALL_VTX_X * MESHWALL_VTX_Y * MESHWALL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHWALL_IDX * MESHWALL_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	//float fDevineX = MESHWALL_SIZEX / MESHWALL_BLOCK_X;
	//float fDevineY = MESHWALL_SIZEY / MESHWALL_BLOCK_Y;

	//for (int nCntPos = 0; nCntPos < MESHWALL_MAX; nCntPos++)
	//{
	//	for (int nCntPos = 0; nCntPos < 2; nCntPos++)
	//	{
	//		pVtx[nCntPos].pos = D3DXVECTOR3(MESHWALL_SIZEX, 0.0f, fDevineY);		//(x,y,z)
	//		fDevineY += fDevineY;
	//	}
	//}

	for (int nCntInit = 0; nCntInit < MESHWALL_MAX; nCntInit++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-MESHWALL_SIZEX,	MESHWALL_SIZEY, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(0.0f,				MESHWALL_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(MESHWALL_SIZEX,	MESHWALL_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(-MESHWALL_SIZEX,	MESHWALL_SIZEY * 0.5, 0.0f);
		pVtx[4].pos = D3DXVECTOR3(0.0f,				MESHWALL_SIZEY * 0.5, 0.0f);		//(x,y,z)
		pVtx[5].pos = D3DXVECTOR3(MESHWALL_SIZEX,	MESHWALL_SIZEY * 0.5, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(-MESHWALL_SIZEX,	0.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(0.0f,				0.0f, 0.0f);
		pVtx[8].pos = D3DXVECTOR3(MESHWALL_SIZEX,	0.0f, 0.0f);

		//法線ベクトルの設定
		//頂点カラーの設定
		for (int nCntcol = 0; nCntcol < MESHWALL_VTXMAX; nCntcol++)
		{
			pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntcol].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)

		//pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	
		pVtx += MESHWALL_VTXMAX;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffMeshWall->Unlock();

	WORD *pIdx;		//インデックス情報へのポインタ

					//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MESHWALL_MAX; nCntIdx++)
	{
		//頂点番号データの設定
		pIdx[0] =  3;
		pIdx[1] =  0;
		pIdx[2] =  4;
		pIdx[3] =  1;
		pIdx[4] =  5;
		pIdx[5] =  2;
		pIdx[6] =  2;
		pIdx[7] =  6;
		pIdx[8] =  6;
		pIdx[9] =  3;
		pIdx[10] = 7;
		pIdx[11] = 4;
		pIdx[12] = 8;
		pIdx[13] = 5;
	
		pIdx += MESHWALL_IDX;
	}
	//インデックスバッファをアンロックする
	g_pIdxBuffMeshWall->Unlock();

	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, MESHWALL_SIZEX), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -MESHWALL_SIZEX), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetMeshWall(D3DXVECTOR3(MESHWALL_SIZEX, 0.0f,  0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	SetMeshWall(D3DXVECTOR3(-MESHWALL_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
}

//地面の終了処理
void UninitMeshWall(void)
{
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;					//頂点バッファの破棄
	}
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;					//インデックスバッファの破棄
	}
}

//地面の更新処理
void UpdateMeshWall(void)
{

}

//地面の描画処理
void DrawMeshWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

	for (int nCntDraw = 0; nCntDraw < MESHWALL_MAX; nCntDraw++)
	{
		if (g_MeshWall[nCntDraw].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_MeshWall[nCntDraw].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_MeshWall[nCntDraw].rot.y, g_MeshWall[nCntDraw].rot.x, g_MeshWall[nCntDraw].rot.z);

			D3DXMatrixMultiply(&g_MeshWall[nCntDraw].mtxWorld, &g_MeshWall[nCntDraw].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_MeshWall[nCntDraw].pos.x, g_MeshWall[nCntDraw].pos.y, g_MeshWall[nCntDraw].pos.z);

			D3DXMatrixMultiply(&g_MeshWall[nCntDraw].mtxWorld, &g_MeshWall[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshWall[nCntDraw].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//インデックスをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				0,
				0,
				9,							//用意した頂点の数
				0,
				12);						//描画するプリミティブの数
		}
	}
}

//壁の設置処理
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntSet = 0; nCntSet < MESHWALL_MAX; nCntSet++)
	{
		if (g_MeshWall[nCntSet].bUse == false)
		{
			g_MeshWall[nCntSet].pos = pos;
			g_MeshWall[nCntSet].rot = rot;
			g_MeshWall[nCntSet].bUse = true;
			break;
		}
	}
}

//壁の情報を渡す
Meshwall GetMeshWall(void)
{
	return g_MeshWall[0];
}
