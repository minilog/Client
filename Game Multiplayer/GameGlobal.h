#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include "TCPSocket.h"

class GameGlobal
{
public:
	static HINSTANCE HInstance;
	static HWND Window;
	static LPDIRECT3DDEVICE9 Device;
	static LPD3DXSPRITE XSprite;

	static bool IsGameRunning;
	static TCPSocketPtr Socket;

	static int Width;
	static int Height;

	static void Send_Exit();
};