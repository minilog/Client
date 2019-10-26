#pragma once
#include "GameGlobal.h"
#include "MemoryBitStream.h"
#include "Animation.h"
#include "SpriteList.h"

enum EntityTypes
{
	ET_None,
	ET_Player, ET_Bullet,
	ET_Boundary, ET_NormalBrick, ET_MetalBrick, ET_Water,
	ET_ProtectItem, ET_UpgradeItem,
	ET_NPC,
	ET_Eagle
};

class Entity
{
protected:
	float x, y;
	float vx = 0, vy = 0;
	float width = 0, height = 0;
public:
	int NetworkID = -1; // -1: chưa xác định
	EntityTypes Type = ET_None; //Tag de nhan dien loai Entity
	bool IsDelete = false;

public:
	Entity() {}
	~Entity() {}

	virtual void Update(float dt) {}
	virtual void Draw() {}
	virtual void CollisionWith(Entity* en) {}
	virtual void Write(OutputMemoryBitStream &os);
	virtual void Read(InputMemoryBitStream& _is){}

	virtual RECT GetBound();
	D3DXVECTOR2 GetPosition();
	virtual void SetPosition(D3DXVECTOR2 pos);
	virtual void AddPosition(D3DXVECTOR2 pos);
	virtual void SetWidth(int width);
	virtual int GetWidth();
	virtual void SetHeight(int height);
	virtual int GetHeight();
	virtual float GetVx();
	virtual void SetVx(float vx);
	virtual float GetVy();
	virtual void SetVy(float vy);
	virtual void AddVx(float vx);
	virtual void AddVy(float vy);
};

enum Direction
{
	D_Stand,
	D_Left,
	D_Right,
	D_Up,
	D_Down
};