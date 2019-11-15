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

	npc = new NPC(0);
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
		player->HandleKeyboard(keyboard, _dt);
	}

	for (auto brick : map->GetBrickList())
	{
		if (!brick->IsDelete)
		{
			for (auto player : playerList)
			{
				if (player->IsDelete == false)
				{
					if (GameCollision::IsCollideInNextFrame(player, brick, _dt))
					{
						player->MakeCollision(brick);
					}
				}
			}

			// npc va chạm bricks
			if (npc->IsDelete == false)
			{
				if (GameCollision::IsCollideInNextFrame(npc, brick, _dt))
				{
					npc->MakeCollision(brick);
				}
			}
		}
	}

	for (auto bullet : bulletList)
	{
		bullet->Update(_dt);
	}
	for (auto player : playerList)
	{
		player->Update(_dt);
	}
	npc->Update(_dt);
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
	npc->Draw();
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

			// nhận bulletList
			for (auto bullet : bulletList)
			{
				bullet->Read(_is, false);
			}

			npc->Read(_is, false);

			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				_is.Read(_isDelete);
			}
		}
		else
		{
			for (auto player : playerList)
			{
				player->Read(_is, true);
			}

			// nhận bulletList
			for (auto bullet : bulletList)
			{
				bullet->Read(_is, true);
			}

			npc->Read(_is, true);

			// nhận brickNorList
			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				_is.Read(_isDelete);

				brick->IsDelete = _isDelete;
			}

			int nFramePrevious = TimeServer::Instance()->GetServerTime() - receivedTime; // đã bao nhiêu frame trôi qua từ lúc client gửi
			lastReceivedTime = receivedTime;
		}
	}
}
