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

	// tạo 3 npcs
	for (int i = 0; i < 3; i++)
	{
		NPC* npc = new NPC(i);
		npcList.push_back(npc);
	}
}

BattleScene::~BattleScene()
{
	delete map;
}

void BattleScene::Update(float dt)
{
	// đã nhận packet trước hàm Update này

	// nhận keyboard
	for (auto player : playerList)
	{
		player->HandleKeyboard(keyboard, dt);
	}

	for (auto player : playerList)
	{
		player->ApplyVelocity();
	}
	for (auto npc : npcList)
	{
		npc->ApplyVelocity();
	}

	for (auto player : playerList)
	{
		if (!player->IsDelete)
		{	// players va chạm npcs
			for (auto npc : npcList)
			{
				if (!npc->IsDelete)
				{
					if (GameCollision::IsCollideInNextFrame(player, npc, dt))
					{
						player->ZeroVelocity();
						npc->ZeroVelocity();
					}
				}
			}

			// players va chạm players
			for (auto player2 : playerList)
			{
				if (!player2->IsDelete && player->ID != player2->ID)
				{
					if (GameCollision::IsCollideInNextFrame(player, player2, dt))
					{
						player->ZeroVelocity();
						player2->ZeroVelocity();
					}
				}
			}
		}
	}

	for (auto player : playerList)
	{
		player->Update(dt);
	}
	for (auto npc : npcList)
	{
		npc->Update(dt);
	}


	for (auto brick : map->GetBrickList())
	{
		if (!brick->IsDelete)
		{
			// players va chạm bricks
			for (auto player : playerList)
			{
				player->CheckCollision(brick);
			}

			// npcs va chạm bricks
			for (auto npc : npcList)
			{
				npc->CheckCollision(brick);
			}
		}
	}

	for (auto bullet : bulletList)
	{
		bullet->Update(dt);
	}
}

void BattleScene::Draw()
{
	map->Draw();
	for (auto player : playerList)
	{
		player->Draw();
	}
	for (auto npc : npcList)
	{
		npc->Draw();
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

		int nFramePrevious = (int)((TimeServer::Instance()->GetServerTime() - receivedTime) / 16.7f); // đã bao nhiêu frame trôi qua từ lúc client gửi
		// không nhận các packet trễ
		if (lastReceivedTime >= receivedTime || nFramePrevious >= 30)
		{
			for (auto player : playerList)
			{
				player->Read(_is, false, receivedTime);
			}
			for (auto bullet : bulletList)
			{
				bullet->Read(_is, false);
			}
			for (auto npc : npcList)
			{
				npc->Read(_is, false);
			}
			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				_is.Read(_isDelete);
			}
		}
		// nhận packet
		else
		{
			lastReceivedTime = receivedTime;

			for (auto player : playerList)
			{
				player->Read(_is, true, receivedTime);
			}
			for (auto bullet : bulletList)
			{
				bullet->Read(_is, true);
			}
			for (auto npc : npcList)
			{
				npc->Read(_is, true);
			}
			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				_is.Read(_isDelete);
				brick->IsDelete = _isDelete;
			}
		}
	}
}
