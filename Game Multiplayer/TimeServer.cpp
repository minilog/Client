#include "TimeServer.h"

TimeServer* TimeServer::instance = nullptr;

TimeServer * TimeServer::Instance()
{
	if (instance == nullptr)
	{
		instance = new TimeServer();
	}
	return instance;
}

TimeServer::TimeServer()
{
	// init animations & labels
	{
		ping_Label = Label("Ping: ms", 20, 10, D3DXVECTOR2(1170, 15));
		myTime_Label = Label("My time: ", 20, 10, D3DXVECTOR2(15, 15));

		sendTime_Label = Label("Send time: ", 20, 10, D3DXVECTOR2(15, 60));
		receiveTime_Label = Label("Receive time: ", 20, 10, D3DXVECTOR2(15, 90));

		label_NReceiveExecuted = Label("", 20, 10, D3DXVECTOR2(15, 135));

		label_TimeDistance = Label("", 20, 10, D3DXVECTOR2(15, 165));
		label_TimeDistanceAverage = Label("Time distance with Server: ", 20, 10, D3DXVECTOR2(15, 195));

		serverTime_Label = Label("Server time: ", 20, 10, D3DXVECTOR2(15, 245));
	}
}

void TimeServer::Update(float _dt)
{
	count_RequestSync += _dt;
	if (count_RequestSync >= time_RequestSync)
	{
		// request sync time
		{
			sendTime = (int)GetTickCount();
			sentID++;

			OutputMemoryBitStream os;

			os.Write(PT_SyncTime, NBit_PacketType);
			os.Write(sentID, NBit_PacketID);

			GameGlobal::Socket->Send(os);
		}
		count_RequestSync -= time_RequestSync;
	}
}

void TimeServer::Draw()
{
	ping_Label.Draw("Ping: " + to_string(ping) + "ms");
	myTime_Label.Draw("Time: " + to_string((unsigned long)GetTickCount()));

	sendTime_Label.Draw("Send time: " + to_string(sendTime));
	receiveTime_Label.Draw("Receive time: " + to_string(receiveTime));

	label_NReceiveExecuted.Draw("Multiplier: " + to_string(timeDistanceMultiplier));
	label_TimeDistance.Draw("Time server - your time: " + to_string(timeDistance + (double)4.f));

	// trong local, số âm nghĩa là servertime tính được chạy chậm hơn thực tế
	label_TimeDistanceAverage.Draw("Time server - your time average: " + to_string(timeDistanceAverage + (double)4.f));
	serverTime_Label.Draw("Server time: " + to_string(GetServerTime()));
}

void TimeServer::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{
	if (_packetType == PT_SyncTime)
	{
		int _receivedID = 0;
		int _timeServerReceived = 0;

		_is.Read(_receivedID, NBit_PacketID);
		_is.Read(_timeServerReceived, NBit_Time);

		// not receive late packet
		if (_receivedID != sentID)
			return;

		receiveTime = (int)GetTickCount();
		ping = receiveTime - sendTime;

		// only receive fast packet
		if (ping <= maxPingToSync && ping > 0)
		{
			double correspondingTime = ((double)receiveTime + (double)sendTime) / 2.f;

			timeDistance = (double)_timeServerReceived - correspondingTime;
			double multiplier = (double)100.f / (double)ping;
			
			timeDistanceAverage = (timeDistanceAverage * timeDistanceMultiplier + timeDistance * multiplier)
				/ (timeDistanceMultiplier + multiplier);

			timeDistanceMultiplier += multiplier;

			isSync = true;
		}
	}
}

int TimeServer::GetServerTime()
{
	if (isSync)
	{
		// time server đo được sẽ chậm hơn 0 - 8ms so với thực tế (vì tính chất frame = 16ms) => + 4ms
		int t = (int)GetTickCount() + (int)timeDistanceAverage + 4;
		return t;
	}
	return -1;
}
