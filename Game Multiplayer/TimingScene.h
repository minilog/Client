#pragma once

#include "Scene.h"

class TimingScene : public Scene
{
	const int maxPingToSync = 100; // ping < 100 => Sync time
	const float time_SendSync = 2.0f; // khoảng thời gian gửi Sync

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
	Label label_NReceiveExecuted;
	Label label_TimeDistance;

	float count_SendSync = time_SendSync; // đếm

	int NSend = 0; // xác định đúng packet muốn nhận

	int sendTime = -1; // tg gửi packet
	int receiveTime = -1; // tg nhận packet

	int NReceivedExecute = 0; // số packet thực thi, bắt đầu = 0
	double timeDistanceAverage = 0.f; // tg tương ứng với Server, bắt đầu = 0;

	int ping = -1;
	double timeDistance = 0.f;
};