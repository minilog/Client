#pragma once
#include "Animation.h"
#include <string>
#include "Label.h"

class InputMemoryBitStream;

class RoomView
{
	int networkID = -1; // -1 = chưa xác định
	int nPlayer = -1; // số người chơi trong phòng
	bool isPlaying = false; // phòng đã bắt đầu game hay chưa
	bool isFull = false; // phòng đã đủ người hay chưa

	const int maxPlayer = 4;
	D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f);

public:
	RoomView();
	~RoomView();
		
	void Draw();
	void Read(InputMemoryBitStream& _is);

	bool CanJoin() { return (nPlayer < 4) && (!isPlaying); }
	void SetPosition(D3DXVECTOR2 _pos);

// các hàm và biến hỗ trợ
private:
	Label label_NPlayer;
	Label label_Name;
	Animation* greenBox;
	Animation* redBox;
	Animation* currentBox;

	void SetData(int _NPlayer, bool _isPlaying);
};

