#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256)		//キーの最大数
#define NUM_PAD_MAX (4)			//PAD使用数

//グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;					//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//入力デバイスへのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];					//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報
XINPUT_STATE g_PadState[NUM_PAD_MAX];			//PADのプレス情報
XINPUT_STATE g_PadStateTrigger[NUM_PAD_MAX];	//PADのトリガー情報

SHORT g_Sticktate[NUM_PAD_MAX];			//PADのプレス情報
SHORT g_StickStateTrigger[NUM_PAD_MAX];	//PADのトリガー情報

//======================================
//キーボードの初期化処理
//======================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットの設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//======================================
//キーボード終了処理
//======================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();				//キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//入力デバイス(キーボード)の破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//======================================
//入力デバイスからデータ取得
//======================================
void UpdateKeyboard(void)
{
 	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータ取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState),&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//キーボードのトリガー情報を保有
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保有
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();						//キーボードへのアクセス権を獲得
	}
}

//======================================
//キーボードのプレス情報を取得
//======================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================
//キーボードのトリガー情報を取得
//======================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================
//PAD入力処理
//======================================
HRESULT InitGamepad(void)
{
	int nCnt;
	XInputEnable(true);

	for (nCnt = 0;nCnt < NUM_PAD_MAX;nCnt++)
	{
		//メモリをクリア
		memset(&g_PadState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_PadStateTrigger[nCnt],0,sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//======================================
//PAD終了処理
//======================================
void UninitGamepad(void)
{
	XInputEnable(false);
}

//======================================
//入力デバイスからデータ取得
//======================================
void UpdateGamepad(void)
{
	XINPUT_STATE PadState[NUM_PAD_MAX];		//ゲームパッドの入力情報
	static int nCntSpace = 0;					//スペース
	int nCnt = 0;

	for (nCnt = 0; nCnt < NUM_PAD_MAX; nCnt++)
	{
		if (XInputGetState(nCnt,&PadState[nCnt]) == ERROR_SUCCESS)
		{
			g_PadStateTrigger[nCnt].Gamepad.wButtons = 
				~ g_PadState[nCnt].Gamepad.wButtons 
				& PadState[nCnt].Gamepad.wButtons;		//PADのトリガー情報を保有

			g_PadState[nCnt] = PadState[nCnt];			//PADのプレス情報を保有
		}
	}
}

//======================================
//PADのプレス情報を取得
//======================================
bool GetPadPress(PADBUTTON button, int nCntPad)
{
	return (g_PadState[nCntPad].Gamepad.wButtons & (0x01 << button)) ? true : false;
}

//======================================
//PADのトリガー情報を取得
//======================================
bool GetPadTrigger(PADBUTTON button, int nCntPad)
{
	return (g_PadStateTrigger[nCntPad].Gamepad.wButtons & (0x01 << button)) ? true : false;
}

//======================================
//PADのLスティックのXのプレス情報を取得
//======================================
SHORT GetGameStickLXPress(PADBUTTON Key, int nPlayer)
{
	return g_PadState[nPlayer].Gamepad.sThumbLX;
}

//======================================
//PADのLスティックのYのプレス情報を取得
//======================================
SHORT GetGameStickLYPress(PADBUTTON Key, int nPlayer)
{
	return g_PadState[nPlayer].Gamepad.sThumbLY;
}

//======================================
//PADのLスティックのXのトリガー情報を取得
//======================================
SHORT GetGameStickLXTrigger(PADBUTTON Key, int nPlayer)
{
	return g_PadStateTrigger[nPlayer].Gamepad.sThumbLX;
}

//======================================
//PADのLスティックのYのトリガー情報を取得
//======================================
SHORT GetGameStickLYTrigger(PADBUTTON Key, int nPlayer)
{
	return g_PadStateTrigger[nPlayer].Gamepad.sThumbLY;
}