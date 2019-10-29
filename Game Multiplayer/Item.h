#pragma once

#include "Entity.h"
#include "Animation.h"

class Item : public Entity
{
	const float existTime = 7.f;
	float count_existTime = 0.f;
	int flashingTime = 0;
protected:
	Animation* animation;

protected:
	void BaseInit(D3DXVECTOR2 _pos)
	{
		position = _pos;
		width = 32;
		height = 32;
		animation = new Animation();
	}
public:
	Item() {}
	~Item() {}

	void Update(float _dt)
	{
		count_existTime += _dt;
		if (count_existTime >= existTime)
		{
			IsDelete = true;
		}
	}

	void Draw() override
	{
		if (IsDelete)
			return;

		// nếu còn 2 giây tồn tại, vẽ 1 lần mỗi 5 frame
		if (existTime - count_existTime < 2)
		{
			flashingTime++;
			if (flashingTime == 5)
			{
				animation->Draw(GetPosition());
				flashingTime = 0;
			}
		}
		else
		{
			animation->Draw(GetPosition());
		}
	}

	void MakeCollision(Entity* _en) override
	{
		IsDelete = true;
	}

	void Read(InputMemoryBitStream& _is) override
	{
	}
};

