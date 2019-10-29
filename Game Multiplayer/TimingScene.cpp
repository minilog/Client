#include "TimingScene.h"

#include "GameDefine.h"
#include "GameLog.h"
using namespace Define;

TimingScene::TimingScene()
{
	// khởi tạo các hình ảnh và các dòng chữ Label
	InitAnimationsAndLabels();
}

void TimingScene::Update(float _dt)
{
	// yêu cầu thời gian
	count_SendSync += _dt;
	if (count_SendSync >= time_SendSync)
	{
		Send_SyncTimePacket();
		count_SendSync -= time_SendSync;
	}
}

void TimingScene::Draw()
{
	// vẽ các Label
	ping_Label.Draw("Ping: " + to_string(ping) + "ms");
	myTime_Label.Draw("Time: " + to_string((unsigned long)GetTickCount()));

	sendTime_Label.Draw("Send time: " + to_string(sendTime));
	receiveTime_Label.Draw("Receive time: " + to_string(receiveTime));

	label_NReceiveExecuted.Draw("NReceive executed: " + to_string(NReceivedExecute));
	label_TimeDistance.Draw("Time server - my time: " + to_string(timeDistance));
	label_TimeDistanceAverage.Draw("Time server - my time average: " + to_string(timeDistanceAverage));

	// tính serverTime = time hiện tại + Distance
	int serverTime = (int)GetTickCount() + (int)timeDistanceAverage;
	serverTime_Label.Draw("Server time: " + to_string(serverTime));
}

void TimingScene::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{
	if (_packetType == PT_SyncTime)
	{
		Receive_SyncTimePacket(_is);
	}
}

void TimingScene::Send_SyncTimePacket()
{
	NSend++;
	sendTime = (int)GetTickCount();

	// yêu cầu thời gian
	OutputMemoryBitStream os;
	os.Write(PT_SyncTime, NBit_PacketType);
	os.Write(NSend, NBit_PacketID);

	GameGlobal::Socket->Send(os);
}

void TimingScene::Receive_SyncTimePacket(InputMemoryBitStream& _is)
{
	int NReceived = 0;
	_is.Read(NReceived, NBit_PacketID);

	int _timeServerReceived = 0;
	_is.Read(_timeServerReceived, NBit_Time);

	// NSend khác NReceive => packet quá 2 giây mà không nhận được
	if (NReceived != NSend)
		return;

	receiveTime = (int)GetTickCount();

	ping = receiveTime - sendTime;

	// nếu ping phải nhỏ thì mới nhận
	if (ping <= maxPingToSync)
	{
		double correspondingTime = ((double)receiveTime + (double)sendTime) / 2.f;

		timeDistance = (double)_timeServerReceived - correspondingTime;

		timeDistanceAverage = (timeDistanceAverage * NReceivedExecute + timeDistance) / (NReceivedExecute + 1);

		NReceivedExecute += 1;
	}
}

void TimingScene::InitAnimationsAndLabels()
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