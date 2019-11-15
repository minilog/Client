#include "NPC.h"

#include "SpriteList.h"

NPC::NPC(int _ID)
{
	ID = _ID;

	Type = ET_NPC;
	IsDelete = true;
	width = 28;
	height = 28;

	// khởi tạo các animation
	leftAnimation = new Animation();
	leftAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 82, 82 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	rightAnimation = new Animation();
	rightAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 233, 233 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	upAnimation = new Animation();
	upAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 2, 2 + 32, 4, 4 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	downAnimation = new Animation();
	downAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 156, 156 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));

	// mặc định animation ban đầu
	currentAnimation = leftAnimation;
}

NPC::~NPC()
{
	// xóa các animation đã khởi tạo new
	delete leftAnimation;
	delete rightAnimation;
	delete upAnimation;
	delete downAnimation;
}

void NPC::Update(float _dt)
{
	if (IsDelete)
		return;

	position += velocity * _dt;
}

void NPC::Draw()
{
	if (IsDelete)
		return;

	currentAnimation->Draw(GetPosition());
}

void NPC::Read(InputMemoryBitStream & is, bool _canReceive)
{
	int x = 0;
	int y = 0;
	Direction dir = D_Stand;
	bool _isDelete = false;

	is.Read(x, NBit_Position);
	is.Read(y, NBit_Position);
	is.Read(dir, NBit_Direction);
	is.Read(_isDelete);

	if (_canReceive)
	{
		position = D3DXVECTOR2(x / 10.0f, y / 10.0f);
		SetDirection(dir);
		IsDelete = _isDelete;
	}
}

void NPC::SetDirection(Direction _dir)
{
	direction = _dir;
	switch (direction)
	{
	case D_Left:
		currentAnimation = leftAnimation;
		velocity = D3DXVECTOR2(-speed, 0.f);
		break;
	case D_Right:
		currentAnimation = rightAnimation;
		velocity = D3DXVECTOR2(speed, 0.f);
		break;
	case D_Up:
		currentAnimation = upAnimation;
		velocity = D3DXVECTOR2(0.f, - speed);
		break;
	case D_Down:
		currentAnimation = downAnimation;
		velocity = D3DXVECTOR2(0.f, speed);
		break;
	case D_Stand:
		velocity = D3DXVECTOR2(0, 0);
		break;
	default:
		break;
	}
}