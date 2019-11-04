#include "BattleScene.h"

BattleScene::BattleScene()
{
	map = new GameMap("Resource files/map.tmx");

	// init 4 players
	for (int i = 0; i < 4; i++)
	{
		Player* player = new Player(i);
		playerList.push_back(player);
	}
}

BattleScene::~BattleScene()
{
	delete map;
}

void BattleScene::Update(float _dt)
{
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
		for (auto player : playerList)
		{
			player->Read(_is);
		}
	}
}
