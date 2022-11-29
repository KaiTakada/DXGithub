#ifndef _WALL_H_								//このマクロ定義がされてなかったら
#define _WALL_H_								//二重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif