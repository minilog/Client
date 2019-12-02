#include"RoomView.h"

#include "MemoryBitStream.h"
#include "SpriteList.h"
using namespace Define;

RoomView::RoomView(int _networkID)
{
	ID = _networkID;

	// khởi tạo animations và labels
	greenBox = new Animation();
	greenBox->AddFrameInfo(FrameInfo(SpriteList::Instance()->Box, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
	redBox = new Animation();
	redBox->AddFrameInfo(FrameInfo(SpriteList::Instance()->Box, 50, 100, 0, 50, D3DXVECTOR2(25.f, 25.f)));
	label_NPlayer = Label("", 30, 10, D3DXVECTOR2(0.f, 0.f));
	label_Name = Label("", 30, 10, D3DXVECTOR2(0.f, 0.f));

	// trạng thái phòng màu xanh
	currentBox = greenBox;

	// init 4 slots
	for (int i = 0; i < 4; i++)
	{
		playerInRoomList.push_back(false);
		playerReadyList.push_back(false);
	}
}

RoomView::~RoomView()
{
	delete greenBox;
	delete redBox;
}

void RoomView::Draw() {
	// vẽ hộp tình trạng của phòng: đỏ hoặc xanh
	currentBox->Draw(position);

	// vẽ tên phòng = ID network	
	label_Name.SetPosition(D3DXVECTOR2(position.x - 36, position.y - 55));
	label_Name.Draw("Room " + to_string(ID), D3DCOLOR_XRGB(255, 242, 0));

	label_NPlayer.SetPosition(D3DXVECTOR2(position.x - 16, position.y - 13));
	// vẽ số người chơi hiện có / số người chơi tối đa
	if (!isPlaying)
	{
		label_NPlayer.Draw(to_string(playerInRoomList[0] + playerInRoomList[1] + playerInRoomList[2] + playerInRoomList[3])
			+ "/" + to_string(maxPlayer), D3DCOLOR_XRGB(255, 242, 0));
	}
	else
	{
		label_NPlayer.Draw(to_string(playerInRoomList[0] + playerInRoomList[1] + playerInRoomList[2] + playerInRoomList[3])
			+ "/" + to_string(maxPlayer) + "\n(Playing)", D3DCOLOR_XRGB(255, 242, 0));
	}
}

void RoomView::Read(InputMemoryBitStream & _is)
{
	// declare temp variables
	bool _player0 = false;
	bool _player0_Ready = false;
	bool _player1 = false;
	bool _player1_Ready = false;
	bool _player2 = false;
	bool _player2_Ready = false;
	bool _player3 = false;
	bool _player3_Ready = false;
	bool _isPlaying = false;
	int _startingTime = 0;
	int timeUp = 0;

	// read stream
	_is.Read(_player0);
	_is.Read(_player0_Ready);
	_is.Read(_player1);
	_is.Read(_player1_Ready);
	_is.Read(_player2);
	_is.Read(_player2_Ready);
	_is.Read(_player3);
	_is.Read(_player3_Ready);
	_is.Read(_isPlaying);
	if (_isPlaying)
	{
		_is.Read(_startingTime, NBit_Time);
		_is.Read(timeUp, NBit_Time);
	}

	// result
	playerInRoomList[0] = _player0;
	playerReadyList[0] = _player0_Ready;
	playerInRoomList[1] = _player1;
	playerReadyList[1] = _player1_Ready;
	playerInRoomList[2] = _player2;
	playerReadyList[2] = _player2_Ready;
	playerInRoomList[3] = _player3;
	playerReadyList[3] = _player3_Ready;

	isPlaying = _isPlaying;
	if (isPlaying)
	{
		StartingTime = _startingTime;
		TimeUp = timeUp;
	}

	// đổi màu phòng
	if ((playerInRoomList[0] && playerInRoomList[1] && playerInRoomList[2] && playerInRoomList[3])
		|| isPlaying)
	{
		currentBox = redBox;
	}
	else
	{
		currentBox = greenBox;
	}
}

bool RoomView::CanJoin()
{
	if (currentBox == redBox)
	{
		return false;
	}

	return true;
}
