#pragma once

#include "Entity.h"
#include <map>
#include "TCPSocket.h"
#include "Bullet.h"
#include <vector>
#include "GameLog.h"
#include "GameDefine.h"

using namespace std;

class Player : public Entity
{
	const float speed = 150.f;
	Direction direction = D_Stand;
	Direction lastDirection = D_Stand;
	Direction receiveDirection = D_Stand;
	int level = 1;

	const float time_BetweenShoots = 0.7f; // bên client sẽ là 0.7, server sẽ là 0.6
	float count_Shoot = 0.0f;
	D3DXVECTOR2 receivedPosition;
	D3DXVECTOR2 bestGuessPosition;
	const float flashDistance = 60.0f;
	std::vector<Explosion*> explosionList; // trỏ đến
	bool isShield = true;
public:
	int Score = 0;
	bool IsMy = false;

public:
	Player(int _ID);
	~Player();
	void Update(float _dt);
	void Draw();
	void DrawArrow();
	void CheckCollision(Entity* e);
	void Read(InputMemoryBitStream& _is, bool _canReceive, int receivedTime);
	void HandleKeyboard(std::map<int, bool> _keys, float _dt);
	void ApplyVelocity(); // vận tốc theo direction
	void ApplyVelocity_Compensation(); // diễn cho chính xác vị trí trên server
	void Update_Compensation(float dt);
	void ZeroVelocity() { velocity = D3DXVECTOR2(0, 0); }
	void AddExplosion(Explosion* e) { explosionList.push_back(e); }

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

	void SetAnimation(Direction _dir);
	void InitAnimation();
	void LogPosition() { GAMELOG("(%i, %i)", (int)position.x, (int)position.y); }

	D3DXVECTOR2 GetVelocityByDirection(Direction dir);
	int count_Log = 0;
};