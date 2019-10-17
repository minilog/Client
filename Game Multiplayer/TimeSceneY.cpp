#include "TimeSceneY.h"

#include "SocketUtil.h"
#include "GameDefine.h"
#include "GameLog.h"

TimeSceneY::TimeSceneY()
{
	// khởi tạo các hình ảnh và dòng chữ Label
	InitSpritesAndLabels();

	// khởi tạo Socket và cố gắng kết nối với Server
	CreateSocketAndTryConnectToServer();
}

void TimeSceneY::Update(float _dt)
{
	// gửi Request Sync Time mỗi timeBetweenSendSync giây
	sendSyncCount -= _dt;
	if (sendSyncCount <= 0.f)
	{
		sendSyncCount += timeBetweenSendSync;
		Send_SyncTimePacket();
	}
}

void TimeSceneY::Draw()
{
	// vẽ các Label
	ping_Label.Draw("Ping: " + to_string(ping) + "ms");
	myTime_Label.Draw("Host tick count: " + to_string((unsigned long)GetTickCount()));
	sendTime_Label.Draw("Send time: " + to_string(sendTime));
	receiveTime_Label.Draw("Receive time: " + to_string(receiveTime));
	delayTimeAverage_Label.Draw("Delay time Average: " + to_string(delayTimeAverage));
	int serverTime = (int)GetTickCount() + timeDistanceWithServer;
	serverTime_Label.Draw("Server time: " + to_string(serverTime));
}

void TimeSceneY::ReceivePacket()
{
	char* buff = static_cast<char*>(std::malloc(1024));
	int receivedByteCount = TCPSocket->Receive(buff, 1024);

	// nếu có Dữ Liệu nhận được
	if (receivedByteCount > 0)
	{
		InputMemoryBitStream is(buff,
			static_cast<uint32_t> (receivedByteCount));

		// đọc packetType, xác định loại dữ liệu sẽ được đọc tiếp theo
		int packetType = 0;
		is.Read(packetType, Define::bitOfTypePacket);

		// nếu là packet SyncTime => đọc time của server
		if (packetType == Define::SyncTime)
		{
			Receive_SyncTimePacket(is);
		}
	}

	// giải phóng Buffer
	free(buff);
}

void TimeSceneY::Send_SyncTimePacket()
{
	NSend++;
	// gửi yêu cầu lấy thời gian của Server
	OutputMemoryBitStream os;
	os.Write(Define::SyncTime, Define::bitOfTypePacket);
	os.Write(NSend, 32);
	TCPSocket->Send(os.GetBufferPtr(), os.GetByteLength());

	sendTime = (int)GetTickCount();
}

void TimeSceneY::Receive_SyncTimePacket(InputMemoryBitStream& _is)
{
	int NReceived = 0;
	_is.Read(NReceived, 32);

	int timeServerReceived = 0;
	_is.Read(timeServerReceived, 32);

	// nếu biến NSend khác NReceive thì return
	if (NReceived != NSend)
		return;

	receiveTime = (int)GetTickCount();

	ping = receiveTime - sendTime;

	// tính thời gian Delay từ Client sang Server
	float delayTimeThisTime = (receiveTime - sendTime) / 2.0f;
	delayTimeAverage = (delayTimeThisTime + delayTimeAverage * NSyncReceive) / (NSyncReceive + 1);
	NSyncReceive++;

	// tính khoảng cách thời gian: timeServer - time This Client
	timeDistanceWithServer = timeServerReceived - (receiveTime - (int)delayTimeAverage);
}

void TimeSceneY::InitSpritesAndLabels()
{
	ping_Label = Label("Ping: ms", 20, 10, D3DXVECTOR2(1170, 15));
	myTime_Label = Label("My time: ", 20, 10, D3DXVECTOR2(15, 15));
	sendTime_Label = Label("Send time: ", 20, 10, D3DXVECTOR2(15, 45));
	receiveTime_Label = Label("Receive time: ", 20, 10, D3DXVECTOR2(15, 75));
	delayTimeAverage_Label = Label("Time distance with Server: ", 20, 10, D3DXVECTOR2(15, 105));
	serverTime_Label = Label("Server time: ", 20, 10, D3DXVECTOR2(15, 135));
}

void TimeSceneY::CreateSocketAndTryConnectToServer()
{
	string ip = "127.0.0.1";
	if (__argv[1] != NULL)
	{
		ip = string(__argv[1]);
	}

	SocketAddress socketAddress(inet_addr(ip.c_str()), 8888);

	TCPSocket = SocketUtil::CreateTCPSocket();
	if (TCPSocket->Connect(socketAddress) == SOCKET_ERROR)
	{
		OutputDebugStringA("Connect to Server failed!");
	}
	else
	{
		OutputDebugStringA("Connect to Server successfull!");
	}

	TCPSocket->ChangetoDontWait(1);

	// GameGlobal save the socket successfully connect to Server
	GameGlobal::socket = TCPSocket;
}


