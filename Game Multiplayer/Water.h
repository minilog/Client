#pragma once

#include "Brick.h"

class Water : public Brick
{
public:
	Water(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		Type = ET_Water;

		// th�m frame animation
		animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 0, 0 + 16, 160, 160 + 16,
			D3DXVECTOR2(8.f, 8.f)));
	}
	~Water() {}
};

