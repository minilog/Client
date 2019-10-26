#pragma once

#include "Scene.h"

class TimingScene : public Scene
{
public:
	TimingScene();
	~TimingScene() {}

	void Update(float _dt);
	void Draw();
	void ReceivePacket(InputMemoryBitStream& _is, int _typePacket) override;

private:
	void InitAnimationsAndLabels();

	void Send_SyncTimePacket();
	void Receive_SyncTimePacket(InputMemoryBitStream& _is);

protected:
	Label ping_Label;
	Label myTime_Label; // tick count
	Label sendTime_Label;
	Label receiveTime_Label;
	Label label_TimeDistanceAverage;
	Label serverTime_Label; // = host time + timeDistance

	int NSend = 0; // NSend xác định đúng Packet muốn nhận

	float time_SendSync = 2.0f;
	float count_SendSync = 2.0f;

	// thời gian gửi nhận gói tin thời gian
	int sendTime = -1;
	int receiveTime = -1;

	int NReceivedPacket = 0; // bắt đầu = 0, số packet đã nhận
	float timeDistanceAverage = 0.0f; // thời gian trễ trung bình so với Server, bắt đầu = 0;

	int ping = -1;
};