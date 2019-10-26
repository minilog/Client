#pragma once

#include "Entity.h"

class Eagle:public Entity
{
	int HP = 10; // máu của đại bàng
	Animation* animation;
public:
	Eagle(int _networkID, D3DXVECTOR2 _pos)
	{
		NetworkID = _networkID;

		Type = ET_Eagle;
		animation = new Animation();

		// tùy theo network ID mà màu của đại bàng sẽ khác nhau, tương ứng với các người chơi
		switch (NetworkID)
		{
		case 0:
			// màu vàng
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 2:
			// màu trắng
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 50, 100, 0, 50, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 3:
			// màu xanh
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 50, 100, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 4:
			// màu đỏ
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 50, 100, 50, 100, D3DXVECTOR2(25.f, 25.f)));
			break;
		}

		width = 50;
		height = 50;
		SetPosition(_pos);
	}
	~Eagle(){}

	void Draw()
	{
		// nếu đã chết thì không cần vẽ nữa
		if (IsDelete) 
			return;

		animation->Draw(GetPosition());
	}

	void Read(InputMemoryBitStream &is) override
	{
		// chỉ đọc máu của đại bàng
		is.Read(HP, 4); // dao động từ 0 - 10 => 4 bit

		if (HP == 0)
			IsDelete = true;
	}
};

