#include"RoomView.h"

#include "MemoryBitStream.h"
#include "SpriteList.h"

RoomView::RoomView()
{
	// khởi tạo animations và labels
	greenBox = new Animation();
	greenBox->AddFrameInfo(FrameInfo(SpriteList::Instance()->Box, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
	redBox = new Animation();
	redBox->AddFrameInfo(FrameInfo(SpriteList::Instance()->Box, 50, 100, 0, 50, D3DXVECTOR2(25.f, 25.f)));
	label_NPlayer = Label("", 30, 10, D3DXVECTOR2(0.f, 0.f), D3DCOLOR_XRGB(255, 242, 0));
	label_Name = Label("", 30, 10, D3DXVECTOR2(0.f, 0.f), D3DCOLOR_XRGB(255, 242, 0));

	currentBox = greenBox;
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

	label_NPlayer.SetPosition(D3DXVECTOR2(position.x - 20, position.y - 13));
	label_Name.SetPosition(D3DXVECTOR2(position.x - 36, position.y - 55));
	label_Name.Draw("Room " + to_string(networkID));

	// vẽ số người chơi hiện có / số người chơi tối đa
	if (!isPlaying)
	{
		label_NPlayer.Draw(to_string(nPlayer) + "/" + to_string(maxPlayer));
	}
	else
	{
		label_NPlayer.Draw(to_string(nPlayer) + "/" + to_string(maxPlayer) + "\n(Playing)");
	}
}

void RoomView::Read(InputMemoryBitStream & _is)
{
	int _nPlayer = 0;
	_is.Read(_nPlayer, 2); // số người chơi dao động từ 0 - 3 => lấy 2 bit

	SetData(_nPlayer, false);
}

void RoomView::SetPosition(D3DXVECTOR2 _pos)
{
	position = _pos;
}

void RoomView::SetData(int _NPlayer, bool _isPlaying)
{
	// thiết lập số người chơi trong phòng, đổi màu phòng dựa theo số người chơi
	nPlayer = _NPlayer;
	if (nPlayer == maxPlayer)
	{
		isFull = true;
		currentBox = redBox;
	}
	else
	{
		isFull = false;
		currentBox = greenBox;
	}

	// thiết lập đang chơi hay không, đổi màu phòng
	isPlaying = _isPlaying;
	if (isPlaying)
	{
		currentBox = redBox;
	}
}