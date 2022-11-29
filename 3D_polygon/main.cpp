//============================
//
//DirectX
//コメント確認すべし。
//
//Author 髙田 佳依
//
//============================
#include "main.h"
#include "stdio.h"
#include "string.h"

#include "input.h"
#include "enemy.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "bullet.h"
#include "wall.h"
#include "particle.h"
#include "effect.h"
#include "explosion.h"
#include "shadow.h"
#include "field.h"
#include "meshcylinder.h"
#include "meshwall.h"
#include "meshfield.h"
#include "billboard.h"

//マクロ定義
#define CLASS_NAME "WindowClass"				//ウィンドウクラスの名前
#define WINDOW_NAME "DirectX"					//ウィンドウの名前(キャプションに表示)
#define ID_TIMER (131)							//タイマーのID
#define TIMER_INTERVAL (1000/60)				//タイマーの発生間隔(ミリ秒)
#define NOT_DATA (0)							//入力枠

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init (HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;						//Dierct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Dierct3Dデバイスへのポインタ
VERTEX_2D g_aVertex[4];							//頂点情報を格納
LPD3DXFONT g_pFont = NULL;						//フォントのカウンタ
int g_nCountFPS = 0;							//FPSカウンタ
int g_nSpace = 0;

//============================
//メイン関数
//============================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;						//現在時刻
	DWORD dwExecLastTime;						//最後に処理した時刻
	DWORD dwFrameCount;							//フレームカウント
	DWORD dwFPSLastTime;						//最後にFPSを計測した時刻

	WNDCLASSEX wcex 
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								//ウィンドウのスタイル
		WindowProc,								//ウィンドウプロシージャ
		0,										//0にする（通常使用しない）
		0,										//0にする（通常使用しない）
		hInstance,								//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),			//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),				//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				//クライアント領域の背景色(+ 0～26の数値を変えることで色が変わる)
		NULL,									//メニューバー
		CLASS_NAME,								//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)			//ファイルのアイコン
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,			//拡張ウィンドウスタイル
		CLASS_NAME,						//ウィンドウクラスの名前
		WINDOW_NAME,					//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,			//ウィンドウスタイル
		CW_USEDEFAULT,					//ウィンドウの左上X座標
		CW_USEDEFAULT,					//ウィンドウの左上Y座標
		(rect.right - rect.left),		//ウィンドウの幅
		(rect.bottom - rect.top),		//ウィンドウの幅

		NULL,							//親ウィンドウのハンドル
		NULL,							//メニューハンドルまたは子ウィンドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウィンドウ作成データ

	//ウィンドウの表示

	//初期化処理

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//初期化
	dwExecLastTime = timeGetTime();		//現在時刻を取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);			//ウィンドウの表示状態を設定
				DispatchMessage(&msg);			//クライアント領域を更新			
			}
		}
		else
		{//DirectXの処理

			dwExecLastTime = timeGetTime();		//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを測定
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
				dwFrameCount = 0;					//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000/60))
			{
				dwExecLastTime = dwCurrentTime;		//処理開始時刻[現在時刻]を保存

				//更新処理
				Update();

				//描画処理
				Draw();
				
				dwFrameCount++;
			}
		}
	}
	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{//
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		//{ESC}キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		
		case VK_SPACE:		//{ - }キーが押された
			g_nSpace = 1;
			break;
		
		}
		break;

	case WM_CLOSE:			//閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0を返さないと終了してしまう
		}
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);				//既定の処理を返す
}

//初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;											//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;									//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		//テクスチャの拡縮の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		//テクスチャの繰り返しの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//テクスチャのアルファブレンドの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用のフォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//カメラの初期化処理
	InitCamera();	
					
	//ライトの初期化処理
	InitLight();

	//ポリゴンの初期化処理
	InitWall();
	InitMeshWall();

	//弾の初期化処理
	InitBullet();

	//地面の初期化処理
	InitField();
	InitMeshfield();

	//筒の初期化処理
	InitMeshCylinder();

	//爆発の初期化処理
	InitExplosion();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//影の初期化処理
	InitShadow();
	
	//ビルボードの初期化処理
	InitBillboard();

	//敵の初期化処理
	InitEnemy();

	//モデルの初期化処理
	InitModel();
	return S_OK;
}

//終了処理
void Uninit(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//PADの終了処理
	UninitGamepad();

	//ポリゴンの終了処理
	UninitWall();
	UninitMeshWall();

	//地面の終了処理
	UninitField();
	UninitMeshfield();

	//筒の終了処理
	UninitMeshCylinder();

	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//爆発の終了処理
	UninitExplosion();

	//影の終了処理
	UninitShadow();

	//ビルボードの終了処理
	UninitBillboard();

	//敵の終了処理
	UninitEnemy();

	//モデルの終了処理
	UninitModel();

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//更新処理
void Update(void)
{
//キーボードの更新処理
	UpdateKeyboard();

	//PADの更新処理
	UpdateGamepad();

	//カメラの更新処理
	UpdateCamera();

	//モデルの更新処理
	UpdateModel();
	
	//敵の更新処理
	UpdateEnemy();

	//ポリゴンの更新処理
	UpdateWall();
	UpdateMeshWall();

	//地面の更新処理
	UpdateField();
	UpdateMeshfield();

	//筒の更新処理
	UpdateMeshCylinder();

	//弾の更新処理
	UpdateBullet();

	//パーティクルの更新処理
	UpdateParticle();

	//エフェクトの更新処理
	UpdateEffect();

	//爆発の更新処理
	UpdateExplosion();

	//影の更新処理
	UpdateShadow();

	//ビルボードの更新処理
	UpdateBillboard();
}

//描画処理
void Draw(void)
{
	//画面クリア
	g_pD3DDevice->Clear(0,NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0,0,0,0),1.0f,0);

//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene() ))
	{//描画開始が成功した場合

		//カメラの設定処理
		SetCamera();

		 //ポリゴンの描画処理
		DrawWall();
		DrawMeshWall();

		//地面の更新処理
		//DrawField();
		DrawMeshfield();

		//ビルボードの描画処理
		DrawBillboard();

		//モデルの描画処理
		DrawModel();

		//敵の更新処理
		DrawEnemy();

		//筒の描画処理
		DrawMeshCylinder();

		//弾の描画処理
		DrawBullet();

		//パーティクルの描画処理
		DrawParticle();

		//エフェクトの描画処理
		DrawEffect();

		//爆発の描画処理
		DrawExplosion();

		//影の描画処理
		DrawShadow();

		//デバッグ表示
		DrawFPS();

	 //描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL,NULL);

}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//FPSの表示
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[9][256];
	D3DXVECTOR3 CameraPosV = GetCameraPosV();
	D3DXVECTOR3 CameraPosR = GetCameraPosR();
	D3DXVECTOR3 CameraVecU = GetCameraVecU();
	D3DXVECTOR3 CameraRot = GetCamerarot();
	D3DXVECTOR3 CameraMove = GetCameramove();
	float fVRLength = GetVRDis();
	char sVRswitch[8] = {};
	bool bVR = GetVRswitch();
	
	if (bVR == false)
	{
		strcpy(sVRswitch, "3人称");
	}
	else
	{
		strcpy(sVRswitch, "1人称");
	}

	sprintf(&aStr[0][0], "FPS：%d", g_nCountFPS);
	sprintf(&aStr[1][0], "カメラの視点：%f %f %f", CameraPosV.x, CameraPosV.y, CameraPosV.z);
	sprintf(&aStr[2][0], "\nカメラの注視点：%f %f %f", CameraPosR.x, CameraPosR.y, CameraPosR.z);
	sprintf(&aStr[3][0], "\n\nカメラの上方向ベクトル：%f %f %f", CameraVecU.x, CameraVecU.y, CameraVecU.z);
	sprintf(&aStr[4][0], "\n\n\nカメラの角度：%f %f %f", CameraRot.x, CameraRot.y, CameraRot.z);
	sprintf(&aStr[5][0], "\n\n\n\nカメラの移動量：%f %f %f", CameraMove.x, CameraMove.y, CameraMove.z);
	sprintf(&aStr[6][0], "\n\n\n\n\n\n<カメラ移動>：(W/A/S/D)");
	sprintf(&aStr[7][0], "\n\n\n\n\n\n\n<視点 / 注視点旋回>：(Q/E/Z/C)");
	sprintf(&aStr[8][0], "\n\n\n\n\n\n\n\n\n<2点間の距離>：[%f]", fVRLength);

			
	//テキスト描画
	g_pFont->DrawText(NULL, &aStr[0][0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[1][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[2][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[3][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[4][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[5][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[6][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[7][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[8][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

LPD3DXFONT GetFont(void)
{
	return g_pFont;
}
