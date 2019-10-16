#pragma once
#include "Camera.h"
#include "Entity.h"
#include <map>
#include "TCPSocket.h"
#include "Bullet.h"
#include <vector>
#include "GameDefine.h"


enum Action
{
	Idle,
	GoLeft,
	GoRight,
	GoUp,
	GoDown,
	Fight
};
class Player : public Entity
{
public:
	Player();
	Player(int id) { ID = id; }
	~Player();

	void Update(float dt);

	void Draw(D3DXVECTOR3 position = D3DXVECTOR3(), RECT sourceRect = RECT(), D3DXVECTOR2 scale = D3DXVECTOR2(), 
		D3DXVECTOR2 transform = D3DXVECTOR2(), float angle = 0, D3DXVECTOR2 rotationCenter = D3DXVECTOR2(), 
		D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void CollideWith_World();

	bool isMe = false;
	string mName = "";
	Action mAction = Idle;
	Action mLastAction = Action::GoDown;
	int mHeal = 10;
	int last_mHeal = 0;

	//bool isLose = false;
	bool isDelete = false;
	int last_time_die = 0;


	int lastFire = 0; //Time last Fight
	int last_move_time = 0;
	int mLevel = 1;

	D3DXVECTOR2 last_position = D3DXVECTOR2(0, 0);

	D3DXVECTOR2 position_score = D3DXVECTOR2(0, 0);
	int mScore_send = 0;
	int last_id_packet = 0;
	bool isLose = false;

	bool Check_to_create_anim();
	bool is_protect = false;
	void ActiveShield();
	void SetSpawn();
	RECT GetBound();
	void CollisionWith(Entity* en) override;
	void onSetID(int ID);
	void Write(OutputMemoryBitStream& os) override;
	void OnsetLevel(int m_level);
	void Read(InputMemoryBitStream& is) override;
	//xu ly input
	//gom 256 key tuong ung true = dang dc nhan, false = khong dc nhan
	void HandleKeyboard(std::map<int, bool> keys);
	Sprite * mCurrentSprite;
	int mScore = 0;
protected:


	//Animation   *mCurrentAnimation;
	Animation* shield;
	Animation* spawn;

	Sprite * mLeftSprite;
	Sprite * mUpSprite;
	Sprite * mRightSprite;
	Sprite * mDownSprite;
	Sprite *m_top_sprite;

	bool is_respaw = false;
	int last_time_spawn = 0;


	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void Stand();
};


