#pragma once

#include "Scene.h"
#include <vector>
#include "RoomView.h"
#include <map>
#include "Label.h"
#include "Animation.h"

class LobbyScene : public Scene
{
	const float time_SendUpdatePlayer = 0.5f; // khoảng tg yêu cầu cập nhật các người chơi

	// đang ở LOBBY
	std::vector<RoomView*> roomViewList;
	const int NCanSelect = 4; // số lượng phòng có thể chọn

	// đang ở ROOM
	const int NCanSelectInRoom = 2; // chọn thoát phòng hoặc sẵn sàng
	bool isReady = false;
	bool isPlaying = false;
	int serverTimeStarting = -1;
	const int time_StartGame = 3000;

public:
	LobbyScene();
	~LobbyScene();
	void Update(float _dt) override;
	void Draw() override;
	void ReceivePacket(InputMemoryBitStream& _is, int _typePacket) override;


private:

	int selectN = 0; // đang chọn ô thứ mấy
	float count_SendUpdatePlayer = time_SendUpdatePlayer; // đếm
	int lastRoomID = -1;

	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isSpacePressed = false;

	Label label_RoomSelect;
	Label label_Failed;
	Label label_LobbyTitle;
	Animation* selectAnimation;
	Label label_ready;

	// ROOM
	Label label_Player0;
	Label label_Player1;
	Label label_Player2;
	Label label_Player3;
	Label label_Ready;
	Label label_Cancel;
	Label label_ExitRoom;
};
