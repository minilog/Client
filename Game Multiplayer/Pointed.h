#pragma once

#include "Entity.h"
#include "Animation.h"

namespace GIVEPOINT {
	const RECT ONE_HUNDRED = RECT{ 5,0, 32,14 };
	const RECT TWO_HUNDRED = RECT{ 37,0, 65,14 };
	const RECT THREE_HUNDRED = RECT{ 70,0, 98,14 };
	const RECT FOUR_HUNDRED = RECT{ 103,0,131,14 };
	const RECT FIVE_HUNDRED = RECT{ 136,0,164,14 };
}
class Pointed : Entity
{
	Animation* OneHundredAnimation;
	Animation* TwoHundredAnimation;
	float count_Exist = 2.f;

public:
	Pointed()
	{
		OneHundredAnimation = new Animation();
		OneHundredAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Point, 5, 32, 0, 14,
			D3DXVECTOR2(14.f, 7.f)));

		TwoHundredAnimation = new Animation();
		TwoHundredAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->PlusBox, 37, 65, 0, 14,
			D3DXVECTOR2(14.f, 7.f)));

		position = D3DXVECTOR2(250, 250);
	}
	~Pointed(){}

	void Update(float dt)
	{
		if (IsDelete)
			return;

		count_Exist -= dt;
		if (count_Exist < 0)
		{
			count_Exist = 1.f;
			IsDelete = true;
		}
	}

	void Draw()
	{
		if (IsDelete)
			return;

		OneHundredAnimation->Draw(position);
	}

	void Read(InputMemoryBitStream& is, bool canReceive)
	{
		int x = 0, y = 0;
		
		is.Read(x, NBit_Position);
		is.Read(y, NBit_Position);

		if (canReceive)
		{
			D3DXVECTOR2 pos = D3DXVECTOR2(x / 10.f, y / 10.f);
			if (position.x != pos.x || position.y != pos.y)
			{
				IsDelete = false;
				position = pos;
			}
		}
	}
};

