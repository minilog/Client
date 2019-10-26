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

	int selectN;
	int NCanSelect = 0;

	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isUpPressed = false;
	bool isDownPressed = false;
	bool isSpacePressed = false;

	// yêu cầu gửi thông tin các phòng
	float time_SendUpdatePlayer = 0.5f;
	float count_SendUpdatePlayer = 0.f;
public:
	LobbyScene();
	~LobbyScene();

	void Update(float _dt);
	void Draw();
	void ReceivePacket(InputMemoryBitStream& _is, int _typePacket) override;

// các hàm và biến hỗ trợ
private:
	Label label_RoomSelect;
	Label label_Failed;
	Label label_LobbyTitle;
	Animation* selectAnimation;

	void CreateAnimationAndLabels();
	void CheckPressArrow();

	void Send_JoinRoom();
	void Send_UpdateCountPlayer();

	void Reveive_UpdateCountPlayer(InputMemoryBitStream& _is);
};
