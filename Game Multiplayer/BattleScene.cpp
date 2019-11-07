#include "BattleScene.h"
#include "GameLog.h"
#include "TimeServer.h"

BattleScene::BattleScene(vector<bool> _playerInRoomList)
{
	map = new GameMap("Resource files/map.tmx");

	// init players in list
	for (int i = 0; i < 4; i++)
	{
		if (_playerInRoomList[i] == true)
		{
			Player* player = new Player(i);
			playerList.push_back(player);
		}
	}
}

BattleScene::~BattleScene()
{
	delete map;
}

void BattleScene::Update(float _dt)
{
	//for (auto brick : map->GetBrickList())
	//{
	//	if (!brick->IsDelete)
	//	{
	//		for (auto player : playerList)
	//		{
	//			if (GameCollision::IsCollideInNextFrame(player, brick, _dt))
	//			{
	//				player->MakeCollision(brick);
	//			}
	//		}
	//	}
	//}

	for (auto player : playerList)
	{
		player->Update(_dt);
		player->HandleKeyboard(keyboard);
	}
}

void BattleScene::Draw()
{
	map->Draw();
	for (auto player : playerList)
	{
		player->Draw();
	}
}

void BattleScene::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{
	if (_packetType == PT_PlayerInput)
	{
		int sentTime = 0;
		_is.Read(sentTime, NBit_Time);

		for (auto player : playerList)
		{
			//player->Read(_is, true);

			// not receive
			if (lastSentTime >= sentTime)
			{
				player->Read(_is, false);
			}
			else
			{
				player->Read(_is, true);
				lastSentTime = sentTime;
			}

			int nFramePrevious = TimeServer::Instance()->GetServerTime() - sentTime; // đã bao nhiêu frame trôi qua từ lúc client gửi
		}
	}
}
