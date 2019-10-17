#pragma once

#include "SceneY.h"

class TimeSceneY : public SceneY
{
public:
	TimeSceneY();
	~TimeSceneY() {}

	TCPSocketPtr TCPSocket;	// socket dùng để gửi, nhận dữ liệu từ server 

	void Update(float _dt);
	void Draw();

	void ReceivePacket() override;

private:
	void InitSpritesAndLabels();
	void CreateSocketAndTryConnectToServer();

	void SendSyncTimePacket();

protected:
	Label ping_Label;
	Label myTime_Label; // tick count
	Label sendTime_Label;
	Label receiveTime_Label;
	Label timeDistanceWithServer_Label;
	Label serverTime_Label; // = host time + timeDistance


	float timeBetweenSendSync = 3.0f;
	float sendSyncCount = 3.0f;

	int serverTime = 0;

};