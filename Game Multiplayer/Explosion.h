#pragma once

#include "Entity.h"
#include "SpriteList.h"

class Explosion : public Entity
{	
	float existTime = 0.f; // thời gian tồn tại của vụ nổ
	float count_Exist = 0.f;

	Animation* animation;
public:
	Explosion(bool _isBig)
	{
		IsDelete = true;

		// khởi tạo animation, mỗi frame 0.2 giây
		animation = new Animation(0.1f);
		// animation nổ bé
		if (!_isBig)
		{ 
			existTime = 0.28f;
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 0 + 32, 64, 64 + 32,
				D3DXVECTOR2(16.f, 16.f)));
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 32, 32 + 32, 64, 64 + 32,
				D3DXVECTOR2(16.f, 16.f)));
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 64, 64 + 32, 64, 64 + 32,
				D3DXVECTOR2(16.f, 16.f)));
		}
		// animation nổ lớn
		else
		{
			existTime = 0.18f;
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 96 + 64, 64, 64 + 64,
				D3DXVECTOR2(32.f, 32.f)));
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 160, 160 + 64, 64, 64 + 64,
				D3DXVECTOR2(32.f, 32.f)));
		}

		count_Exist = existTime;
	}
	~Explosion() {}

	void Update(float _dt)
	{
		if (IsDelete)
			return;

		animation->Update(_dt);

		count_Exist -= _dt;
		if (count_Exist < 0)
		{
			count_Exist = existTime;
			IsDelete = true;
		}
	}

	void Draw()
	{
		if (IsDelete)
			return;

		animation->Draw(GetPosition());
	}

	void Spawn(D3DXVECTOR2 _pos)
	{
		position = _pos;
		animation->Reset();
		IsDelete = false;
	}
};

