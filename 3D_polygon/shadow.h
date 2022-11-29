#ifndef _SHADOW_H_								//このマクロ定義がされてなかったら
#define _SHADOW_H_								//二重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);		//設定処理
void SetPositionShadow(int nIdxShadow,D3DXVECTOR3 pos);		//位置設定処理
void SetShadowEnable(int nIdxShadow);							//使用切り替え

#endif