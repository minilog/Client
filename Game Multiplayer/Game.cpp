#include "Game.h"

#include "GameGlobal.h"
#include "SceneManager.h"
#include <thread>
#include "TimingScene.h"
#include "GameLog.h"
#include "LobbyScene.h"
#include "RoomScene.h"
#include "MemoryBitStream.h"
#include "SocketUtil.h"

Game::Game(int _fps)
{
	FPS = (float)_fps;

	CreateSocketAndTryConnectToServer();

	timingScene = new TimingScene();
	SceneManager::Instance()->ReplaceScene(new LobbyScene());

	InitLoop();
}

void Game::Update(float dt)
{
	// đưa Dữ Liệu nhận được vào Input Stream
	char* buff = static_cast<char*>(std::malloc(1024));
	int receivedByteCount = GameGlobal::Socket->Receive(buff, 1024);

	// nếu có Dữ Liệu nhận được
	if (receivedByteCount > 0)
	{
		InputMemoryBitStream is(buff,
			static_cast<uint32_t> (receivedByteCount));

		while (is.GetRemainingBitCount() > Define::bitOfTypePacket)
		{
			// đọc packetType, xác định loại dữ liệu sẽ được đọc tiếp theo
			int packetType = 0;
			is.Read(packetType, Define::bitOfTypePacket);

			// màn game time server nhận packet
			timingScene->ReceivePacket(is, packetType);

			// màn game chính nhận packet
			SceneManager::Instance()->GetCurrentScene()->ReceivePacket(is, packetType);
		}
	}
	// giải phóng buffer
	free(buff);

	// các màn game cập nhật
	timingScene->Update(dt);
	SceneManager::Instance()->GetCurrentScene()->Update(dt);

	Render();
}

void Game::Render()
{
	auto device = GameGlobal::Device;
	auto scene = SceneManager::Instance()->GetCurrentScene();
	device->Clear(0, NULL, D3DCLEAR_TARGET, timingScene->GetBackcolor(), 0.0f, 0);

	{
		device->BeginScene();

		//bat dau ve
		GameGlobal::XSprite->Begin(D3DXSPRITE_ALPHABLEND);

		//draw here
		scene->Draw();
		timingScene->Draw();

		//ket thuc ve
		GameGlobal::XSprite->End();

		device->EndScene();
	}

	device->Present(0, 0, 0, 0);
}

void Game::InitLoop()
{
	MSG msg;

	float tickPerFrame = 1.0f / 60, delta = 0;

	while (GameGlobal::IsGameRunning)
	{
		GameTime::GetInstance()->StartCounter();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		delta += GameTime::GetInstance()->GetCouter();

		if (delta >= tickPerFrame)
		{
			Update(1.0f / 60);
			delta = 0;
		}
		else
		{
			int delta_time = (int)(tickPerFrame - delta);
			Sleep(delta_time);
			delta = tickPerFrame;
		}
	}
}

void Game::CreateSocketAndTryConnectToServer()
{
	//"180.34.5.101"
	string ip = "127.0.0.1";
	//ip = "180.34.5.101";
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