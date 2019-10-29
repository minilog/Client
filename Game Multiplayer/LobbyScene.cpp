#include "LobbyScene.h"

#include "SceneManager.h"
#include "RoomScene.h"
#include "SpriteList.h"
using namespace Define;

LobbyScene::LobbyScene()
{
	// khởi tạo các hình ảnh và các dòng chữ
	CreateAnimationAndLabels();

	for (int i = 0; i < 4; i++)
	{
		RoomView* room = new RoomView(i);
		roomViewList.push_back(room);
	}
}

LobbyScene::~LobbyScene()
{
	delete selectAnimation;
	for (auto room : roomViewList)
		delete room;
}

void LobbyScene::Update(float _dt)
{
	// điều khiển mũi tên
	CheckPressArrow();

	if (keyboard[VK_SPACE] && !isSpacePressed)
	{
		if (roomViewList[selectN]->CanJoin())
		{
			// yêu cầu tham gia phòng
			Send_JoinRoom();
		}	

		isSpacePressed = true;
	}
	if (!keyboard[VK_SPACE])
	{
		isSpacePressed = false;
	}

	count_SendUpdatePlayer += _dt;
	if (count_SendUpdatePlayer >= time_SendUpdatePlayer)
	{
		// yêu cầu thông tin phòng
		Send_UpdateCountPlayer();
		count_SendUpdatePlayer -= time_SendUpdatePlayer;
	}
}

//// PHẦN BỔ SUNG
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
		for (int i = 0; i < (int)roomViewList.size(); ++i)
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
	if (_packetType == PT_UpdateRooms)
	{
		// cập nhật các phòng
		Reveive_UpdateRooms(_is);
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
	if (!keyboard[VK_LEFT])
	{
		isLeftPressed = false;
	}

	if (keyboard[VK_RIGHT] && !isRightPressed)
	{
		isRightPressed = true;
		selectN++;
	}
	if (!keyboard[VK_RIGHT])
	{
		isRightPressed = false;
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
	os.Write(PT_JoinRoom, NBit_PacketType);
	os.Write(selectN, NBit_RoomID); // ID phòng muốn tham gia

	GameGlobal::Socket->Send(os);
}

void LobbyScene::Send_UpdateCountPlayer()
{
	OutputMemoryBitStream os;
	os.Write(PT_UpdateRooms, NBit_PacketType);

	GameGlobal::Socket->Send(os);
}

void LobbyScene::Reveive_UpdateRooms(InputMemoryBitStream& _is)
{
	bool _inRoom;
	_is.Read(_inRoom); // networkID âm hay dương => 1 bit

	for (auto room : roomViewList)
	{
		room->Read(_is);
	}

	if (_inRoom)
	{
		int _socketID = 0;
		_is.Read(_socketID, NBit_PlayerID); // ID người chơi

		int _roomID;
		_is.Read(_roomID, NBit_RoomID); // ID phòng

		GameGlobal::Socket->ID = _socketID;

		// chuyển màn
		SceneManager::Instance()->ReplaceScene(new RoomScene());

		return;
	}
}