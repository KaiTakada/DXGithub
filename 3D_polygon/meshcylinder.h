#ifndef _MESHCYLINDER_H_								//このマクロ定義がされてなかったら
#define _MESHCYLINDER_H_								//二重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
//void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
