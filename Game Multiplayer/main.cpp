#include <d3d9.h>
#include <d3dx9.h>

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <dinput.h>
#include <windowsx.h>
#include "GameGlobal.h"
#include "GameTime.h"
#include "SceneManager.h"
#include "Game.h"
#include "MemoryBitStream.h"
#pragma comment(lib,"WS2_32")

using namespace std;

#define WIN_NAME L"Game DirectX"
#define WIN_TITLE L"Game DirectX"
#define FPS 60
#define KEYBOARD_BUFFERD_SIZE 1024

/*

*/
int initWindow(int cmdShow);
int InitDevice();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LPDIRECT3D9             mDirect3D9;
LPD3DXSPRITE            mSpriteHandler;
PDIRECT3D9              mD3d;
LPDIRECT3DDEVICE9       mDevice;
HINSTANCE               mHInstance;
int                     mCmdShow;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	mHInstance = hInstance;
	initWindow(cmdShow);


	return 0;
}

int initWindow(int cmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = mHInstance;

	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WIN_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(
		WIN_NAME,
		WIN_NAME,
		WS_OVERLAPPEDWINDOW,
		116,
		40,
		GameGlobal::Width,
		GameGlobal::Height,
		NULL,
		NULL,
		mHInstance,
		NULL);

	GameGlobal::HInstance = mHInstance;
	GameGlobal::Window = hWnd;

	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);

	WSADATA a;
	WSAStartup(MAKEWORD(2, 2), &a);
	if (InitDevice())
	{
		Game *game = new Game();
	}

	return 0;
}

int InitDevice()
{
	mD3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = GameGlobal::Width;
	d3dpp.BackBufferHeight = GameGlobal::Height;

	HRESULT dvresult = mD3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		GameGlobal::Window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&mDevice);
	GameGlobal::Device = mDevice;

	D3DXCreateSprite(GameGlobal::Device, &mSpriteHandler);
	GameGlobal::XSprite = mSpriteHandler;

	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		GameGlobal::Send_Exit();
		GameGlobal::Send_Exit();

		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		SceneManager::Instance()->GetCurrentScene()->OnKeyDown(wParam);
		break;

	case WM_KEYUP:
		SceneManager::Instance()->GetCurrentScene()->OnKeyUp(wParam);
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}