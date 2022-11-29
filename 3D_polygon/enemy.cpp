//============================
//
//敵の処理(enemy.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "enemy.h"
#include "camera.h"
#include "bullet.h"
#include "shadow.h"
#include "input.h"

//マクロ定義
#define ENEMY_MAX (2)							//敵の数
#define ENEMY_TECMAX (10)						//テクスチャの数
#define ENEMY_SPEED (0.25f)						//敵の移動スピード
#define ADULTWALL (1600.0f - 15.0f)				//大人の壁定期"
#define STALK_COUNT (100)						//回り込みカウンター

//敵の列挙型
typedef enum
{
	ENEMYTYPE_CHAIR = 0,
	ENEMYTYPE_BATTAN,
	ENEMYTYPE_MAX
}ENEMYTYPE;

//敵の構造体
typedef struct
{
	D3DXVECTOR3 pos;							//現在の位置
	D3DXVECTOR3 posDest;						//予定の位置
	D3DXVECTOR3 move;							//移動量
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3 vtxMinModel, vtxMaxModel;		//敵の最小値・最大値
	DWORD dwNumMatEnemy = 0;					//マテリアルの数
	int nIdxShadowEnemy = -1;					//対象の影のインデックス(番号)
}Enemy;

//xファイル名
const char *c_apFilenameEnemy[] =
{
	"data\\MODEL\\chair000.x",
	"data\\MODEL\\battan.x",
};

//グローバル変数宣言
LPD3DXMESH g_pMeshEnemy[ENEMY_MAX] = {};		//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatEnemy[ENEMY_MAX] = {};		//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEnemy[ENEMY_MAX][ENEMY_TECMAX] = {};		//テクスチャへのポインタ
Enemy g_enemy[ENEMY_MAX];	//敵の情報

//敵の初期化処理
void InitEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntInit = 0; nCntInit < ENEMY_MAX; nCntInit++)
	{
		g_enemy[0].pos = D3DXVECTOR3(-200.0f, 10.0f, 0.0f);		//初期化する
		g_enemy[1].pos = D3DXVECTOR3(-300.0f, 0.0f, 200.0f);		//初期化する
		g_enemy[nCntInit].posDest = D3DXVECTOR3(0.0f, 10.0f, 100.0f);	//初期化する
		g_enemy[nCntInit].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//初期化する
		g_enemy[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//初期化する
		g_enemy[nCntInit].vtxMinModel = {};
		g_enemy[nCntInit].vtxMaxModel = {};


		//Xファイルの読み込み
		D3DXLoadMeshFromX(c_apFilenameEnemy[nCntInit],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatEnemy[nCntInit],
			NULL,
			&g_enemy[nCntInit].dwNumMatEnemy,
			&g_pMeshEnemy[nCntInit]);

		//頂点数を取得
		nNumVtx = g_pMeshEnemy[nCntInit]->GetNumVertices();

		//頂点フォーマットのサイズの取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy[nCntInit]->GetFVF());

		D3DXMATERIAL *pMat;	//マテリアルへのポインタ

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[nCntInit]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_enemy[nCntInit].dwNumMatEnemy; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
			 //テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,				//テクスチャのファイル名
					&g_pTextureEnemy[nCntInit][nCntMat]);
			}
		}

		//頂点バッファをロック
		g_pMeshEnemy[nCntInit]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

			//すべての頂点を比較して敵の最小値・最大値を抜き出す
			if (g_enemy[nCntInit].vtxMaxModel.x < vtx.x)
			{
				g_enemy[nCntInit].vtxMaxModel.x = vtx.x;
			}
			if (g_enemy[nCntInit].vtxMaxModel.y < vtx.y)
			{
				g_enemy[nCntInit].vtxMaxModel.y = vtx.y;
			}
			if (g_enemy[nCntInit].vtxMaxModel.z < vtx.z)
			{
				g_enemy[nCntInit].vtxMaxModel.z = vtx.z;
			}

			if (g_enemy[nCntInit].vtxMinModel.x > vtx.x)
			{
				g_enemy[nCntInit].vtxMinModel.x = vtx.x;
			}
			if (g_enemy[nCntInit].vtxMinModel.y > vtx.y)
			{
				g_enemy[nCntInit].vtxMinModel.y = vtx.y;
			}
			if (g_enemy[nCntInit].vtxMinModel.z > vtx.z)
			{
				g_enemy[nCntInit].vtxMinModel.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;		//頂点フォーマットのサイズ分ポインタを進める
		}
		//頂点バッファをアンロック
		g_pMeshEnemy[nCntInit]->UnlockVertexBuffer();

		//影を設定
		g_enemy[nCntInit].nIdxShadowEnemy = SetShadow();
	}
}

//敵の終了処理
void UninitEnemy(void)
{
	//if (g_pTextureEnemy != NULL)
	//{
	//	g_pTextureEnemy->Release();
	//	g_pTextureEnemy = NULL;					//テクスチャの破棄
	//}
	for (int nCntUninit = 0;nCntUninit < ENEMY_MAX;nCntUninit++)
	{
		if (g_pMeshEnemy[nCntUninit] != NULL)
		{
			g_pMeshEnemy[nCntUninit]->Release();
			g_pMeshEnemy[nCntUninit] = NULL;					//メッシュの破棄
		}
		if (g_pBuffMatEnemy[nCntUninit] != NULL)
		{
			g_pBuffMatEnemy[nCntUninit]->Release();
			g_pBuffMatEnemy[nCntUninit] = NULL;					//バッファの破棄
		}
	}
}

//敵の更新処理
void UpdateEnemy(void)
{
	for (int nCntUpdate = 0; nCntUpdate < ENEMY_MAX; nCntUpdate++)
	{
		//影の位置を設定
		SetPositionShadow(g_enemy[nCntUpdate].nIdxShadowEnemy, g_enemy[nCntUpdate].pos);
	}
}
	

//敵の描画処理
void DrawEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	for (int nCntDraw = 0; nCntDraw < ENEMY_MAX; nCntDraw++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_enemy[nCntDraw].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_enemy[nCntDraw].rot.y, g_enemy[nCntDraw].rot.x, g_enemy[nCntDraw].rot.z);

		D3DXMatrixMultiply(&g_enemy[nCntDraw].mtxWorld, &g_enemy[nCntDraw].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_enemy[nCntDraw].pos.x, g_enemy[nCntDraw].pos.y, g_enemy[nCntDraw].pos.z);

		D3DXMatrixMultiply(&g_enemy[nCntDraw].mtxWorld, &g_enemy[nCntDraw].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_enemy[nCntDraw].mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[nCntDraw]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_enemy[nCntDraw].dwNumMatEnemy; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemy[nCntDraw][nCntMat]);

			//敵(パーツ)の描画
			g_pMeshEnemy[nCntDraw]->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//当たり判定
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	for (int nCntCol = 0;nCntCol < ENEMY_MAX;nCntCol++)
	{
		if ((g_enemy[nCntCol].vtxMaxModel.x > posOld.x || g_enemy[nCntCol].vtxMinModel.x < posOld.x ) &&
			(g_enemy[nCntCol].vtxMaxModel.x < pPos->x && g_enemy[nCntCol].vtxMinModel.x > pPos->x))
		{//z軸
			pPos->x = posOld.x;
		}
		if ((g_enemy[nCntCol].vtxMaxModel.z > posOld.x || g_enemy[nCntCol].vtxMinModel.x < posOld.z) &&
			g_enemy[nCntCol].vtxMaxModel.z < pPos->x && g_enemy[nCntCol].vtxMinModel.x > pPos->z)
		{//x軸
			pPos->z = posOld.z;
		}
	}
}
