#pragma once

#include "Brick.h"

class BrickNormal : public Brick
{
public:
	BrickNormal(D3DXVECTOR2 _pos)
	{
		Type = ET_NormalBrick;
		animation = new Animation();
		animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 0, 0 + 16, 128, 128 + 16,
			D3DXVECTOR2(8.f, 8.f)));

		BaseInit(_pos);
	}

	~BrickNormal() {}

	void CollisionWith(Entity* en) override
	{
		IsDelete = true;
	}
};

