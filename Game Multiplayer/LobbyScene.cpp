#include "LobbyScene.h"

#include "SceneManagerY.h"
#include "RoomSceneY.h"

LobbyScene::LobbyScene()
{
	// khởi tạo các hình ảnh và các dòng chữ
	CreateSpritesAndLabels();

	selectN = 0;

	socket = GameGlobal::socket; // socket đã được khởi tạo ở TimeScene
}


void LobbyScene::Update(float _dt)
{
	// điều khiển Con trỏ chọn
	CheckPressArrow();

	if (keyboard[VK_SPACE])
	{
		// Người Chơi chọn nút Create Room...
		if (selectN == NCanSelect - 1)
		{
			// gửi Packet CreateRoom cho Server, chuyển Màn sang Phòng Chờ
			Send_CreateRoom();
			SceneManagerY::GetInstance()->ReplaceScene(new RoomSceneY());
			return;
		}
		// Người Chơi chọn 1 Phòng để tham gia Chơi 
		else
		{
			// khi phòng đã vào Trận hoặc đã Đủ người...
			if (roomList[selectN]->isPlaying || roomList[selectN]->isFull)
			{
				// thông báo tham gia phòng thất bại
			}
			// khi Người Chơi có thể vào phòng...
			else
			{
				// gửi packet JoinRoom lên Server, chuyển Màn sang Phòng Chờ
				Send_JoinRoom();
				SceneManagerY::GetInstance()->ReplaceScene(new RoomSceneY());
				return;
			}
		}
	}

	// yêu cầu Server gửi dữ liệu về các người chơi ở các phòng
	Send_UpdateCountPlayer();
}

/////////////////////////// PHẦN BỔ SUNG
///////////////////////////
void LobbyScene::Draw()
{
	// vẽ dấu Select
	{
		int hor = selectN % 4;
		int ver = selectN / 4;
		D3DXVECTOR3 selectPosition = D3DXVECTOR3(250 + hor * 250, 570 - ver * 250, 0);
		sprite_Select->Draw(selectPosition);
	}
	// vẽ các Room
	{
		D3DXVECTOR3 startPosition = D3DXVECTOR3(275, 510, 0);
		int hor = 0;
		int ver = 0;
		for (int i = 0; i < roomList.size(); ++i)
		{
			hor = i % 4;
			ver = i / 4;
			roomList[i]->setPosition(startPosition + D3DXVECTOR3(250 * hor, -250 * ver, 0));
			roomList[i]->Draw();
		}
	}
	// vẽ Create Room Button
	{
		int hor = (NCanSelect - 1) % 4;
		int ver = (NCanSelect - 1) / 4;
		DrawCreateRoom(D3DXVECTOR2(251, 515) + D3DXVECTOR2(250 * hor, -250 * ver));
	}
}

void LobbyScene::ReceivePacket()
{
	char* buff = static_cast<char*>(std::malloc(1024));
	int receivedByteCount = socket->Receive(buff, 1024);

	// nếu có Dữ Liệu cần được đọc
	if (receivedByteCount > 0)
	{
		// đưa Dữ Liệu vào Input Stream
		InputMemoryBitStream is(buff,
			static_cast<uint32_t> (receivedByteCount));

		// đọc loại Dữ Liệu => xác định Dữ Liệu sẽ được đọc tiếp theo
		int typeofPacket = 0;
		is.Read(typeofPacket, Define::bitOfTypePacket);

		// packetType là Cập Nhật Người Chơi...
		if (typeofPacket == Define::UpdateCountPlayer)
		{
			Reveive_UpdateCountPlayer(is);
		}
	}
	// giải phóng Buffer
	free(buff);
}

void LobbyScene::CreateSpritesAndLabels()
{
	sprite_Select = new Sprite("Resource files/topOfplayer.png", RECT(), 0, 0, D3DCOLOR_XRGB(255, 0, 255));
	sprite_CreateRoom = new Sprite("Resource files/plusbox.jpg");
	sprite_CreateRoom->SetPosition(GameGlobal::GetWidth() / 2 - 50, 200);
	label_CreateRoom = Label("Create Room", 20, 10, D3DXVECTOR2(GameGlobal::GetWidth() / 2 - 120, 600));
	label_RoomSelect = Label("Room select", 30, 20, D3DXVECTOR2(500, 200));
	label_Failed = Label("Can't join this room ", 30, 20, D3DXVECTOR2(GameGlobal::GetWidth() / 2 - 100, GameGlobal::GetHeight() / 2 - 50));;
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

void LobbyScene::DrawCreateRoom(D3DXVECTOR2 _pos)
{
	sprite_CreateRoom->SetPosition(_pos);
	D3DXVECTOR2 pos2 = D3DXVECTOR2(_pos.x - 63, GameGlobal::GetHeight() - _pos.y + 35);
	label_CreateRoom.SetPosition(pos2);

	sprite_CreateRoom->Draw();
	label_CreateRoom.Draw();
}

void LobbyScene::Send_CreateRoom()
{
	OutputMemoryBitStream os;
	os.Write(Define::CreateRoom, Define::bitOfTypePacket);
	socket->Send(os);
}

void LobbyScene::Send_JoinRoom()
{
	OutputMemoryBitStream os;
	os.Write(Define::ChooseRoom, Define::bitOfTypePacket);
	os.Write(selectN, 4); // gửi số Phòng muốn tham gia
	socket->Send(os);
}

void LobbyScene::Send_UpdateCountPlayer()
{
	OutputMemoryBitStream os;
	os.Write(Define::UpdateCountPlayer, Define::bitOfTypePacket);
	socket->Send(os);
}

void LobbyScene::Reveive_UpdateCountPlayer(InputMemoryBitStream& _is)
{
	// đọc số lượng Phòng Chơi hiện có trên Server
	int size = 0;
	_is.Read(size, Define::bitOfTypePacket);

	// nếu số Phòng Chơi ít hơn Server => thêm Phòng Chơi vào danh sách
	while (roomList.size() < size)
	{
		Room* room = new Room();
		roomList.push_back(room);
		
		// tăng số lượng Con trỏ có thể lựa chọn lên 1
		NCanSelect++;
	}
	// nếu số Phòng Chơi nhiều hơn Server => xóa Phòng Chơi trong danh sách
	while (roomList.size() > size)
	{
		roomList.pop_back();

		// giảm số lượng Con trỏ có thể lựa chọn xuống 1
		NCanSelect--;
	}
	// đọc thông tin danh sách Phòng Chơi từ Server
	for (auto room : roomList)
	{
		room->Read(_is);
	}
}

LobbyScene::~LobbyScene()
{
	delete sprite_Select;
	for (auto room : roomList)
		delete room;
}