#include "BattleScene.h"

BattleScene::BattleScene()
{
	map = new GameMap("Resource files/map.tmx");
}

BattleScene::~BattleScene()
{
	delete map;
}

void BattleScene::Update(float _dt)
{
}

void BattleScene::Draw()
{
	map->Draw();
}

void BattleScene::ReceivePacket(InputMemoryBitStream & _is, int _typePacket)
{
}
