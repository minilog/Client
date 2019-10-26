#pragma once
#include "Camera.h"
#include "Entity.h"
#include <map>
#include "TCPSocket.h"
#include "Bullet.h"
#include <vector>
#include "GameDefine.h"

using namespace std;

class Player : public Entity
{
	Direction direction = D_Stand;
	Direction lastDirection = D_Stand;

	bool isSpawn = false;
	int lastTimeSpawn = 0;

public:
	bool IsMe = false; // tôi là người điều khiển ?
	string Name = "";
	int Health = 2;
	int LastHealth = 2;
	int LastTimeDie = 0;
	int LastFire = 0;
	int LastMoveTime = 0;
	int Level = 1;
	D3DXVECTOR2 positionScore = D3DXVECTOR2(0, 0);
	int Score_Send = 0;
	int LastIDPacket = 0;
	bool isLose = false;
	int Score = 0;
	bool IsProtect = false;

public:
	Player(int _networkID);
	~Player();

	void Update(float _dt);
	void Draw();
	void Write(OutputMemoryBitStream& _os) override;
	void Read(InputMemoryBitStream& _is) override;

	void CollideWith_World();
	bool CheckCreateAnim();

	void CollisionWith(Entity* _en) override;
	void HandleKeyboard(std::map<int, bool> _keys);

// các biến và hàm hỗ trợ
private:
	// các animation di chuyển
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

	void InitAnimation();
	void SetAnimationByDirection(Direction _dir);
};