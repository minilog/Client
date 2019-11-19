#pragma once

#include "Entity.h"
#include "Explosion.h"

class NPC :public Entity
{
	const float speed = 150.f;
	Direction direction; // hướng di chuyển
	D3DXVECTOR2 receivedPosition;

public:
	NPC(int _entityID);
	~NPC();
	void Update(float _dt) override;
	void Draw() override;
	void CheckCollision(Entity* e);
	void Read(InputMemoryBitStream& is, bool _canReceive);
	void ApplyVelocity(); // gọi trước khi gọi CheckCollision
	void ZeroVelocity() { velocity = D3DXVECTOR2(0, 0); }

// các biến và hàm hỗ trợ
private:
	Animation* leftAnimation;
	Animation* rightAnimation;
	Animation* upAnimation;
	Animation* downAnimation;

	Animation *currentAnimation; // animation hiện tại

	void ApplyAnimation();
};

