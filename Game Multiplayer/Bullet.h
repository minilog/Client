﻿#pragma once

#include "Entity.h"
#include "Explosion.h"

class Bullet : public Entity
{
	const float speed = 400.f;
	Direction direction;	// hướng bay
	D3DXVECTOR2 receivedPosition;
	D3DXVECTOR2 destroyPosition;
	std::vector<Explosion*> explosionList; // trỏ đến
	
	Animation* leftAnimation;
	Animation* rightAnimation;
	Animation* upAnimation;
	Animation* downAnimation;
	Animation* currentAnimation; // trỏ đến 1 trong 4 animation để vẽ lên
public:
	int PlayerID = -1; // là của người chơi nào

public:
	Bullet(int _ID, int _playerID)
	{
		// gắn network ID và ID của người chơi sở hữu 
		ID = _ID;
		PlayerID = _playerID;
		Type = ET_Bullet;
		IsDelete = true;
		width = 6;
		height = 6;

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
	}
	
	~Bullet() 
	{
		// xóa các animation đã khởi tạo
		delete leftAnimation;
		delete rightAnimation;
		delete upAnimation;
		delete downAnimation;
	}

	void Update(float _dt) override
	{
		if (IsDelete)
			return;
		
		position += velocity * _dt;
	}

	void Draw() override
	{
		if (IsDelete)
			return;

		currentAnimation->Draw(position);
	}

	void MakeCollision(Entity* _en) override
	{
		IsDelete = true;
	}

	void Read(InputMemoryBitStream& _is, bool _canReceive);

	// thay đổi vận tốc và animation đựa theo hướng bay
	void SetDirection(Direction _dir)
	{
		direction = _dir;
		switch (direction)
		{
		case D_Left:
			currentAnimation = leftAnimation;
			break;
		case D_Right:
			currentAnimation = rightAnimation;
			break;
		case D_Up:
			currentAnimation = upAnimation;
			break;
		case D_Down:
			currentAnimation = downAnimation;
			break;
		}
	}

	void ApplyVelocity()
	{
		if (IsDelete)
			return;

		switch (direction)
		{
		case D_Left:
			velocity = D3DXVECTOR2(-speed, 0.f);
			break;
		case D_Right:
			velocity = D3DXVECTOR2(speed, 0.f);
			break;
		case D_Up:
			velocity = D3DXVECTOR2(0.f, -speed);
			break;
		case D_Down:
			velocity = D3DXVECTOR2(0.f, speed);
			break;
		default:
			break;
		}

		velocity += (receivedPosition - position) * 1.0f;
	}

	void AddExpolostion(Explosion* e)
	{
		explosionList.push_back(e);
	}
};

