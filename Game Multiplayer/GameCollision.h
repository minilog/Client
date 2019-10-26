#pragma once
#include "Entity.h"

class GameCollision
{
public:
	GameCollision() {}
	~GameCollision() {}

	static bool	isCollide(Entity* _obj1, Entity* _obj2, float _dt);
};

