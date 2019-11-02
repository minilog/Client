#pragma once

#include "Scene.h"
#include "GameMap.h"
#include "Player.h"

class BattleScene : public Scene
{
	GameMap* map;
	Player* myPlayer;

public:
	BattleScene();
	~BattleScene();
	void Update(float _dt) override;
	void Draw() override;
	void ReceivePacket(InputMemoryBitStream& _is, int _typePacket) override;
};

