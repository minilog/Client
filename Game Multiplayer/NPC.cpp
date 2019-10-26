#include "NPC.h"

#include "SpriteList.h"

NPC::NPC(int _networkID)
{
	// gán địa chỉ ID network
	NetworkID = _networkID;

	Type = ET_NPC;

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

	// chiều rộng, chiều cao xét va chạm
	width = 32.f;
	height = 32.f;
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
	// nếu đã bị tiêu diệt => không cập nhật
	if (IsDelete)
		return;

	x += vx * _dt;
	y += vy * _dt;
}

void NPC::Draw()
{
	currentAnimation->Draw(GetPosition());
}

void NPC::Read(InputMemoryBitStream& _is)
{
	Entity::Read(_is);

	int _direction = 0;
	_is.Read(_direction, 3); // direction dao động từ 0 - 5 =>  3 bit

	lastHealth = health;

	_is.Read(health, 2); // máu dao động từ 0 - 2 => 2 bit

	SetDirection((Direction)_direction);

	if (health == 0)
	{
		IsDelete = true;
		SetPosition(D3DXVECTOR2(-30.f, -30.f)); // tọa độ ở ngoài map
	}
	else
	{
		IsDelete = false;
	}
}

void NPC::CollisionWith(Entity* _en)
{
	if (_en->Type == ET_Player || 
		_en->Type == ET_NPC)
	{
		vx = 0;
		vy = 0;
	}
}

bool NPC::CheckCreateAnim()
{
	return (health == 0) && (lastHealth != 0);
}

void NPC::SetDirection(Direction _dir)
{
	// thay đổi vận tốc và animation đựa theo hướng di chuyển
	direction = _dir;
	switch (direction)
	{
	case D_Left:
		vx = -200.f;
		vy = 0.f;
		currentAnimation = leftAnimation;
		break;
	case D_Right:
		vx = 200.f;
		vy = 0.f;
		currentAnimation = rightAnimation;
		break;
	case D_Up:
		vx = 0.f;
		vy = -200.f;
		currentAnimation = upAnimation;
		break;
	case D_Down:
		vx = 0.f;
		vy = 200.f;
		break;
	default:
		break;
	}
}