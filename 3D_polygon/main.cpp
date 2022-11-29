//============================
//
//DirectX
//�R�����g�m�F���ׂ��B
//
//Author ���c ����
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

//�}�N����`
#define CLASS_NAME "WindowClass"				//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME "DirectX"					//�E�B���h�E�̖��O(�L���v�V�����ɕ\��)
#define ID_TIMER (131)							//�^�C�}�[��ID
#define TIMER_INTERVAL (1000/60)				//�^�C�}�[�̔����Ԋu(�~���b)
#define NOT_DATA (0)							//���͘g

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init (HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;						//Dierct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Dierct3D�f�o�C�X�ւ̃|�C���^
VERTEX_2D g_aVertex[4];							//���_�����i�[
LPD3DXFONT g_pFont = NULL;						//�t�H���g�̃J�E���^
int g_nCountFPS = 0;							//FPS�J�E���^
int g_nSpace = 0;

//============================
//���C���֐�
//============================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;						//���ݎ���
	DWORD dwExecLastTime;						//�Ō�ɏ�����������
	DWORD dwFrameCount;							//�t���[���J�E���g
	DWORD dwFPSLastTime;						//�Ō��FPS���v����������

	WNDCLASSEX wcex 
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,								//�E�B���h�E�̃X�^�C��
		WindowProc,								//�E�B���h�E�v���V�[�W��
		0,										//0�ɂ���i�ʏ�g�p���Ȃ��j
		0,										//0�ɂ���i�ʏ�g�p���Ȃ��j
		hInstance,								//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),			//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),				//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),				//�N���C�A���g�̈�̔w�i�F(+ 0�`26�̐��l��ς��邱�ƂŐF���ς��)
		NULL,									//���j���[�o�[
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)			//�t�@�C���̃A�C�R��
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,FALSE);

	//�E�B���h�E�𐶐�
	hWnd = CreateWindowEx(0,			//�g���E�B���h�E�X�^�C��
		CLASS_NAME,						//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,					//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,					//�E�B���h�E�̍���Y���W
		(rect.right - rect.left),		//�E�B���h�E�̕�
		(rect.bottom - rect.top),		//�E�B���h�E�̕�

		NULL,							//�e�E�B���h�E�̃n���h��
		NULL,							//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,						//�C���X�^���X�n���h��
		NULL);							//�E�B���h�E�쐬�f�[�^

	//�E�B���h�E�̕\��

	//����������

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//������
	dwExecLastTime = timeGetTime();		//���ݎ������擾
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//�E�B���h�E�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);			//�E�B���h�E�̕\����Ԃ�ݒ�
				DispatchMessage(&msg);			//�N���C�A���g�̈���X�V			
			}
		}
		else
		{//DirectX�̏���

			dwExecLastTime = timeGetTime();		//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS�𑪒�
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000/60))
			{
				dwExecLastTime = dwCurrentTime;		//�����J�n����[���ݎ���]��ۑ�

				//�X�V����
				Update();

				//�`�揈��
				Draw();
				
				dwFrameCount++;
			}
		}
	}
	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{//
	case WM_DESTROY:
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:		//{ESC}�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				//�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		
		case VK_SPACE:		//{ - }�L�[�������ꂽ
			g_nSpace = 1;
			break;
		
		}
		break;

	case WM_CLOSE:			//����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);				//����̏�����Ԃ�
}

//����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;											//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;									//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
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

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		//�e�N�X�`���̊g�k�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		//�e�N�X�`���̌J��Ԃ��̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//�e�N�X�`���̃A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�O�\���p�̃t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�J�����̏���������
	InitCamera();	
					
	//���C�g�̏���������
	InitLight();

	//�|���S���̏���������
	InitWall();
	InitMeshWall();

	//�e�̏���������
	InitBullet();

	//�n�ʂ̏���������
	InitField();
	InitMeshfield();

	//���̏���������
	InitMeshCylinder();

	//�����̏���������
	InitExplosion();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�e�̏���������
	InitShadow();
	
	//�r���{�[�h�̏���������
	InitBillboard();

	//�G�̏���������
	InitEnemy();

	//���f���̏���������
	InitModel();
	return S_OK;
}

//�I������
void Uninit(void)
{
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//PAD�̏I������
	UninitGamepad();

	//�|���S���̏I������
	UninitWall();
	UninitMeshWall();

	//�n�ʂ̏I������
	UninitField();
	UninitMeshfield();

	//���̏I������
	UninitMeshCylinder();

	//�e�̏I������
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�����̏I������
	UninitExplosion();

	//�e�̏I������
	UninitShadow();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�G�̏I������
	UninitEnemy();

	//���f���̏I������
	UninitModel();

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//�X�V����
void Update(void)
{
//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//PAD�̍X�V����
	UpdateGamepad();

	//�J�����̍X�V����
	UpdateCamera();

	//���f���̍X�V����
	UpdateModel();
	
	//�G�̍X�V����
	UpdateEnemy();

	//�|���S���̍X�V����
	UpdateWall();
	UpdateMeshWall();

	//�n�ʂ̍X�V����
	UpdateField();
	UpdateMeshfield();

	//���̍X�V����
	UpdateMeshCylinder();

	//�e�̍X�V����
	UpdateBullet();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�����̍X�V����
	UpdateExplosion();

	//�e�̍X�V����
	UpdateShadow();

	//�r���{�[�h�̍X�V����
	UpdateBillboard();
}

//�`�揈��
void Draw(void)
{
	//��ʃN���A
	g_pD3DDevice->Clear(0,NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0,0,0,0),1.0f,0);

//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene() ))
	{//�`��J�n�����������ꍇ

		//�J�����̐ݒ菈��
		SetCamera();

		 //�|���S���̕`�揈��
		DrawWall();
		DrawMeshWall();

		//�n�ʂ̍X�V����
		//DrawField();
		DrawMeshfield();

		//�r���{�[�h�̕`�揈��
		DrawBillboard();

		//���f���̕`�揈��
		DrawModel();

		//�G�̍X�V����
		DrawEnemy();

		//���̕`�揈��
		DrawMeshCylinder();

		//�e�̕`�揈��
		DrawBullet();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�����̕`�揈��
		DrawExplosion();

		//�e�̕`�揈��
		DrawShadow();

		//�f�o�b�O�\��
		DrawFPS();

	 //�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL,NULL);

}

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//FPS�̕\��
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
		strcpy(sVRswitch, "3�l��");
	}
	else
	{
		strcpy(sVRswitch, "1�l��");
	}

	sprintf(&aStr[0][0], "FPS�F%d", g_nCountFPS);
	sprintf(&aStr[1][0], "�J�����̎��_�F%f %f %f", CameraPosV.x, CameraPosV.y, CameraPosV.z);
	sprintf(&aStr[2][0], "\n�J�����̒����_�F%f %f %f", CameraPosR.x, CameraPosR.y, CameraPosR.z);
	sprintf(&aStr[3][0], "\n\n�J�����̏�����x�N�g���F%f %f %f", CameraVecU.x, CameraVecU.y, CameraVecU.z);
	sprintf(&aStr[4][0], "\n\n\n�J�����̊p�x�F%f %f %f", CameraRot.x, CameraRot.y, CameraRot.z);
	sprintf(&aStr[5][0], "\n\n\n\n�J�����̈ړ��ʁF%f %f %f", CameraMove.x, CameraMove.y, CameraMove.z);
	sprintf(&aStr[6][0], "\n\n\n\n\n\n<�J�����ړ�>�F(W/A/S/D)");
	sprintf(&aStr[7][0], "\n\n\n\n\n\n\n<���_ / �����_����>�F(Q/E/Z/C)");
	sprintf(&aStr[8][0], "\n\n\n\n\n\n\n\n\n<2�_�Ԃ̋���>�F[%f]", fVRLength);

			
	//�e�L�X�g�`��
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
