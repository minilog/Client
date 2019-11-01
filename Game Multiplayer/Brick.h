#pragma once

#include "Entity.h"
#include "Animation.h"
#include "SpriteList.h"

class Brick : public Entity
{
protected:
	Animation* animation;

public:
	Brick() {}
	~Brick()  { delete animation; }

	void Draw() {
		if (!IsDelete)
			animation->Draw(position);
	}

// các hàm hỗ trợ
protected:
	void BaseInit(D3DXVECTOR2 _pos)
	{
		position = _pos;
		width = 16;
		height = 16;
		animation = new Animation();
	}
};

