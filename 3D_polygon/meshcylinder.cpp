//============================
//
//地面の処理(meshCylinder.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "meshcylinder.h"

//マクロ定義
#define MESHCYLINDER_MAX (1)			//シリンダーの数
#define MESHCYLWALL_MAX (8)				//壁の数
#define MESHCYLINDER_SIZEX (400.0f)		//半径の長さ
#define MESHCYLINDER_SIZEY (200.0f)		//半径の長さ
#define MESHCYLINDER_HEIGHT (100.0f)	//縦の長さ
#define MESHCYLINDER_SIZE (100.0f)		//シリンダーの半径
#define MESHCYLINDER_VTXMAX (18)		//頂点数
#define MESHCYLINDER_BLOCK_X (2)		//ブロック数x
#define MESHCYLINDER_BLOCK_Y (2)		//ブロック数y
#define MESHCYLINDER_VTX_X (9)			//頂点数x
#define MESHCYLINDER_VTX_Y (2)			//頂点数y
#define MESHCYLINDER_IDX (18)			//インデックス数
#define MESHCYLINDER_PLGN (12)			//ポリゴン数

//メッシュウォールの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//座標
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	bool bUse;
}Meshcylinder;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	//インデックスバッファへのポインタ
Meshcylinder g_posMeshCylinder[MESHCYLINDER_MAX];		//中心座標

//int g_nVtx = (MESHCYLINDER_BLOCK_X + 1) * (MESHCYLINDER_BLOCK_Z + 1);										//頂点数
//int g_nIdx = (1 + MESHCYLINDER_BLOCK_X * 2) * MESHCYLINDER_BLOCK_Z + (MESHCYLINDER_BLOCK_Z - 1) * 4;		//インデックス数
//int g_nPlgn = (MESHCYLINDER_BLOCK_X * MESHCYLINDER_BLOCK_Z * 2) + (MESHCYLINDER_BLOCK_Z - 1) * 4;			//ポリゴン数


//地面の初期化処理
void InitMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\murabitoUV012.png",				//テクスチャのファイル名
		&g_pTextureMeshCylinder);

	g_posMeshCylinder[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する
	g_posMeshCylinder[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する
	g_posMeshCylinder[0].bUse = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHCYLINDER_VTX_X * MESHCYLINDER_VTX_Y,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHCYLINDER_IDX * MESHCYLINDER_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//float fDevineX = MESHCYLINDER_SIZEX / MESHCYLINDER_BLOCK_X;
	//float fDevineY = MESHCYLINDER_SIZEY / MESHCYLINDER_BLOCK_Y;

	//for (int nCntPos = 0; nCntPos < MESHCYLINDER_MAX; nCntPos++)
	//{
	//	for (int nCntPos = 0; nCntPos < 2; nCntPos++)
	//	{
	//		pVtx[nCntPos].pos = D3DXVECTOR3(MESHCYLINDER_SIZEX, 0.0f, fDevineY);		//(x,y,z)
	//		fDevineY += fDevineY;
	//	}
	//}

	float frot = D3DX_PI / MESHCYLWALL_MAX;

	for (int nCntInit = 0; nCntInit < MESHCYLINDER_MAX; nCntInit++)
	{
		//g_posMeshCylinder[0].x + sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE;	//(x)

		//頂点座標の設定(x,y,z)
		//上
		pVtx[0].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[1].pos = D3DXVECTOR3( sinf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[2].pos = D3DXVECTOR3( sinf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[3].pos = D3DXVECTOR3( sinf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[4].pos = D3DXVECTOR3( sinf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[5].pos = D3DXVECTOR3( sinf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[6].pos = D3DXVECTOR3( sinf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[7].pos = D3DXVECTOR3( sinf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		pVtx[8].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									MESHCYLINDER_HEIGHT,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);
		
		//下
		pVtx[9].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[10].pos = D3DXVECTOR3( sinf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[11].pos = D3DXVECTOR3( sinf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[12].pos = D3DXVECTOR3( sinf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[13].pos = D3DXVECTOR3( sinf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(1.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[14].pos = D3DXVECTOR3( sinf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(-0.75f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[15].pos = D3DXVECTOR3( sinf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(-0.5f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[16].pos = D3DXVECTOR3( sinf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(-0.25f * D3DX_PI) * -MESHCYLINDER_SIZE);

		pVtx[17].pos = D3DXVECTOR3( sinf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE,
									0.0f,
									 cosf(0.0f * D3DX_PI) * -MESHCYLINDER_SIZE);

		//法線ベクトルの設定
		//頂点カラーの設定
		for (int nCntcol = 0; nCntcol < MESHCYLINDER_VTXMAX; nCntcol++)
		{
			//D3DXVECTOR3 pos = {};
			//pos -= pVtx[nCntcol].pos;
			pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//(x,y,z)

			D3DXVec3Normalize(&pVtx[nCntcol].nor, &pVtx[nCntcol].nor);		//ベクトルを正規化する(1.0fにする)

			pVtx[nCntcol].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
		//pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)

		//テクスチャ座標の設定
		pVtx[ 0].tex = D3DXVECTOR2(0.0f,	 0.0f);
		pVtx[ 1].tex = D3DXVECTOR2(0.125f,	 0.0f);
		pVtx[ 2].tex = D3DXVECTOR2(0.25f,	 0.0f);
		pVtx[ 3].tex = D3DXVECTOR2(0.375f,	 0.0f);
		pVtx[ 4].tex = D3DXVECTOR2(0.5f,	 0.0f);
		pVtx[ 5].tex = D3DXVECTOR2(0.625f,	 0.0f);
		pVtx[ 6].tex = D3DXVECTOR2(0.75f,	 0.0f);
		pVtx[ 7].tex = D3DXVECTOR2(0.875f,	 0.0f);
		pVtx[ 8].tex = D3DXVECTOR2(1.0f,	 0.0f);

		pVtx[9 ].tex = D3DXVECTOR2(0.0f,	 1.0f);
		pVtx[10].tex = D3DXVECTOR2(0.125f,	 1.0f);
		pVtx[11].tex = D3DXVECTOR2(0.25f,	 1.0f);
		pVtx[12].tex = D3DXVECTOR2(0.375f,	 1.0f);
		pVtx[13].tex = D3DXVECTOR2(0.5f,	 1.0f);
		pVtx[14].tex = D3DXVECTOR2(0.625f,	 1.0f);
		pVtx[15].tex = D3DXVECTOR2(0.75f,	 1.0f);
		pVtx[16].tex = D3DXVECTOR2(0.875f,	 1.0f);
		pVtx[17].tex = D3DXVECTOR2(1.0f,	 1.0f);

		pVtx += MESHCYLINDER_VTXMAX;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;		//インデックス情報へのポインタ

					//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MESHCYLINDER_MAX; nCntIdx++)
	{
		//頂点番号データの設定
		//外側
		//pIdx[0] = 0;
		//pIdx[1] = 9;
		//pIdx[2] = 1;
		//pIdx[3] = 10;
		//pIdx[4] = 2;
		//pIdx[5] = 11;
		//pIdx[6] = 3;
		//pIdx[7] = 12;
		//pIdx[8] = 4;
		//pIdx[9] = 13;
		//pIdx[10] = 5;
		//pIdx[11] = 14;
		//pIdx[12] = 6;
		//pIdx[13] = 15;
		//pIdx[14] = 7;
		//pIdx[15] = 16;
		//pIdx[16] = 8;
		//pIdx[17] = 17;

		//内側
		pIdx[0] = 9;
		pIdx[1] = 0;
		pIdx[2] = 10;
		pIdx[3] = 1;
		pIdx[4] = 11;
		pIdx[5] = 2;
		pIdx[6] = 12;
		pIdx[7] = 3;
		pIdx[8] = 13;
		pIdx[9] = 4;
		pIdx[10] = 14;
		pIdx[11] = 5;
		pIdx[12] = 15;
		pIdx[13] = 6;
		pIdx[14] = 16;
		pIdx[15] = 7;
		pIdx[16] = 17;
		pIdx[17] = 8;

		pIdx += MESHCYLINDER_IDX;
	}
	//インデックスバッファをアンロックする
	g_pIdxBuffMeshCylinder->Unlock();

	//SetMeshCylinder(D3DXVECTOR3(0.0f, 0.0f, MESHCYLINDER_SIZEX), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetMeshCylinder(D3DXVECTOR3(0.0f, 0.0f, -MESHCYLINDER_SIZEX), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetMeshCylinder(D3DXVECTOR3(MESHCYLINDER_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetMeshCylinder(D3DXVECTOR3(-MESHCYLINDER_SIZEX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
}

//地面の終了処理
void UninitMeshCylinder(void)
{
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;					//頂点バッファの破棄
	}
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;					//インデックスバッファの破棄
	}
}

//地面の更新処理
void UpdateMeshCylinder(void)
{

}

//地面の描画処理
void DrawMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

	for (int nCntDraw = 0; nCntDraw < MESHCYLINDER_MAX; nCntDraw++)
	{
		if (g_posMeshCylinder[nCntDraw].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_posMeshCylinder[nCntDraw].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_posMeshCylinder[nCntDraw].rot.y, g_posMeshCylinder[nCntDraw].rot.x, g_posMeshCylinder[nCntDraw].rot.z);

			D3DXMatrixMultiply(&g_posMeshCylinder[nCntDraw].mtxWorld, &g_posMeshCylinder[nCntDraw].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_posMeshCylinder[nCntDraw].pos.x, g_posMeshCylinder[nCntDraw].pos.y, g_posMeshCylinder[nCntDraw].pos.z);

			D3DXMatrixMultiply(&g_posMeshCylinder[nCntDraw].mtxWorld, &g_posMeshCylinder[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_posMeshCylinder[nCntDraw].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			//インデックスをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshCylinder);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				0,
				0,
				18,							//用意した頂点の数
				0,
				17);						//描画するプリミティブの数
		}
	}
}

////壁の設置処理
//void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nDevine)
//{
//	for (int nCntSet = 0; nCntSet < MESHCYLINDER_MAX; nCntSet++)
//	{
//		if (g_MeshCylinder[nCntSet].bUse == false)
//		{
//			g_MeshCylinder[nCntSet].pos = pos;
//			g_MeshCylinder[nCntSet].rot = rot;
//			g_MeshCylinder[nCntSet].bUse = true;
//			break;
//		}
//	}
//}
