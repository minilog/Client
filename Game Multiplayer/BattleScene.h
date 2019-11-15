#pragma once

#include "Scene.h"
#include "GameMap.h"
#include "Player.h"
#include "Bullet.h"
#include "NPC.h"

class BattleScene : public Scene
{
	GameMap* map;
	vector<Player*> playerList;
	vector<Bullet*> bulletList;
	int lastReceivedTime = -1;
	NPC* npc;

public:
	BattleScene(vector<bool> _playerInRoomList);
	~BattleScene();
	void Update(float _dt) override;
	void Draw() override;
	void ReceivePacket(InputMemoryBitStream& _is, int _packetType) override;
};

