#pragma once

#include "Entity.h"
#include "Animation.h"

class Item : public Entity
{
	const float existTime = 7.0f;

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

		// nếu còn 2 giây tồn tại, vẽ 1 lần mỗi 5 frame
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

// các biến và hàm hỗ trợ
private:
	float count_existTime = 0.f; // đếm
	int flashingTime = 0; // giúp vẽ nhấp nháy
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

