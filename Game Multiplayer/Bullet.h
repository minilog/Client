#pragma once
#include "Entity.h"

class Bullet : public Entity
{
	const float speed = 500.f;
	Direction direction;	// hướng di chuyển của viên đạn

	Animation* leftAnimation;
	Animation* rightAnimation;
	Animation* upAnimation;
	Animation* downAnimation;

	Animation* currentAnimation; // trỏ đến 1 trong 4 animation để vẽ lên

public:
	int PlayerID = -1; // -1: chưa xác định

public:
	Bullet(int _entityID, int _playerID)
	{
		// gắn network ID và ID của người chơi sở hữu 
		EntityID = _entityID;
		PlayerID = _playerID;

		Type = ET_Bullet;
		IsDelete = true;

		// khởi tạo 4 animation
		leftAnimation = new Animation();
		leftAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 148, 148 + 8, 12, 12 + 6,
			D3DXVECTOR2(4.f, 3.f)));
		rightAnimation = new Animation();
		rightAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 180, 180 + 8, 12, 12 + 6,
			D3DXVECTOR2(4.f, 3.f)));
		upAnimation = new Animation();
		upAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 134, 134 + 6, 12, 12 + 8,
			D3DXVECTOR2(3.f, 4.f)));
		downAnimation = new Animation();
		downAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 166, 166 + 6, 12, 12 + 8,
			D3DXVECTOR2(3.f, 4.f)));

		// thiết lập animation ban đầu
		currentAnimation = leftAnimation;

		// thiết lập chiều rộng, chiều cao va chạm => 6, 6
		width = 6;
		height = 6;
	}
	~Bullet() 
	{
		// xóa các animation đã khởi tạo
		delete leftAnimation;
		delete rightAnimation;
		delete upAnimation;
		delete downAnimation;
	}

	void Update(float _dt) 
	{
		if (IsDelete)
			return;
		
		position += velocity * _dt;
	}

	void Draw()
	{
		if (IsDelete)
			return;

		currentAnimation->Draw(GetPosition());
	}

	void Read(InputMemoryBitStream& _is) override
	{
	}

	// thay đổi vận tốc và animation đựa theo hướng bay
	void SetDirection(Direction _dir)
	{
		direction = _dir;
		switch (direction)
		{
		case D_Left:
			currentAnimation = leftAnimation;
			velocity = D3DXVECTOR2(-speed, 0.f);
			break;
		case D_Right:
			currentAnimation = rightAnimation;
			velocity = D3DXVECTOR2(speed, 0.f);
			break;
		case D_Up:
			currentAnimation = upAnimation;
			velocity = D3DXVECTOR2(0.f, -speed);
			break;
		case D_Down:
			currentAnimation = downAnimation;
			velocity = D3DXVECTOR2(0.f, speed);
			break;
		default:
			break;
		}
	}
};

