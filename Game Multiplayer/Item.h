#pragma once

#include "Entity.h"
#include "Animation.h"

class Item : public Entity
{
	const float existTime = 7.0f;
	float count_existTime = 0.0f; // đếm
	int flashingTime = 0; // giúp vẽ nhấp nháy

public:
	Item() {}
	~Item() { delete animation; }

	void Update(float _dt) override
	{
		if (IsDelete)
			return;

		count_existTime += _dt;
		if (count_existTime >= existTime)
		{
			IsDelete = true;
			count_existTime = 0.f;
		}
	}

	void Draw() override
	{
		if (IsDelete)
			return;

		// nếu còn 1.5 giây tồn tại, vẽ nhấp nháy
		if (existTime - count_existTime < 1.5f)
		{
			flashingTime++;
			if (flashingTime > 5)
			{
				animation->Draw(position);
				//flashingTime = 0;
			}
			if (flashingTime == 10)
			{
				flashingTime = 0;
			}
		}
		else
		{
			animation->Draw(position);
		}
	}

	void MakeCollision(Entity* _en) override
	{
		IsDelete = true;
	}

	void Read(InputMemoryBitStream& is, bool canReceive)
	{
		int posX = 0;
		int posY = 0;
		bool _isDelete = false;

		is.Read(posX, NBit_Position);
		is.Read(posY, NBit_Position);
		is.Read(_isDelete);

		if (canReceive)
		{
			if (IsDelete && !_isDelete)
			{
				count_existTime = 0;
			}
			IsDelete = _isDelete;

			position = D3DXVECTOR2(posX / 10.0f, posY / 10.0f);
		}
	}

// các biến và hàm hỗ trợ
protected:
	Animation* animation;

	void BaseInit(D3DXVECTOR2 _pos)
	{
		position = _pos;
		width = 32;
		height = 32;
		animation = new Animation();
	}
};

