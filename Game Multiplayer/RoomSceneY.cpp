#include "RoomSceneY.h"



RoomSceneY::RoomSceneY()
{
	label_TitleRoom = Label("ROOM: 0", 40, 20, D3DXVECTOR2(GameGlobal::GetWidth() / 2 - 90, 50));
}

RoomSceneY::~RoomSceneY()
{
}

void RoomSceneY::Draw()
{
	label_TitleRoom.Draw();
}
