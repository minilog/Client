#include "GameGlobal.h"

#include "MemoryBitStream.h"
#include "GameDefine.h"
using namespace Define;

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

	os.Write(PT_ExitGame, NBit_PacketType);

	GameGlobal::Socket->Send(os);

	GameGlobal::IsGameRunning = false;
}
