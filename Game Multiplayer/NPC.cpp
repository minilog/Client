#include "NPC.h"



NPC::NPC(int id)
{
	ID = id;
	mUpSprite = new Sprite("Resource files/AI_tank.png", RECT{ 2,4,2 + 32,4 + 32 }, 0, 0, D3DXCOLOR(255, 0, 255, 255));
	mLeftSprite = new Sprite("Resource files/AI_tank.png", RECT{ 82,2,82 + 32,2 + 32 }, 0, 0, D3DXCOLOR(255, 0, 255, 255));
	mDownSprite = new Sprite("Resource files/AI_tank.png", RECT{ 156,2,156 + 32,2 + 32 }, 0, 0, D3DXCOLOR(255, 0, 255, 255));
	mRightSprite = new Sprite("Resource files/AI_tank.png", RECT{ 233,2,233 + 32,2 + 32 }, 0, 0, D3DXCOLOR(255, 0, 255, 255));


	this->vx = 0;
	this->vy = 0;

	Tag = Entity::npc;
	mCurrentSprite = mUpSprite;
}


NPC::~NPC()
{
}

void NPC::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle,
	D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	mCurrentSprite->SetPosition(this->GetPosition());
	mCurrentSprite->Draw(D3DXVECTOR3(posX, posY, 0));
}

void NPC::CollideWith_World()
{
	vx = 0;
	vy = 0;
}

void NPC::CollisionWith(Entity* en)
{
	if (en->Tag == EntityTypes::player || en->Tag == EntityTypes::npc)
	{
		vx = 0;
		vy = 0;
	}
}

bool NPC::Check_to_create_anim()
{
	if (mHeal == 0 && last_mHeal != 0)
		return true;
	return false;
}

void NPC::Read(InputMemoryBitStream& is)
{
	last_position = GetPosition();

	Entity::Read(is);
	int a = 0;
	is.Read(a, Define::bitofID);
	dir = (Direction)a;
	OnsetDir();
	last_mHeal = mHeal;
	is.Read(mHeal, Define::bitofTypePacket);
	if (mHeal == 0) isDelete = true;
	else isDelete = false;
}

void NPC::Update(float dt)
{
	if (isDelete)
	{
		SetPosition(-30, -30);
		return;
	}
	//OnsetDir();
	Entity::Update(dt);
}

RECT NPC::GetBound()
{
	RECT rect;
	rect.left = this->posX - 32 / 2;
	rect.right = rect.left + 23;
	rect.top = this->posY - 32 / 2;
	rect.bottom = rect.top + 32;
	return rect;
}

void NPC::Write(OutputMemoryBitStream& os)
{
}

void NPC::OnsetDir()
{
	switch (dir)
	{
	case left:MoveLeft(); mCurrentSprite = mLeftSprite; break;
	case right:MoveRight(); mCurrentSprite = mRightSprite; break;
	case up:MoveUp(); mCurrentSprite = mUpSprite; break;
	case down:MoveDown(); mCurrentSprite = mDownSprite; break;
	}
}
void NPC::MoveLeft()
{
	dir = left;
	this->SetVx(-200.0f);
	this->SetVy(0);
}

void NPC::MoveRight()
{
	dir = right;
	this->SetVx(200.0f);
	this->SetVy(0);
}

void NPC::MoveUp()
{
	dir = up;
	this->SetVx(0);
	this->SetVy(200.0f);

}

void NPC::MoveDown()
{
	dir = down;
	this->SetVx(0);
	this->SetVy(-200.0f);
}

void NPC::IDLE()
{
	this->SetVx(0);
	this->SetVy(0);
}
