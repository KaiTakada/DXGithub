#ifndef _BULLET_H_								//このマクロ定義がされてなかったら
#define _BULLET_H_								//二重インクルード防止のマクロを定義する

//弾の種類の列挙型
typedef enum
{
	BULLETTYPE_PLAYER = 0,						//プレイヤーの弾
	BULLETTYPE_MAX
}BULLETTYPE;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BULLETTYPE type);
void CollisionWall(int nCnt);

#endif