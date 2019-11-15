﻿#pragma once

#include "Entity.h"
#include "Explosion.h"

class NPC :public Entity
{
	const float speed = 150.f;
	Direction direction; // hướng di chuyển

public:
	NPC(int _entityID);
	~NPC();
	void Update(float _dt) override;
	void Draw() override;
	void MakeCollision(Entity* _en) override { velocity = D3DXVECTOR2(0.f, 0.f); }
	void Read(InputMemoryBitStream& is, bool _canReceive);
	void SetDirection(Direction _dir); // thay đổi vận tốc và animation đựa theo hướng di chuyển

// các biến và hàm hỗ trợ
private:
	Animation* leftAnimation;
	Animation* rightAnimation;
	Animation* upAnimation;
	Animation* downAnimation;

	Animation *currentAnimation; // animation hiện tại
};

