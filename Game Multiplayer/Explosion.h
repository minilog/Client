#pragma once

#include "Entity.h"
#include "SpriteList.h"

class Explosion : Entity
{	
	float existTime = 0.f; // thời gian tồn tại của vụ nổ
	float count_existTime = 0.f;
	bool isActive = false; // đang hoạt động hay không

	Animation* animation;
public:
	Explosion(D3DXVECTOR2 _pos, bool _isBig)
	{
		// khởi tạo animation, mỗi frame 0.2 giây
		animation = new Animation(0.2f);
		// animation nổ bé
		if (!_isBig)
		{ 
			existTime = 0.6f;
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
			existTime = 0.4f;
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 96 + 64, 64, 64 + 64,
				D3DXVECTOR2(32.f, 32.f)));
			animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 160, 160 + 64, 64, 64 + 64,
				D3DXVECTOR2(32.f, 32.f)));
		}

		SetPosition(_pos);
	}
	~Explosion() {}

	void Update(float _dt)
	{
		// nếu không hoạt động => không cập nhật
		if (!isActive)
			return;

		// cập nhật animation của vụ nổ
		animation->Update(_dt);

		// đếm => đặt lại trạng thái không hoạt động
		count_existTime += _dt;
		if (count_existTime > existTime)
		{
			isActive = false;
			count_existTime = 0.f;
		}
	}

	void Draw()
	{
		// nếu không hoạt động, không vẽ
		if (!isActive)
			return;

		animation->Draw(GetPosition());
	}
};

