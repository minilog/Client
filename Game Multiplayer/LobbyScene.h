#pragma once

#include "SceneY.h"
#include <vector>
#include "Room.h"
#include <map>
#include "Label.h"

class LobbyScene :
	public SceneY
{
public:
	LobbyScene();
	~LobbyScene();

	void Update(float _dt);
	void Draw();
	void ReceivePacket() override;

private:
	void CreateSpritesAndLabels();
	void CheckPressArrow();
	void DrawCreateRoom(D3DXVECTOR2 _pos);

	void Send_CreateRoom();
	void Send_JoinRoom();
	void Send_UpdateCountPlayer();

	void Reveive_UpdateCountPlayer(InputMemoryBitStream& _is);

	TCPSocketPtr socket;

	Label label_RoomSelect;
	Label label_Failed;
	Label label_CreateRoom;
	Sprite* sprite_Select;
	Sprite* sprite_CreateRoom;

	std::vector<Room *> roomList;

	int selectN;
	int NCanSelect = 1;
	
	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isUpPressed = false;
	bool isDownPressed = false;
};

