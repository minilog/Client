#pragma once

#include "Entity.h"

class Eagle:public Entity
{
	Animation* animation;

public:
	Eagle(int _ID, D3DXVECTOR2 _pos)
	{
		ID = _ID;

		Type = ET_Eagle;
		position = _pos;
		width = 50;
		height = 50;
		animation = new Animation();

		// tùy theo network ID mà màu của đại bàng sẽ khác nhau, tương ứng với các người chơi
		switch (ID)
		{
		case 0:
			// màu vàng
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 1:
			// màu trắng
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 50, 100, 0, 50, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 2:
			// màu xanh
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 50, 100, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 3:
			// màu đỏ
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 50, 100, 50, 100, D3DXVECTOR2(25.f, 25.f)));
			break;
		}
	}
	~Eagle() { delete animation; }

	void Draw()
	{
		// nếu đã chết thì không cần vẽ nữa
		if (IsDelete) 
			return;

		animation->Draw(GetPosition());
	}

	void Read(InputMemoryBitStream &_is) override
	{
		bool _isDelete = false;
		_is.Read(_isDelete); // 1 bit

		IsDelete = _isDelete;
	}
};

