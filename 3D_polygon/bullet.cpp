//============================
//
//ビルボードの処理(bullet.cpp)
// Author: Kai Takada
//
//============================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "bullet.h"
#include "shadow.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "meshwall.h"

//マクロ定義
#define BULLET_SPEED (1.0f)		//弾のスピード
#define BULLET_SIZE (5.0f)		//弾の半径
#define BULLET_MAX (256)		//弾の最大数

//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//現在の座標
	D3DXVECTOR3 posOld;		//前回の座標
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 vecMove;	//移動ベクトル
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nLife;				//寿命
	BULLETTYPE type;		//弾の種類
	bool bUse;				//使用中か否か
	int nIdxShadow = -1;	//対象の影のインデックス(番号)
}Bullet;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[BULLET_MAX];

//弾の初期化処理
void InitBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",				//テクスチャのファイル名
		&g_pTextureBullet);

	for (int nCntGro = 0;nCntGro < BULLET_MAX;nCntGro++)
	{//初期化する
		g_aBullet[nCntGro].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntGro].posOld = g_aBullet[nCntGro].pos;
		g_aBullet[nCntGro].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntGro].vecMove = g_aBullet[nCntGro].rot;
		g_aBullet[nCntGro].nLife = 300;
		g_aBullet[nCntGro].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntGro].bUse = false;
	}
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BULLET_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntInit = 0;nCntInit < BULLET_MAX;nCntInit++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE, BULLET_SIZE, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE, -BULLET_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE, -BULLET_SIZE, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//弾の終了処理
void UninitBullet(void)
{
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;					//バッファの破棄
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				SetEffect(g_aBullet[nCntBullet].pos,D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR(0.3f, 0.4f, 1.0f, 1.0f), 120, 10.0f);
				//CollisionEnemy(pBullet);
			}

			//頂点座標の更新
			g_aBullet[nCntBullet].pos.x += sinf(g_aBullet[nCntBullet].rot.y + 1.0f * D3DX_PI) * BULLET_SPEED;		//x
			g_aBullet[nCntBullet].pos.z += cosf(g_aBullet[nCntBullet].rot.y + 1.0f * D3DX_PI) * BULLET_SPEED;		//y

			//壁との当たり判定
			CollisionWall(nCntBullet);

			//影の位置を設定
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			g_aBullet[nCntBullet].nLife--;
			//寿命のカウントダウン
			if (g_aBullet[nCntBullet].nLife <= 0)
			{//寿命を過ぎたら
				g_aBullet[nCntBullet].bUse = false;			//使用していない状態にする
			}
			//else if (g_aBullet[nCntBullet].pos.x >= 400.0f || g_aBullet[nCntBullet].pos.x <= -400.0f ||
			//	g_aBullet[nCntBullet].pos.z >= 400.0f || g_aBullet[nCntBullet].pos.z <= -400.0f)
			//{//大人の壁に当たったら
			//	g_aBullet[nCntBullet].bUse = false;			//使用していない状態にする
			//}

			if (g_aBullet[nCntBullet].bUse == false)
			{//死んだら
				SetShadowEnable(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos);
				SetParticle(g_aBullet[nCntBullet].pos, 0);
			}
		}
	}
}

//弾の描画処理
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;									//ビューマトリックス取得用

	for (int nCntDraw = 0; nCntDraw < BULLET_MAX; nCntDraw++)
	{
		if (g_aBullet[nCntDraw].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntDraw].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntDraw].mtxWorld, NULL, &mtxView);
			g_aBullet[nCntDraw].mtxWorld._41 = 0.0f;
			g_aBullet[nCntDraw].mtxWorld._42 = 0.0f;
			g_aBullet[nCntDraw].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntDraw].pos.x, g_aBullet[nCntDraw].pos.y, g_aBullet[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntDraw].mtxWorld, &g_aBullet[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntDraw].mtxWorld);

			//Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				0,												//描画する最初のインデックス
				2);												//描画するプリミティブ数

			//Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
		}
	}

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1][256];
	int nCnterBullet = 0;
	LPD3DXFONT Font = GetFont();
	for (int nCnt = 0;nCnt < BULLET_MAX;nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			nCnterBullet++;
		}
	}

	//文字列に代入
	sprintf(&aStr[0][0], "\n\n\n\n\n\n\n\n\n弾の数：%d", nCnterBullet);

	//テキスト描画
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================
//弾の設定処理
//=============================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBullet].pos = pos;				//座標を代入
			g_aBullet[nCntBullet].rot = rot;				//向きを代入
			g_aBullet[nCntBullet].type = type;				//種類を代入
			g_aBullet[nCntBullet].bUse = true;				//使用している状態にする

			switch (g_aBullet[nCntBullet].type)
			{
			case BULLETTYPE_PLAYER:
				g_aBullet[nCntBullet].nLife = 300;
				break;
			}
			//影を設定
			g_aBullet[nCntBullet].nIdxShadow = SetShadow();
			break;
		}
	}
}

//壁との当たり判定
void CollisionWall(int nCntBullet)
{
	//マクロ
#define WALL_MAX (4)

////壁の情報をもらう
//Meshwall *pMeshWall = &GetMeshWall();

	D3DXVECTOR3 Wallpos0;
	D3DXVECTOR3 Wallpos1;
	D3DXVECTOR3 vecTopos;
	D3DXVECTOR3 vecLine;

	for (int nCntcpy = 0; nCntcpy < WALL_MAX; nCntcpy++)
	{
		switch (nCntcpy)
		{
		case 0:
			Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
			Wallpos1 = D3DXVECTOR3(-400.0f,0.0f,400.0f);
			
			vecLine = Wallpos1 - Wallpos0;
			vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

			if ((vecLine.z * vecTopos.x)
				- (vecLine.x * vecTopos.z) >= 0.0f)
			{
				g_aBullet[nCntBullet].bUse = false;			//使用していない状態にする
			}
			break;

		//case 1:
		//	Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
		//	Wallpos1 = D3DXVECTOR3(-400.0f, 0.0f, 400.0f);

		//	vecLine = Wallpos1 - Wallpos0;
		//	vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

		//	if ((vecLine.z * vecTopos.x)
		//		- (vecLine.x * vecTopos.z) >= 0.0f)
		//	{
		//		g_aBullet[nCntBullet].bUse = false;			//使用していない状態にする
		//	}
		//	break;

		//case 2:
		//	Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
		//	Wallpos1 = D3DXVECTOR3(-400.0f, 0.0f, 400.0f);

		//	vecLine = Wallpos1 - Wallpos0;
		//	vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

		//	if ((vecLine.z * vecTopos.x)
		//		- (vecLine.x * vecTopos.z) >= 0.0f)
		//	{
		//		g_aBullet[nCntBullet].bUse = false;			//使用していない状態にする
		//	}
		//	break;
		//
		//case 3:
		//	Wallpos0 = D3DXVECTOR3(400.0, 0.0f, 400.0f);
		//	Wallpos1 = D3DXVECTOR3(-400.0f, 0.0f, 400.0f);

		//	vecLine = Wallpos1 - Wallpos0;
		//	vecTopos = g_aBullet[nCntBullet].pos - Wallpos0;

		//	if ((vecLine.z * vecTopos.x)
		//		- (vecLine.x * vecTopos.z) >= 0.0f)
		//	{
		//		g_aBullet[nCntBullet].bUse = false;			//使用していない状態にする
		//	}
		//	break;
		}

	}
}
