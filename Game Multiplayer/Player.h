#pragma once

#include "Entity.h"
#include <map>
#include "TCPSocket.h"
#include "Bullet.h"
#include <vector>
#include "GameDefine.h"

using namespace std;

class Player : public Entity
{
	const float speed1 = 200.f;
	const float speed2 = 250.f;
	Direction direction = D_Stand;
	Direction lastDirection = D_Stand;
	int level = 1;
	bool isMy = false;

public:
	Player(int _ID);
	~Player();
	void Update(float _dt);
	void Draw();
	void MakeCollision(Entity* _en) override { velocity = D3DXVECTOR2(0.f, 0.f); }
	void Read(InputMemoryBitStream& _is, bool _canReceive);
	void HandleKeyboard(std::map<int, bool> _keys);

// support 
private:
	Animation* leftAnimationLv01;
	Animation* rightAnimationLv01;
	Animation* upAnimationLv01;
	Animation* downAnimationLv01;
	Animation* leftAnimationLv02;
	Animation* rightAnimationLv02;
	Animation* upAnimationLv02;
	Animation* downAnimationLv02;
	Animation* leftAnimationLv03;
	Animation* rightAnimationLv03;
	Animation* upAnimationLv03;
	Animation* downAnimationLv03;

	Animation* currentAnimation; // animation hiện tại

	Animation* arrowAnimation; // animation mũi tên ở trên người chơi
	Animation* shieldAnimation; // animation khiên 
	Animation* spawnAnimation; // animation hồi sinh

	void SetAnimationByDirection(Direction _dir);
};