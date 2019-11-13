#include "BattleScene.h"
#include "GameLog.h"
#include "TimeServer.h"

BattleScene::BattleScene(vector<bool> _playerInRoomList)
{
	map = new GameMap("Resource files/map.tmx");

	// tạo plays
	for (int i = 0; i < 4; i++)
	{
		if (_playerInRoomList[i] == true)
		{
			Player* player = new Player(i);
			playerList.push_back(player);

			// tạo 4 bullet cho mỗi player
			for (int j = 0; j < 4; j++)
			{
				Bullet* bullet = new Bullet(j, i);
				bulletList.push_back(bullet);
			}
		}
	}
}

BattleScene::~BattleScene()
{
	delete map;
}

void BattleScene::Update(float _dt)
{
	// nhận keyboard
	for (auto player : playerList)
	{
		player->HandleKeyboard(keyboard);
	}

	for (auto brick : map->GetBrickList())
	{
		if (!brick->IsDelete)
		{
			for (auto player : playerList)
			{
				if (GameCollision::IsCollideInNextFrame(player, brick, _dt))
				{
					player->MakeCollision(brick);
				}
			}
		}
	}

	for (auto player : playerList)
	{
		player->Update(_dt);
	}
}

void BattleScene::Draw()
{
	map->Draw();
	for (auto player : playerList)
	{
		player->Draw();
	}
	for (auto bullet : bulletList)
	{
		bullet->Draw();
	}
}

void BattleScene::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{
	if (_packetType == PT_World)
	{
		int receivedTime = 0;
		_is.Read(receivedTime, NBit_Time);

		// không nhận các packet trễ
		if (lastReceivedTime >= receivedTime)
		{
			for (auto player : playerList)
			{
				player->Read(_is, false);
			}
		}
		else
		{
			for (auto player : playerList)
			{
				player->Read(_is, true);
			}

			int nFramePrevious = TimeServer::Instance()->GetServerTime() - receivedTime; // đã bao nhiêu frame trôi qua từ lúc client gửi
			lastReceivedTime = receivedTime;
		}
	}
}
