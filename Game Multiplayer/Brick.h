#pragma once

#include "Entity.h"
#include "Animation.h"
#include "SpriteList.h"

class Brick : public Entity
{
protected:
	Animation* animation;

protected:
	void BaseInit(D3DXVECTOR2 _pos)
	{
		width = 16;
		height = 16;

		SetPosition(_pos);
	}
public:
	Brick() {}
	~Brick()  { delete animation; }

	void Draw() {
		if (!IsDelete)
			animation->Draw(GetPosition());
	}

	void Read(InputMemoryBitStream& is) override
	{
		is.Read(IsDelete);
	}
};

