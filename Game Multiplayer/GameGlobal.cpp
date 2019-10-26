#include "GameGlobal.h"

#include "MemoryBitStream.h"

HINSTANCE GameGlobal::HInstance = NULL;
HWND GameGlobal::Window = NULL;
LPDIRECT3DDEVICE9 GameGlobal::Device = nullptr;
LPD3DXSPRITE GameGlobal::XSprite = NULL;

bool GameGlobal::IsGameRunning = true;
TCPSocketPtr GameGlobal::Socket = NULL;

int GameGlobal::Width = 1300;
int GameGlobal::Height = 770;

void GameGlobal::Send_Exit()
{
	OutputMemoryBitStream os;
	os.Write(Define::ExitGame, Define::bitOfTypePacket);
	GameGlobal::Socket->Send(os);
	GameGlobal::IsGameRunning = false;
}
