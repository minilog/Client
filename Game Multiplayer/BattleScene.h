#pragma once

#include "Scene.h"
#include "GameMap.h"
#include "Player.h"
#include "Bullet.h"
#include "NPC.h"
#include "Explosion.h"
#include "ProtectItem.h"
#include "UpgradeItem.h"
#include "Pointed.h"

class BattleScene : public Scene
{
	GameMap* map;
	vector<Player*> playerList;
	vector<Bullet*> bulletList;
	int lastReceivedTime = -1;
	vector<NPC*> npcList;
	vector<Explosion*> smallExList;
	vector<Explosion*> bigExList;
	ProtectItem* protectItem;
	UpgradeItem* upgradeItem;
	Pointed* pointed;

	Label label_Player0;
	Label label_Player1;
	Label label_Player2;
	Label label_Player3;
	Label label_CountTime;
	Label label_TimeUp;
	float count_TimeUp = 100.0f;;

public:
	BattleScene(vector<bool> _playerInRoomList, float time);
	~BattleScene();
	void Update(float _dt) override;
	void Draw() override;
	void ReceivePacket(InputMemoryBitStream& _is, int _packetType) override;
};

