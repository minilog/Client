#include "Game.h"

#include "GameGlobal.h"
#include "SceneManager.h"
#include <thread>
#include "TimeServer.h"
#include "GameLog.h"
#include "LobbyScene.h"
#include "MemoryBitStream.h"
#include "SocketUtil.h"
#include <stdio.h>
using namespace Define;


Game::Game()
{
	InitSocket();

	SceneManager::Instance()->ReplaceScene(new LobbyScene());

	InitLoop();
}

void Game::Update(float dt)
{
	TimeServer::Instance()->Update(dt);
	SceneManager::Instance()->GetCurrentScene()->Update(dt);
}

void Game::ReceivePacket()
{
	char* buff = static_cast<char*>(std::malloc(4096));
	int receivedByteCount = GameGlobal::Socket->Receive(buff, 4096);

	// nhận được data ?
	if (receivedByteCount > 0)
	{
		InputMemoryBitStream is(buff,
			static_cast<uint32_t> (receivedByteCount));

		int pType;
		while ((int)is.GetRemainingBitCount() > NBit_PacketType)
		{
			pType = 0;
			is.Read(pType, NBit_PacketType);

			SceneManager::Instance()->GetCurrentScene()->ReceivePacket(is, pType);
			TimeServer::Instance()->ReceivePacket(is, pType);

			// dọn dẹp byte cuối của packet (khi có nhiều packet 1 lúc)
			int nClearBit = is.GetRemainingBitCount() % 8;
			int temp;
			is.Read(temp, nClearBit);
		}
	}
	free(buff);
}

void Game::Render()
{
	auto device = GameGlobal::Device;
	auto scene = SceneManager::Instance()->GetCurrentScene();

	device->Clear(0, NULL, D3DCLEAR_TARGET, scene->GetBackcolor(), 0.0f, 0);

	{
		device->BeginScene();

		//bat dau ve
		GameGlobal::XSprite->Begin(D3DXSPRITE_ALPHABLEND);

		//draw here
		scene->Draw();
		TimeServer::Instance()->Draw();

		//ket thuc ve
		GameGlobal::XSprite->End();

		device->EndScene();
	}

	device->Present(0, 0, 0, 0);
}

void Game::InitSocket()
{
	FILE *fp;
	char buff[255];

	fopen_s(&fp, "Resource files/Config.txt", "r");
	fgets(buff, 255, (FILE*)fp);

	string ip = buff;
	if (__argv[1] != NULL)
	{
		ip = string(__argv[1]);
	}

	SocketAddress socketAddress(inet_addr(ip.c_str()), 8888);

	GameGlobal::Socket = SocketUtil::CreateTCPSocket();

	if (GameGlobal::Socket->Connect(socketAddress) == SOCKET_ERROR)
	{
		OutputDebugStringA("Connect to Server failed!");
	}
	else
	{
		OutputDebugStringA("Connect to Server successfull!");
	}

	GameGlobal::Socket->ChangetoDontWait(1);
}

void Game::InitLoop()
{
	MSG msg;
	float delta = 0;

	while (GameGlobal::IsGameRunning)
	{
		GameTime::GetInstance()->StartCounter();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		delta += GameTime::GetInstance()->GetCouter();

		if (delta >= 1.0f / 60)
		{
			ReceivePacket();
			Update(delta);
			Render();
			delta = 0;
		}
		else
		{
			Sleep((int)(1.0f / 60 - delta));
			delta = 1.0f / 60;
		}
	}
}