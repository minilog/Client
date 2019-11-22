#include "Player.h"
#include "GameLog.h"
#include "SpriteList.h"
#include "TimeServer.h"
#include "GameCollision.h"

Player::Player(int _ID)
{
	ID = _ID;
	Type = ET_Player;
	InitAnimation();
	currentAnimation = upAnimationLv01;
	width = 32;
	height = 32;

	// set vị trí ban đầu = ID người chơi
	switch (_ID)
	{
	case 0:
		position = D3DXVECTOR2(55.f, 55.f);
		break;
	case 1:
		position = D3DXVECTOR2(745.f, 55.f);
		break;
	case 2:
		position = D3DXVECTOR2(55.f, 745.f);
		break;
	case 3:
		position = D3DXVECTOR2(745.f, 745.f);
		break;
	default:
		break;
	}
	bestGuessPosition = position;
	receivedPosition = position;

	if (_ID == GameGlobal::Socket->PlayerID)
	{
		isMy = true;
	}
}

Player::~Player()
{
	// xóa các animation đã khởi tạo new
	delete leftAnimationLv01;
	delete rightAnimationLv01;
	delete upAnimationLv01;
	delete downAnimationLv01;
	delete leftAnimationLv02;
	delete rightAnimationLv02;
	delete upAnimationLv02;
	delete downAnimationLv02;
	delete leftAnimationLv03;
	delete rightAnimationLv03;
	delete upAnimationLv03;
	delete downAnimationLv03;
	delete shieldAnimation;
	delete spawnAnimation;
	delete arrowAnimation;
}

void Player::Read(InputMemoryBitStream & _is, bool _canReceive, int receivedTime)
{
	int x = 0;
	int y = 0;
	Direction _dir = D_Stand;
	Direction shootDir = D_Stand;
	bool _isDelete = false;

	_is.Read(x, NBit_Position);
	_is.Read(y, NBit_Position);
	_is.Read(_dir, NBit_Direction);
	_is.Read(shootDir, NBit_Direction);
	_is.Read(_isDelete);

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

		if (!isMy)
		{
			direction = _dir;
			SetAnimation(shootDir);
		}

		receivedPosition = D3DXVECTOR2(x / 10.f, y / 10.f);
		int timeDistance = TimeServer::Instance()->GetServerTime() - receivedTime;
		bestGuessPosition = receivedPosition + GetVelocityByDirection(_dir) * (timeDistance / 1000.0f);

		// nếu vị trí nhận được quá xa so với hiện tại => tốc biến
		D3DXVECTOR2 distance = position - receivedPosition;
		if (sqrt(distance.x * distance.x + distance.y * distance.y) >= flashDistance)
		{
			position = receivedPosition;
			direction = _dir;
			lastDirection = _dir;
		}

		if (IsDelete)
		{
			position = receivedPosition;
		}
		IsDelete = _isDelete;
	}
}

void Player::Update(float _dt)
{
	if (IsDelete)
		return;

	position += velocity * _dt;
}

void Player::HandleKeyboard(std::map<int, bool> keys, float _dt)
{
	if (IsDelete || !isMy)
		return;

	// shoot
	count_Shoot -= _dt;
	if (count_Shoot < 0 && keys[VK_SPACE])
	{
		count_Shoot = time_BetweenShoots;
		GAMELOG("Send Shoot");

		// send shoot
		int sTime = TimeServer::Instance()->GetServerTime();
		for (int i = 0; i < 3; i++)
		{
			OutputMemoryBitStream os;
			os.Write(PT_PlayerShoot, NBit_PacketType);
			os.Write(sTime, NBit_Time); // write server time

			GameGlobal::Socket->Send(os);
		}
	}

	// bàn phím thay đổi direction
	if (keys[VK_LEFT])
	{
		direction = D_Left;
	}
	else if (keys[VK_RIGHT])
	{
		direction = D_Right;
	}
	else if (keys[VK_UP])
	{
		direction = D_Up;
	}
	else if (keys[VK_DOWN])
	{
		direction = D_Down;
	}
	else
	{
		direction = D_Stand;
	}
	
	SetAnimation(direction);

	if (direction != lastDirection)
	{
		// send input
		int sTime = TimeServer::Instance()->GetServerTime();
		for (int i = 0; i < 5; i++)
		{
			OutputMemoryBitStream os;

			os.Write(PT_PlayerInput, NBit_PacketType);
			os.Write(sTime, NBit_Time); // write server time
			os.Write(direction, NBit_Direction);

			GameGlobal::Socket->Send(os);
		}
	}

	lastDirection = direction;
}

void Player::SetAnimation(Direction _dir)
{
	if (level == 1)
	{
		switch (_dir)
		{
		case D_Left:
			currentAnimation = leftAnimationLv01;
			break;
		case D_Right:
			currentAnimation = rightAnimationLv01;
			break;
		case D_Up:
			currentAnimation = upAnimationLv01;
			break;
		case D_Down:
			currentAnimation = downAnimationLv01;
			break;
		default:
			break;
		}
	}
	else if (level == 2)
	{
		switch (_dir)
		{
		case D_Left:
			currentAnimation = leftAnimationLv02;
			break;
		case D_Right:
			currentAnimation = rightAnimationLv02;
			break;
		case D_Up:
			currentAnimation = upAnimationLv02;
			break;
		case D_Down:
			currentAnimation = downAnimationLv02;
			break;
		default:
			break;
		}
	}
	else if (level == 3)
	{
		switch (_dir)
		{
		case D_Left:
			currentAnimation = leftAnimationLv03;
			break;
		case D_Right:
			currentAnimation = rightAnimationLv03;
			break;
		case D_Up:
			currentAnimation = upAnimationLv03;
			break;
		case D_Down:
			currentAnimation = downAnimationLv03;
			break;
		default:
			break;
		}
	}
}

void Player::InitAnimation()
{
	leftAnimationLv01 = new Animation();
	rightAnimationLv01 = new Animation();
	upAnimationLv01 = new Animation();
	downAnimationLv01 = new Animation();
	leftAnimationLv02 = new Animation();
	rightAnimationLv02 = new Animation();
	upAnimationLv02 = new Animation();
	downAnimationLv02 = new Animation();
	leftAnimationLv03 = new Animation();
	rightAnimationLv03 = new Animation();
	upAnimationLv03 = new Animation();
	downAnimationLv03 = new Animation();

	// animation xe tăng của người chơi 0
	if (ID == 0)
	{
		leftAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 82, 82 + 32, 2, 2 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		rightAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 32, 2, 2 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		upAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 4, 4 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		downAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 32, 2, 2 + 32,
			D3DXVECTOR2(16.f, 16.f)));

		leftAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 76, 76 + 37, 196, 196 + 32,
			D3DXVECTOR2(18.f, 16.f)));
		rightAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 195, 195 + 30,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 192, 192 + 36,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 195, 195 + 34,
			D3DXVECTOR2(15.f, 17.f)));

		leftAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 77, 77 + 37, 79, 79 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		rightAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 79, 79 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 77, 77 + 37,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 78, 78 + 37,
			D3DXVECTOR2(15.f, 18.f)));
	}
	// animation xe tăng của người chơi 1
	else if (ID == 1)
	{
		leftAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 389, 389 + 32, 2, 2 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		rightAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 32, 2, 2 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		upAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 4, 4 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		downAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 32, 2, 2 + 32,
			D3DXVECTOR2(16.f, 16.f)));

		leftAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 196, 196 + 32,
			D3DXVECTOR2(18.f, 16.f)));
		rightAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 195, 195 + 30,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 192, 192 + 36,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 195, 195 + 34,
			D3DXVECTOR2(15.f, 17.f)));

		leftAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 79, 79 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		rightAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 79, 79 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 77, 77 + 37,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 78, 78 + 37,
			D3DXVECTOR2(15.f, 18.f)));
	}
	// animation xe tăng của người chơi 2
	else if (ID == 2)
	{
		leftAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 82, 82 + 32, 310, 310 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		rightAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 32, 310, 310 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		upAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 312, 312 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		downAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 32, 310, 310 + 32,
			D3DXVECTOR2(16.f, 16.f)));

		leftAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 76, 76 + 37, 504, 504 + 32,
			D3DXVECTOR2(18.f, 16.f)));
		rightAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 502, 502 + 30,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 500, 500 + 36,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 502, 502 + 34,
			D3DXVECTOR2(15.f, 17.f)));

		leftAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 77, 77 + 37, 387, 387 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		rightAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 387, 387 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 384, 384 + 37,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 387, 387 + 37,
			D3DXVECTOR2(15.f, 18.f)));
	}
	// animation xe tăng của người chơi 3
	else if (ID == 3)
	{
		leftAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 389, 389 + 32, 310, 310 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		rightAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 32, 310, 310 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		upAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 312, 312 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		downAnimationLv01->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 32, 310, 310 + 32,
			D3DXVECTOR2(16.f, 16.f)));

		leftAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 504, 504 + 32,
			D3DXVECTOR2(18.f, 16.f)));
		rightAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 502, 502 + 30,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 500, 500 + 36,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv02->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 502, 502 + 34,
			D3DXVECTOR2(15.f, 17.f)));

		leftAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 387, 387 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		rightAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 387, 387 + 31,
			D3DXVECTOR2(18.f, 15.f)));
		upAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 384, 384 + 37,
			D3DXVECTOR2(16.f, 18.f)));
		downAnimationLv03->AddFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 387, 387 + 37,
			D3DXVECTOR2(15.f, 18.f)));
	}


	arrowAnimation = new Animation();
	arrowAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Arrow, 0, 26, 0, 26,
		D3DXVECTOR2(13.f, 13.f)));
	shieldAnimation = new Animation(0.05f);
	shieldAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 0 + 39, 11, 111 + 42,
		D3DXVECTOR2(20.f, 21.f)));
	shieldAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 39, 39 + 40, 111, 111 + 40,
		D3DXVECTOR2(20.f, 20.f)));
	spawnAnimation = new Animation(0.05f);
	spawnAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 30, 0, 30,
		D3DXVECTOR2(15.f, 15.f)));
	spawnAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 30, 60, 0, 30,
		D3DXVECTOR2(15.f, 15.f)));
	spawnAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 60, 60 + 34, 0, 0 + 31,
		D3DXVECTOR2(17.f, 15.f)));
	spawnAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 93 + 35, 0, 0 + 32,
		D3DXVECTOR2(17.f, 16.f)));
}

void Player::ApplyVelocity()
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

	if (!isMy)
	{
		velocity += (receivedPosition - position) * lerpSmooth;
	}
	else
	{
		D3DXVECTOR2 extraVel = (bestGuessPosition - position);
		float magnitude = sqrt(extraVel.x * extraVel.x + extraVel.y * extraVel.y);
		D3DXVec2Normalize(&extraVel, &extraVel);

		if (magnitude > 30)
			magnitude = 30;
		velocity += (extraVel * magnitude) * 1.2f;
	}
}

D3DXVECTOR2 Player::GetVelocityByDirection(Direction dir)
{
	if (dir == D_Stand)
	{
		return D3DXVECTOR2(0, 0);
	}
	else if (dir == D_Left)
	{
		return D3DXVECTOR2(-speed, 0);
	}
	else if (dir == D_Right)
	{
		return D3DXVECTOR2(speed, 0);
	}
	else if (dir == D_Up)
	{
		return D3DXVECTOR2(0, -speed);
	}
	else if (dir == D_Down)
	{
		return D3DXVECTOR2(0, speed);
	}

	return D3DXVECTOR2(0, 0);
}

void Player::Draw()
{
	if (!IsDelete)
	{
		currentAnimation->Draw(position);
	}
}

void Player::CheckCollision(Entity * e)
{
	if (IsDelete)
		return;


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
