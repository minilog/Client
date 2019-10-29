#pragma once

#include "Scene.h"
#include <vector>
#include "RoomView.h"
#include <map>
#include "Label.h"
#include "Animation.h"

class LobbyScene : public Scene
{
	std::vector<RoomView*> roomViewList;

	const int NCanSelect = 4; // số lượng phòng có thể chọn
	const float time_SendUpdatePlayer = 0.5f; // khoảng tg yêu cầu cập nhật các người chơi

public:
	LobbyScene();
	~LobbyScene();

	void Update(float _dt);
	void Draw();
	void ReceivePacket(InputMemoryBitStream& _is, int _typePacket) override;

// các hàm và biến hỗ trợ
private:
	int selectN = 0; // đang chọn phòng nào
	float count_SendUpdatePlayer = time_SendUpdatePlayer; // đếm

	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isSpacePressed = false;

	Label label_RoomSelect;
	Label label_Failed;
	Label label_LobbyTitle;
	Animation* selectAnimation;

	void CreateAnimationAndLabels();
	void CheckPressArrow();
	void Send_JoinRoom();
	void Send_UpdateCountPlayer();
	void Reveive_UpdateRooms(InputMemoryBitStream& _is);
};
