//============================================
//パーティカルの処理(particle.cpp)
//Author:Kai Takada
//============================================
#include "main.h"
#include "particle.h"
#include "effect.h"
#include <time.h>

//マクロ定義
#define PARTICLE_MAX (256)			//パーティカルの最大数
#define PARTICLE_EFFECT (16)		//エフェクトの生成数
#define PARTICLE_RADIUS (10.0f)		//エフェクトの半径
#define PARTICLE_LIFE (10)			//パーティクルの寿命
#define PARTICLE_EFFECTLIFE (180)	//エフェクトの寿命

//パーティクルの列挙型
typedef enum
{
	PARTICLETYPE_EXPLOSION = 0,		//爆発の時用
	PARTICLETYPE_MAX,
}PARTICLETYPE;

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//座標
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	float fRadius;			//半径
	int nLife;				//寿命
	int nEffectLife;		//寿命
	bool bUse;				//使われているか？
}Particle;

//グローバル変数宣言
Particle g_aParticle[PARTICLE_MAX];						//パーティクルの情報

//パーティカルの初期化処理
void InitParticle(void)
{
	srand((unsigned int)time(NULL));

	//エフェクトの情報を初期化する
	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].fRadius = PARTICLE_RADIUS;
		g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;
		g_aParticle[nCntParticle].bUse = false;			//使用していない状態にする
	}
}

//パーティカルの終了処理
void UninitParticle(void)
{

}

//パーティカルの更新処理
void UpdateParticle(void)
{
	for (int nCntUpdate = 0; nCntUpdate < PARTICLE_MAX; nCntUpdate++)
	{
		if (g_aParticle[nCntUpdate].bUse == true)
		{//パーティクルが使用されている
			//パーティクルの生成
			for (int nCntParticle = 0; nCntParticle < PARTICLE_EFFECT; nCntParticle++)
			{
				g_aParticle[nCntParticle].pos;
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
				g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
				g_aParticle[nCntParticle].col;
				g_aParticle[nCntParticle].fRadius;
				g_aParticle[nCntParticle].nLife;
				g_aParticle[nCntParticle].nEffectLife;

				//エフェクトの設定
				SetEffect(g_aParticle[nCntUpdate].pos, g_aParticle[nCntUpdate].move, g_aParticle[nCntUpdate].col, g_aParticle[nCntUpdate].nEffectLife, g_aParticle[nCntUpdate].fRadius);
			}

			//寿命削り
			g_aParticle[nCntUpdate].nLife--;

			if (g_aParticle[nCntUpdate].nLife <= 0)
			{//寿命が尽きたら死ぬ
				g_aParticle[nCntUpdate].bUse = false;
			}
		}
	}
}

//パーティカルの描画処理
void DrawParticle(void)
{

}

//パーティカルの設定処理
void SetParticle(D3DXVECTOR3 pos, int nType)
{
	for (int nCntSet = 0;nCntSet < PARTICLE_MAX;nCntSet++)
	{
		if (g_aParticle[nCntSet].bUse == false)
		{
			g_aParticle[nCntSet].pos = pos;

			switch (nType)
			{
			case PARTICLETYPE_EXPLOSION:
				g_aParticle[nCntSet].move = D3DXVECTOR3(1.0f,1.0f,1.0f);
				g_aParticle[nCntSet].col = D3DXCOLOR(0.5f, 0.1f, 0.1f, 1.0f);
				g_aParticle[nCntSet].fRadius = PARTICLE_RADIUS;
				g_aParticle[nCntSet].nLife = PARTICLE_LIFE;
				g_aParticle[nCntSet].nEffectLife = PARTICLE_EFFECTLIFE;
				break;
			}

			g_aParticle[nCntSet].bUse = true;
			break;
		}
	}
}
