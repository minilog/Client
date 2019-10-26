#include "RoomScene.h"

RoomScene::RoomScene()
{
	label_TitleRoom = Label("ROOM: 0", 40, 20, D3DXVECTOR2(90.f, 50.f));
}

RoomScene::~RoomScene()
{
}

void RoomScene::Draw()
{
	label_TitleRoom.Draw();
}
