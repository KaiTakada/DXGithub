#ifndef _MESHWALL_H_								//このマクロ定義がされてなかったら
#define _MESHWALL_H_								//二重インクルード防止のマクロを定義する

//メッシュウォールの構造体
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 rot;							//移動量
	D3DXVECTOR3 size;							//大きさ
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	bool bUse;
}Meshwall;

//プロトタイプ宣言
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
Meshwall GetMeshWall(void);

#endif
