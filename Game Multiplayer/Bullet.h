#pragma once
#include "Entity.h"

class Bullet : public Entity
{
	Direction direction;	// hướng di chuyển của viên đạn

	Animation* leftAnimation;
	Animation* rightAnimation;
	Animation* upAnimation;
	Animation* downAnimation;

	Animation* currentAnimation; // trỏ đến 1 trong 4 animation để vẽ lên

public:
	int NetworkPlayerID = -1; // -1: chưa xác định
	bool IsActive = false;

public:
	Bullet(int _networkID, int _networkPlayerID)
	{
		// gắn network ID và ID của người chơi sở hữu 
		NetworkPlayerID = _networkID;
		NetworkPlayerID = _networkPlayerID;

		// loại đối tượng viên đạn
		Type = ET_Bullet;

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
		// nếu không hoạt động => không cập nhật
		if (!IsActive)
			return;

		x += vx * _dt;
		y += vy * _dt;
	}

	void Draw()
	{
		// nếu không hoạt đông => không vẽ
		if (!IsActive)
			return;

		currentAnimation->Draw(GetPosition());
	}

	void Write(OutputMemoryBitStream& _os) override
	{
		_os.Write(IsActive);
	}

	void Read(InputMemoryBitStream& _is) override
	{
		Entity::Read(_is);
		_is.Read(IsActive);
	}

	void SetDirection(Direction _dir)
	{
		// thay đổi vận tốc và animation đựa theo hướng bay
		direction = _dir;
		switch (direction)
		{
		case D_Left:
			currentAnimation = leftAnimation;
			vx = -Define::BULLET_SPEED;
			vy = 0.f;
			break;
		case D_Right:
			currentAnimation = rightAnimation;
			vx = Define::BULLET_SPEED;
			vy = 0.f;
			break;
		case D_Up:
			currentAnimation = upAnimation;
			vx = 0.f;
			vy = -Define::BULLET_SPEED;
			break;
		case D_Down:
			currentAnimation = downAnimation;
			vx = 0.f;
			vy = Define::BULLET_SPEED;
			break;
		default:
			break;
		}
	}
};

