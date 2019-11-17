#include "Game.h"

#include "GameGlobal.h"
#include "SceneManager.h"
#include <thread>
#include "TimeServer.h"
#include "GameLog.h"
#include "LobbyScene.h"
#include "MemoryBitStream.h"
#include "SocketUtil.h"
using namespace Define;

Game::Game()
{
	// tạo socket & kết nối với server
	{
		string ip = "127.0.0.1";
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

	SceneManager::Instance()->ReplaceScene(new LobbyScene());

	InitLoop();
}

void Game::Update(float _dt)
{
	TimeServer::Instance()->Update(_dt);
	SceneManager::Instance()->GetCurrentScene()->Update(_dt);
}

void Game::ReceivePacket()
{
	// receive packet
	{
		char* buff = static_cast<char*>(std::malloc(1024));
		int receivedByteCount = GameGlobal::Socket->Receive(buff, 1024);

		// have data to receive
		if (receivedByteCount > 0)
		{
			InputMemoryBitStream is(buff,
				static_cast<uint32_t> (receivedByteCount));

			int packetType;
			while ((int)is.GetRemainingBitCount() > NBit_PacketType)
			{
				packetType = 0;
				is.Read(packetType, NBit_PacketType);

				TimeServer::Instance()->ReceivePacket(is, packetType);
				SceneManager::Instance()->GetCurrentScene()->ReceivePacket(is, packetType);

				// dọn dẹp byte cuối của packet (khi có nhiều packet 1 lúc)
				{
					int nClearBit = is.GetRemainingBitCount() % 8; // số bit cần clear
					int k;
					is.Read(k, nClearBit);
				}
			}

			free(buff);
		}
	}
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

void Game::InitLoop()
{
	MSG msg;

	//std::thread task_receive_packet(ReceivePacket);
	//task_receive_packet.detach();
	float tickPerFrame = 1.0f / 60, delta = 0;

	int count = 0;
	while (GameGlobal::IsGameRunning)
	{
		ReceivePacket();

		GameTime::GetInstance()->StartCounter();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		delta += GameTime::GetInstance()->GetCouter();

		if (delta >= tickPerFrame)
		{
			count++;
			if (count == 60)
			{
				count = 0;
				GAMELOG("%i", (int)GetTickCount());
			}

			Update(delta);
			delta = 0;
			Render();
		}
		else
		{
			ReceivePacket();
			int delta_time = (int)(tickPerFrame - delta);
			Sleep(delta_time);
			delta = tickPerFrame;	
		}

	}
}