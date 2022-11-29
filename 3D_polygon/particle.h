#ifndef _PARTICLE_H_		//このマクロ定義がされてなかったら
#define _PARTICLE_H_		//二重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, int nType);

#endif
