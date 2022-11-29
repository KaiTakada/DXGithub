//============================
//
//地面の処理(meshfield.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "meshfield.h"

//マクロ定義
#define MESHFIELD_SIZE (1600.0f)	//半径の長さ
#define MESHFIELD_MAX (9)			//頂点数
#define MESHFIELD_BLOCK_X (2)		//ブロック数x
#define MESHFIELD_BLOCK_Z (2)		//ブロック数z
#define MESHFIELD_VTX_X (3)			//頂点数x
#define MESHFIELD_VTX_Z (3)			//頂点数z
#define MESHFIELD_IDX (14)			//インデックス数
#define MESHFIELD_PLGN (12)			//ポリゴン数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;	//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshfield;							//位置
D3DXVECTOR3 g_rotMeshfield;							//移動量
D3DXMATRIX g_mtxWorldMeshfield;						//ワールドマトリックス
int g_nVtx = (MESHFIELD_BLOCK_X + 1) * (MESHFIELD_BLOCK_Z + 1);									//頂点数
int g_nIdx = (1 + MESHFIELD_BLOCK_X * 2) * MESHFIELD_BLOCK_Z + (MESHFIELD_BLOCK_Z - 1) * 4;		//インデックス数
int g_nPlgn = (MESHFIELD_BLOCK_X * MESHFIELD_BLOCK_Z * 2) + (MESHFIELD_BLOCK_Z - 1) * 4;		//ポリゴン数

//地面の初期化処理
void InitMeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stone000.jpg",				//テクスチャのファイル名
		&g_pTextureMeshfield);

	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VTX_X * MESHFIELD_VTX_Z,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshfield,
								NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	float fDevineX = MESHFIELD_SIZE / MESHFIELD_BLOCK_X;
	float fDevineZ = MESHFIELD_SIZE / MESHFIELD_BLOCK_Z;
	float fMaxX = -MESHFIELD_SIZE;
	float fMaxZ = MESHFIELD_SIZE;
	D3DXVECTOR3 pos[9] = {};

	for (int nCntPosX = 0; nCntPosX < MESHFIELD_VTX_X; nCntPosX++)
	{
		for (int nCntPosZ = 0; nCntPosZ < MESHFIELD_VTX_Z; nCntPosZ++)
		{
			pVtx[nCntPosX * MESHFIELD_VTX_X + nCntPosZ].pos = D3DXVECTOR3(fMaxX, 0.0f, fMaxZ);		//(x,y,z)
			pos[nCntPosX * MESHFIELD_VTX_X + nCntPosZ] = D3DXVECTOR3(fMaxX, 0.0f, fMaxZ);

			fMaxX += MESHFIELD_SIZE;

			if (fMaxX > MESHFIELD_SIZE)
			{
				fMaxX = -MESHFIELD_SIZE;
			}
		}
		fMaxZ -= MESHFIELD_SIZE;
	}

	////頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);		//(x,y,z)
	//pVtx[1].pos = D3DXVECTOR3(0.0f,				0.0f,	MESHFIELD_SIZE);
	//pVtx[2].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	//pVtx[3].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	0.0f);
	//pVtx[4].pos = D3DXVECTOR3(0.0f,				0.0f,	0.0f);		//(x,y,z)
	//pVtx[5].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	0.0f);
	//pVtx[6].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	//pVtx[7].pos = D3DXVECTOR3(0.0f,				0.0f,	-MESHFIELD_SIZE);
	//pVtx[8].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);

	//法線ベクトルの設定
	//頂点カラーの設定
	for (int nCntcol = 0;nCntcol < MESHFIELD_MAX;nCntcol++)
	{
		pVtx[nCntcol].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCntcol].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f , 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f , 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f , 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f , 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f , 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f , 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f , 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f , 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();

	WORD *pIdx;		//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshfield->Lock(0,0,(void**)&pIdx,0);

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

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshfield->Unlock();
}

//地面の終了処理
void UninitMeshfield(void)
{
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;					//頂点バッファの破棄
	}
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;					//インデックスバッファの破棄
	}
}

//地面の更新処理
void UpdateMeshfield(void)
{

}

//地面の描画処理
void DrawMeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

														//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//インデックスをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshfield);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
									0,
									0,
									9,							//用意した頂点の数
									0,
									12);						//描画するプリミティブの数
}
