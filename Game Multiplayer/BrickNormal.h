#pragma once

#include "Brick.h"

class BrickNormal : public Brick
{
public:
	BrickNormal(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		Type = ET_NormalBrick;

		// thêm frame animation
		animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 0, 0 + 16, 128, 128 + 16,
			D3DXVECTOR2(8.f, 8.f)));


	}
	~BrickNormal() {}


	void Read(InputMemoryBitStream& _is) override
	{
		bool _isDelete = false;
		_is.Read(_isDelete);

		IsDelete = _isDelete;
	}
};

