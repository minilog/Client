#pragma once

#include "Entity.h"

class NPC :public Entity
{
	Direction direction;
	int level = 1;
	int health = 2;
	int lastHealth = 2;
	float lastRandom = 0.f;

	Animation* leftAnimation;
	Animation* rightAnimation;
	Animation* upAnimation;
	Animation* downAnimation;

	Animation *currentAnimation; // animation hiện tại

public:
	NPC(int _networkID);
	~NPC();

	void Update(float _dt) override;
	void Draw() override;
	void Write(OutputMemoryBitStream& os) override {}
	void Read(InputMemoryBitStream& is) override;

	void CollisionWith(Entity* _en) override;
	bool CheckCreateAnim();
	void SetDirection(Direction _dir);
};

