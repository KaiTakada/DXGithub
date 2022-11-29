#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

//ライブラリのリンク
#pragma comment(lib,"xinput.lib")				//Gamepad入力処理に必要

//ボタンの列挙型
typedef enum
{
	PADBUTTON_0 = 0,			//上ボタン
	PADBUTTON_1,				//下ボタン
	PADBUTTON_2,				//左ボタン
	PADBUTTON_3,				//右ボタン
	PADBUTTON_4,				//スタートボタン
	PADBUTTON_5,				//バックボタン
	PADBUTTON_6,				//Lスティック押し込み
	PADBUTTON_7,				//Rスティック押し込み
	PADBUTTON_8,				//LBボタン
	PADBUTTON_9,				//RBボタン
	PADBUTTON_10,				//スティックトリガー？
	PADBUTTON_11,				//スティックトリガー？
	PADBUTTON_12,				//Aボタン
	PADBUTTON_13,				//Bボタン
	PADBUTTON_14,				//Yボタン
	PADBUTTON_15,				//Xボタン
	PADBUTTON_16,				//Lスティック左右プレス情報
	PADBUTTON_17,				//Lスティック斜め上下プレス
	PADBUTTON_18,				//↑同じ？
	PADBUTTON_MAX
}PADBUTTON;

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance,HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);

HRESULT InitGamepad(void);
void UninitGamepad(void);
void UpdateGamepad(void);
bool GetPadPress(PADBUTTON button,int nCntPad);
bool GetPadTrigger(PADBUTTON button,int nCntPad);

SHORT GetGameStickLXPress(PADBUTTON Key, int nPlayer);
SHORT GetGameStickLYPress(PADBUTTON Key, int nPlayer);
SHORT GetGameStickLXTrigger(PADBUTTON Key, int nPlayer);
SHORT GetGameStickLYTrigger(PADBUTTON Key, int nPlayer);

#endif