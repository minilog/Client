#pragma once

#include "Brick.h"

class Boundary : public Brick
{
public:
	Boundary(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		Type = ET_Boundary;

		// thêm frame animation
		animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 32, 32 + 16, 144, 144 + 16,
			D3DXVECTOR2(8.f, 8.f)));
	}
	~Boundary() {}
};

