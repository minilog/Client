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

	void Send_SyncTimePacket();
	void Receive_SyncTimePacket(InputMemoryBitStream& _is);

protected:
	Label ping_Label;
	Label myTime_Label; // tick count
	Label sendTime_Label;
	Label receiveTime_Label;
	Label delayTimeAverage_Label;
	Label serverTime_Label; // = host time + timeDistance

	int NSend = 0; // NSend xác định đúng Packet muốn nhận

	float timeBetweenSendSync = 3.0f;
	float sendSyncCount = 0.0f;

	int sendTime = 0;
	int receiveTime = 0;

	float delayTimeAverage = 0.0f; // delayTimeAverage = trung bình cộng của các (receiveTime - sendTime) / 2
	int NSyncReceive = 0; // số lần nhận packet SyncTime

	int timeDistanceWithServer = 0;

	int ping = 0;
};