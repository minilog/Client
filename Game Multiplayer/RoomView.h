#pragma once
#include "Animation.h"
#include <string>
#include "Label.h"

class InputMemoryBitStream;

class RoomView
{
	int ID = -1; // -1 = chưa xác định
	bool isPlaying = false; // phòng đã bắt đầu game hay chưa
	const int maxPlayer = 4;
public:
	bool Player0 = false;
	bool Player1 = false;
	bool Player2 = false;
	bool Player3 = false;
	bool Player0_Ready = false;
	bool Player1_Ready = false;
	bool Player2_Ready = false;
	bool Player3_Ready = false;
	int StartingTime = -1;

public:
	RoomView(int _networkID);
	~RoomView();
		
	void Draw();
	void Read(InputMemoryBitStream& _is);

	bool CanJoin() { return (!Player0 || !Player1 || !Player2 || !Player3) && (!isPlaying); }
	bool GetIsPlaying() { return isPlaying; }
	void SetPosition(D3DXVECTOR2 _pos) { position = _pos; }

// các hàm và biến hỗ trợ
private:
	D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f);

	Label label_NPlayer;
	Label label_Name;
	Animation* greenBox;
	Animation* redBox;
	Animation* currentBox;
};

