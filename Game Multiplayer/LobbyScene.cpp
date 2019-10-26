#include "LobbyScene.h"

#include "SceneManager.h"
#include "RoomScene.h"
#include "SpriteList.h"

LobbyScene::LobbyScene()
{
	// khởi tạo các hình ảnh và các dòng chữ
	CreateAnimationAndLabels();

	for (int i = 0; i < 4; i++)
	{
		RoomView* room = new RoomView();
		roomViewList.push_back(room);
	}

	// thông số mũi tên lựa chọn
	NCanSelect = 4;
	selectN = 0;
}


void LobbyScene::Update(float _dt)
{
	// điều khiển mũi tên
	CheckPressArrow();

	// tham gia phòng
	if (keyboard[VK_SPACE] && !isSpacePressed)
	{
		if (roomViewList[selectN]->CanJoin())
		{
			// gửi packet JoinRoom lên Server, chuyển Màn sang Phòng Chờ
			Send_JoinRoom();
			SceneManager::Instance()->ReplaceScene(new RoomScene());
			return;
		}	
		// khi phòng đã vào Trận hoặc đã Đủ người...
		else
		{
			// thông báo tham gia phòng thất bại
		}

		isSpacePressed = true;
	}
	if (!keyboard[VK_SPACE])
	{
		isSpacePressed = false;
	}

	// yêu cầu thông tin các người chơi
	count_SendUpdatePlayer += _dt;
	if (count_SendUpdatePlayer >= time_SendUpdatePlayer)
	{
		count_SendUpdatePlayer -= time_SendUpdatePlayer;
		Send_UpdateCountPlayer();
	}
}

/////////////////////////// PHẦN BỔ SUNG
///////////////////////////
void LobbyScene::Draw()
{
	// vẽ dấu Select
	{
		int hor = selectN % 4;
		int ver = selectN / 4;
		D3DXVECTOR2 selectPosition = D3DXVECTOR2(275.f + hor * 250.f, 310.f + ver * 250.f);
		selectAnimation->Draw(selectPosition);
	}
	// vẽ các Room
	{
		D3DXVECTOR2 startPosition = D3DXVECTOR2(275.f, 385.f);
		int hor = 0;
		int ver = 0;
		for (int i = 0; i < roomViewList.size(); ++i)
		{
			hor = i % 4;
			ver = i / 4;
			roomViewList[i]->SetPosition(startPosition + D3DXVECTOR2(250.f * hor, 250.f * ver));
			roomViewList[i]->Draw();
		}
	}

	label_LobbyTitle.SetPosition(D3DXVECTOR2(550.f, 50.f));
	label_LobbyTitle.Draw();
}

void LobbyScene::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{
	// packetType là Cập Nhật Người Chơi...
	if (_packetType == Define::UpdateCountPlayer)
	{
		Reveive_UpdateCountPlayer(_is);
	}
}

void LobbyScene::CreateAnimationAndLabels()
{
	selectAnimation = new Animation();
	selectAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Arrow, 0, 0 + 27, 0, 0 + 27,
		D3DXVECTOR2(13.f, 13.f)));
	label_RoomSelect = Label("Room select", 30, 20, D3DXVECTOR2(500, 200));
	label_Failed = Label("Can't join this room ", 30, 20, D3DXVECTOR2( 100, 50));
	label_LobbyTitle = Label("LOBBY", 39, 25, D3DXVECTOR2(0.f, 0.f));
}

void LobbyScene::CheckPressArrow()
{
	if (keyboard[VK_LEFT] && !isLeftPressed)
	{
		isLeftPressed = true;
		selectN--;
	}
	else if (!keyboard[VK_LEFT])
	{
		isLeftPressed = false;
	}

	if (keyboard[VK_RIGHT] && !isRightPressed)
	{
		isRightPressed = true;
		selectN++;
	}
	else if (!keyboard[VK_RIGHT])
	{
		isRightPressed = false;
	}

	if (keyboard[VK_UP] && !isUpPressed)
	{
		isUpPressed = true;
		if (selectN >= 4)
			selectN -= 4;
	}
	else if (!keyboard[VK_UP])
	{
		isUpPressed = false;
	}

	if (keyboard[VK_DOWN] && !isDownPressed)
	{
		isDownPressed = true;
		if (selectN < 4)
			selectN += 4;
	}
	else if (!keyboard[VK_DOWN])
	{
		isDownPressed = false;
	}

	if (selectN >= NCanSelect)
	{
		selectN = NCanSelect - 1;
	}
	else if (selectN < 0)
	{
		selectN = 0;
	}
}

void LobbyScene::Send_JoinRoom()
{
	OutputMemoryBitStream os;
	os.Write(Define::Join, Define::bitOfTypePacket);
	os.Write(selectN, 4); // gửi số Phòng muốn tham gia
	GameGlobal::Socket->Send(os);
}

void LobbyScene::Send_UpdateCountPlayer()
{
	OutputMemoryBitStream os;
	os.Write(Define::UpdateCountPlayer, Define::bitOfTypePacket);
	GameGlobal::Socket->Send(os);
}

void LobbyScene::Reveive_UpdateCountPlayer(InputMemoryBitStream& _is)
{
	// networkID
	bool _inRoom;
	_is.Read(_inRoom); // nhận 1 bit

	if (_inRoom)
	{
		_is.Read(GameGlobal::Socket->ID, 2); // ID: 0 - 3 => nhận 2 bit
		int _roomID;
		_is.Read(_roomID, 2); // ID phòng: 0 - 3 => nhận 2 bit

		// cần chuyển màn
		int remainingBits;
		_is.Read(remainingBits, 8); // nhận 8 bit thừa của 4 phòng
	}

	// đọc thông tin danh sách Phòng Chơi từ Server
	for (auto room : roomViewList)
	{	
		room->Read(_is);
	}
}

LobbyScene::~LobbyScene()
{
	delete selectAnimation;
	for (auto room : roomViewList)
		delete room;
}