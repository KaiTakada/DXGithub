#ifndef _MODEL_H_								//このマクロ定義がされてなかったら
#define _MODEL_H_								//二重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
D3DXVECTOR3 GetModelpos(void);
D3DXVECTOR3 GetModelposDest(void);
D3DXVECTOR3 GetModelRot(void);

#endif
