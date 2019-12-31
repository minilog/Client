#pragma once

#include "GameGlobal.h"
#include <vector>
#include <map>
#include "Label.h"
#include "MemoryBitStream.h"
#include "GameDefine.h"
#include "GameLog.h"
using namespace Define;

class TimeServer
{
	static TimeServer *instance;
	const int maxPingToSync = 100; // ping < 100 => Sync time
	const float time_RequestSync = 2.0f; // time between sync requests
	bool isSync = false;
	float frameTime;

private:
	TimeServer();
public:
	static TimeServer* Instance();
	~TimeServer() {}

	void Update(float _dt);
	void Draw();
	void ReceivePacket(InputMemoryBitStream& _is, int _typePacket);
	int ServerTime();
	int GetIsSync() { return isSync; }

protected:
	Label ping_Label;
	Label myTime_Label;
	Label sendTime_Label;
	Label receiveTime_Label;
	Label label_TimeDistanceAverage;
	Label serverTime_Label;
	Label label_NReceiveExecuted;
	Label label_TimeDistance;

	int sentID = -1; // define exactly packet want to receive

	int ping = -1;
	int sendTime = -1; // last time sent packet
	int receiveTime = -1; // last time receive packet

	double timeDistance = -1.f;
	double timeDistanceMultiplier = 0.f; // the multiplier - calculate timeDistanceAverage
	double timeDistanceAverage = -1.f; // Almost right: server time - your time

	float count_RequestSync = time_RequestSync;
};