#pragma once

#include "Brick.h"

class Boundary : public Brick
{
public:
	Boundary(D3DXVECTOR2 _pos)
	{
		// loại đối tượng
		Type = ET_Boundary;

		// khởi tạo animation
		animation = new Animation();
		animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 32, 32 + 16, 144, 144 + 16,
			D3DXVECTOR2(8.f, 8.f)));

		// kích cỡ, vị trí viên gạch
		BaseInit(_pos);
	}
	~Boundary() {}
};

