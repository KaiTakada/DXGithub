#ifndef _ENEMY_H_								//このマクロ定義がされてなかったら
#define _ENEMY_H_								//二重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);

#endif
