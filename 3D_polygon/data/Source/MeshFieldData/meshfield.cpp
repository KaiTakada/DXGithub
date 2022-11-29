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
#define MESHFIELD_SIZE (400.0f)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3 g_posMeshfield;							//位置
D3DXVECTOR3 g_rotMeshfield;							//移動量
D3DXMATRIX g_mtxWorldMeshfield;						//ワールドマトリックス

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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

							//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	0.0f);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	pVtx[2].pos = D3DXVECTOR3(0.0f,				0.0f,	0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,				0.0f,	MESHFIELD_SIZE);
	pVtx[4].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	0.0f);		//(x,y,z)
	pVtx[5].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	pVtx[6].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	MESHFIELD_SIZE);
	pVtx[7].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	pVtx[8].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	pVtx[9].pos = D3DXVECTOR3(-MESHFIELD_SIZE,	0.0f,	0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f,			0.0f,	-MESHFIELD_SIZE);		//(x,y,z)
	pVtx[11].pos = D3DXVECTOR3(0.0f,			0.0f,	0.0f);
	pVtx[12].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	-MESHFIELD_SIZE);
	pVtx[13].pos = D3DXVECTOR3(MESHFIELD_SIZE,	0.0f,	0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f , 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.0f , 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f , 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.5f , 0.0f);
	pVtx[4].tex = D3DXVECTOR2(1.0f , 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[10].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[12].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.5f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();
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
		g_pVtxBuffMeshfield = NULL;					//バッファの破棄
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

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//描画する最初のインデックス
		12);											//描画するプリミティブ数
}
