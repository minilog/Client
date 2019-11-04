#pragma once

#include "Scene.h"
#include "GameMap.h"
#include "Player.h"

class BattleScene : public Scene
{
	GameMap* map;
	vector<Player*> playerList;

public:
	BattleScene();
	~BattleScene();
	void Update(float _dt) override;
	void Draw() override;
	void ReceivePacket(InputMemoryBitStream& _is, int _packetType) override;
};

