#include "NPC.h"
#include "SpriteList.h"
#include "GameCollision.h"

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

void NPC::CheckCollision(Entity * e)
{
	if (IsDelete)
		return;

	if (e->Type == ET_MetalBrick || e->Type == ET_NormalBrick || e->Type == ET_Boundary || e->Type == ET_Water)
	{
		CollisionResult cR = GameCollision::Get_CollisionResult(this, e);
		if (cR.IsCollided)
		{
			if (cR.Side == CS_Left)
			{
				position.x += (float)(cR.Rect.right - cR.Rect.left) + 1;
			}
			else if (cR.Side == CS_Right)
			{
				position.x -= (float)(cR.Rect.right - cR.Rect.left) - 1;
			}
			else if (cR.Side == CS_Top)
			{
				position.y += (float)(cR.Rect.bottom - cR.Rect.top) + 1;
			}
			else if (cR.Side == CS_Bottom)
			{
				position.y -= (float)(cR.Rect.bottom - cR.Rect.top) - 1;
			}
		}
	}
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
		// spawn nổ
		if (!IsDelete && _isDelete)
		{
			for (auto e : explosionList)
			{
				if (e->IsDelete)
				{
					e->Spawn(position);
					break;
				}
			}
		}

		if (IsDelete)
			position = D3DXVECTOR2(x / 10.0f, y / 10.0f);

		receivedPosition = D3DXVECTOR2(x / 10.0f, y / 10.0f);
		IsDelete = _isDelete;
		direction = dir;
		ApplyAnimation();

		// nếu vị trí nhận được quá xa so với hiện tại => tốc biến
		D3DXVECTOR2 distance = position - receivedPosition;
		if (sqrt(distance.x * distance.x + distance.y * distance.y) >= 80.f)
		{
			position = receivedPosition;
		}
	}
}

void NPC::ApplyAnimation()
{
	switch (direction)
	{
	case D_Left:
		currentAnimation = leftAnimation;
		break;
	case D_Right:
		currentAnimation = rightAnimation;
		break;
	case D_Up:
		currentAnimation = upAnimation;
		break;
	case D_Down:
		currentAnimation = downAnimation;
		break;
	default:
		break;
	}
}

void NPC::ApplyVelocity()
{
	switch (direction)
	{
	case D_Stand:
		velocity = D3DXVECTOR2(0.f, 0.f);
		break;
	case D_Left:
		velocity = D3DXVECTOR2(-speed, 0.f);
		break;
	case D_Right:
		velocity = D3DXVECTOR2(speed, 0.f);
		break;
	case D_Up:
		velocity = D3DXVECTOR2(0.f, -speed);
		break;
	case D_Down:
		velocity = D3DXVECTOR2(0.f, speed);
		break;
	}

	velocity += (receivedPosition - position) * 3;
}
