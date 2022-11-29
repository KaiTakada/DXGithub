#ifndef _EFFECT_H_								//このマクロ定義がされてなかったら
#define _EFFECT_H_								//二重インクルード防止のマクロを定義する

#include "main.h"

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius);

#endif
