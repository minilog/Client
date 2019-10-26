#pragma once

#include "Scene.h"

class RoomScene : public Scene
{
public:
	RoomScene();
	~RoomScene();

	void Draw();

private:
	Label label_TitleRoom;
};

