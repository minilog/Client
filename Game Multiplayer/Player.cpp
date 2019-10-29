#include "Player.h"

#include "GameLog.h"
#include "SpriteList.h"

Player::Player(int _networkID)
{
	// gán ID network cho Player
	EntityID = _networkID;

	// gán loại đối tượng
	Type = ET_Player;

	// khởi tạo các animation dựa theo NetworkID
	InitAnimation();

	// animation ban đầu
	currentAnimation = upAnimationLv01;

	// kích cỡ va chạm, 28, 28 cho dễ di chuyển
	width = 28;
	height = 28;
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

void Player::Write(OutputMemoryBitStream& _os)
{
	_os.Write(direction, 3); // dao đọng từ 0 - 5 => 3 bit
}

void Player::Read(InputMemoryBitStream& _is)
{
	// đọc networkd ID và loại đối tượng
	Entity::Read(_is);

	// đọc hướng di chuyển
	Direction _dir;
	_is.Read(_dir, 3); // dao động từ 0 - 5 => 3 bit

	// đọc cấp độ
	int _level;
	_is.Read(_level, 3); // dao động từ 1 - 3 => 2 bit 

	// đọc được bảo vệ hay không
	_is.Read(IsProtect); // => 1 bit

	LastHealth = Health; // lưu lại HP trước khi đọc

	// đọc HP
	_is.Read(Health, 2); // HP dao động từ 0 - 2 => 2 bit

	// đọc time di chuyển lần cuối
	_is.Read(LastMoveTime);

	// đọc điểm
	Score_Send = 0;
	_is.Read(Score_Send, 10); // dao động từ 0 - 1023 => 10 bit


	// đọc vị trí Score
	int x, y;
	_is.Read(x, 12); // => 12 bit
	_is.Read(y, 12); // => 12 bit

	// xử lý level đọc được
	if (Level != _level)
	{
		Level = _level;
		//OnSetLevel()
	}

	// xử lý điểm đọc được
	Score += Score_Send;

	//  xử lý vị trí score đọc được
	positionScore.x = (float)x;
	positionScore.y = (float)y;
}

void Player::Update(float _dt)
{
	// khi HP hiện tại lớn hớn HP lần gần nhất => Hồi sinh
	if (Health != 0 && LastHealth == 0)
	{
		isSpawn = true;
		// thời gian bắt đầu hồi sinh
		lastTimeSpawn = GetTickCount();
	}

	// khi HP hiện tại = 0 và chưa bị delete
	if (Health == 0 && !IsDelete)
	{
		IsDelete = true;
		// thời gian đã chết
		LastTimeDie = GetTickCount();
	}

	// nếu đang bị delete, tính toán sự hồi sinh dựa vào thời gian đã chết gần nhất
	if (IsDelete) 
	{
		// hồi sinh trong 4 giây
		if (GetTickCount() - LastTimeDie > 4000)
		{
			IsDelete = false;
		}
		return;
	}

	// nếu đang hồi sinh
	if (isSpawn)
	{
		spawnAnimation->Update(_dt);
		// nếu lần chết gần nhất đã quá 2 giây...
		if (GetTickCount() - LastTimeDie > 2000)
		{
			isSpawn = false;
		}
		return;
	}

	if (IsProtect)
		shieldAnimation->Update(_dt);

	position += velocity * _dt;

	// thay đổi animation đựa vào hướng đi
	SetAnimationByDirection(direction);
	switch (direction)
	{
	case D_Stand:
		SetAnimationByDirection(lastDirection);
		velocity = D3DXVECTOR2(0.f, 0.f);
		break;
	case D_Left:
		if (Level == 1)
		{
			velocity = D3DXVECTOR2(-speed1, 0.f);
		}
		else
		{
			velocity = D3DXVECTOR2(-speed2, 0.f);
		}
		break;
	case D_Right:
		if (Level == 1)
		{
			velocity = D3DXVECTOR2(speed1, 0.f);
		}
		else
		{
			velocity = D3DXVECTOR2(speed2, 0.f);
		}
		break;
	case D_Up:
		if (Level == 1)
		{
			velocity = D3DXVECTOR2(0.f, -speed1);
		}
		else
		{
			velocity = D3DXVECTOR2(0.f, -speed2);
		}
		break;
	case D_Down:
		if (Level == 1)
		{
			velocity = D3DXVECTOR2(0.f, speed1);
		}
		else
		{
			velocity = D3DXVECTOR2(0.f, speed1);
		}
		break;
	}
}

void Player::HandleKeyboard(std::map<int, bool> keys)
{
	// nếu đã thua, đã bị delete hoặc đang hồi sinh => không nhận phím
	if (isLose ||
		IsDelete ||
		isSpawn)
	{
		return;
	}

	// thay đổi hướng đi dựa vào bàn phím
	if (keys[VK_LEFT])
	{
		direction = D_Left;
		lastDirection = D_Left;
	}
	else if (keys[VK_RIGHT])
	{
		direction = D_Right;
		lastDirection = D_Right;
	}
	else if (keys[VK_UP])
	{
		direction = D_Up;
		lastDirection = D_Up;
	}
	else if (keys[VK_DOWN])
	{
		direction = D_Down;
		lastDirection = D_Down;
	}
	else
	{
		direction = D_Stand;
	}

	// tính toán bắn đạn
	int time_to_fight = 1000;
	if (keys[VK_SPACE])
	{
		if (Level > 2)
		{
			time_to_fight = 700;
		}

		if ((int)GetTickCount() - LastFire > time_to_fight)
		{
			LastFire = (int)GetTickCount();
			// bắn đạn ở đây
		}
	}

	// gửi dữ liệu

	//if (mLastAction != mAction)
	//{

	//	OutputMemoryBitStream os;
	//	os.Write(Define::InputPacket, Define::bitOfTypePacket);
	//	os.Write(ID, Define::bitofID);
	//	os.Write((int)mAction, Define::bitofID);
	//	os.Write(last_id_packet++, Define::bitofID);
	//	if (last_id_packet == 1000) last_id_packet = 0;
	//	GameGlobal::socket->Send(os.GetBufferPtr(), os.GetByteLength());
	//}

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
	if (EntityID == 0)
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
	else if (EntityID == 1)
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
	else if (EntityID == 2)
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
	else if (EntityID == 3)
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

void Player::SetAnimationByDirection(Direction _dir)
{
	if (Level == 1)
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
	else if (Level == 2)
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
	else if (Level == 3)
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

void Player::Draw()
{
	//if (isLose) return;
	//if (isDelete) return;
	//if (is_respaw)
	//{
	//	spawn->SetPosition(GetPosition());
	//	spawn->Draw();
	//	return;
	//}

	//mCurrentSprite->SetPosition(this->GetPosition());

	//mCurrentSprite->Draw(D3DXVECTOR3(x, y, 0));
	//if (isMe)
	//{
	//	m_top_sprite->SetPosition(D3DXVECTOR3(x, y + 35, 0));
	//	m_top_sprite->Draw();
	//}

	//if (is_protect)
	//{
	//	shield->SetPosition(GetPosition());
	//	shield->Draw();
	//}
}