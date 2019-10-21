#pragma once
#include "SceneY.h"
class RoomSceneY :
	public SceneY
{
public:
	RoomSceneY();
	~RoomSceneY();

	void Draw();

private:
	Label label_TitleRoom;
};

