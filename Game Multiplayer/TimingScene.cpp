#include "TimingScene.h"

#include "GameDefine.h"
#include "GameLog.h"

TimingScene::TimingScene()
{
	// khởi tạo các hình ảnh và các dòng chữ Label
	InitAnimationsAndLabels();
}

void TimingScene::Update(float _dt)
{
	// gửi Request Sync Time mỗi time_SendSync giây
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
	label_TimeDistanceAverage.Draw("Time distance with Server: " + to_string(timeDistanceAverage));

	// tính serverTime = time hiện tại + Distance
	int serverTime = (int)GetTickCount() + (int)timeDistanceAverage;
	serverTime_Label.Draw("Server time: " + to_string(serverTime));
}

void TimingScene::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{
	if (_packetType == Define::SyncTime)
	{
		Receive_SyncTimePacket(_is);
	}
}

void TimingScene::Send_SyncTimePacket()
{
	NSend++;
	// gửi yêu cầu lấy thời gian của Server
	OutputMemoryBitStream os;
	os.Write(Define::SyncTime, Define::bitOfTypePacket);
	os.Write(NSend, 32);
	GameGlobal::Socket->Send(os);

	sendTime = (int)GetTickCount();
}

void TimingScene::Receive_SyncTimePacket(InputMemoryBitStream& _is)
{
	int NReceived = 0;
	_is.Read(NReceived, 32);

	int timeServerReceived = 0;
	_is.Read(timeServerReceived, 32);

	// nếu biến NSend khác NReceive => nó thuộc các Packet quá 2 giây mà không nhận được, bỏ qua
	if (NReceived != NSend)
		return;

	receiveTime = (int)GetTickCount();

	ping = receiveTime - sendTime;

	float correspondingTime = (receiveTime + sendTime) / 2.0f;
	float timeDistance = (float)timeServerReceived - correspondingTime;

	timeDistanceAverage = (timeDistanceAverage * NReceivedPacket + timeDistance) / (NReceivedPacket + 1);

	NReceivedPacket += 1;
}

void TimingScene::InitAnimationsAndLabels()
{
	ping_Label = Label("Ping: ms", 20, 10, D3DXVECTOR2(1170, 15));
	myTime_Label = Label("My time: ", 20, 10, D3DXVECTOR2(15, 15));
	sendTime_Label = Label("Send time: ", 20, 10, D3DXVECTOR2(15, 45));
	receiveTime_Label = Label("Receive time: ", 20, 10, D3DXVECTOR2(15, 75));
	label_TimeDistanceAverage = Label("Time distance with Server: ", 20, 10, D3DXVECTOR2(15, 105));
	serverTime_Label = Label("Server time: ", 20, 10, D3DXVECTOR2(15, 135));
}


