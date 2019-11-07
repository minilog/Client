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
	vector<bool> playerInRoomList;
	vector<bool> playerReadyList;

	int StartingTime = -1;

public:
	RoomView(int _networkID);
	~RoomView();
		
	void Draw();
	void Read(InputMemoryBitStream& _is);

	bool CanJoin();
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

