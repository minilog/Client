#pragma once

#include "Entity.h"
#include "Animation.h"

class Item : public Entity
{
private:
	int flashingTime = 0;
protected:
	float existTime = 0.f;
	float count_existTime = 0.f;

	Animation* animation;

protected:
	void BaseInit(D3DXVECTOR2 _pos)
	{
		width = 32;
		height = 32;

		SetPosition(_pos);
	}
public:
	Item()
	{
		IsDelete = false;
		existTime = 7.f;
	}

	~Item() {}

	virtual void Update(float _dt)
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

	void CollisionWith(Entity* _e) override
	{
		IsDelete = true;
	}

	void Read(InputMemoryBitStream& _is) override
	{
		Entity::Read(_is);
		// còn đọc x, y nữa
		BaseInit(D3DXVECTOR2(x, y));
	}
};

