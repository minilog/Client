#pragma once

#include "Entity.h"
#include "Explosion.h"

class NPC :public Entity
{
	const float speed = 200.f;
	Direction direction; // hướng di chuyển
	bool isExplosion = false;
	Explosion* explosion; // trỏ đến đối tượng trong Scene

public:
	NPC(int _entityID);
	~NPC();

	void Update(float _dt) override;
	void Draw() override;
	void MakeCollision(Entity* _en) override { velocity = D3DXVECTOR2(0.f, 0.f); }
	void Read(InputMemoryBitStream& is) override {}

	void SetExplosion(Explosion *_explosion) { explosion = _explosion; }
	void SetDirection(Direction _dir);

// các biến và hàm hỗ trợ
private:
	Animation* leftAnimation;
	Animation* rightAnimation;
	Animation* upAnimation;
	Animation* downAnimation;

	Animation *currentAnimation; // animation hiện tại
};

