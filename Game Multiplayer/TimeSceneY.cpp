#include "TimeSceneY.h"

#include "SocketUtil.h"
#include "MemoryBitStream.h"
#include "GameDefine.h"

TimeSceneY::TimeSceneY()
{
	InitSpritesAndLabels();

	CreateSocketAndTryConnectToServer();
}

void TimeSceneY::Update(float _dt)
{
	sendSyncCount -= _dt;
	if (sendSyncCount <= 0.f)
	{
		sendSyncCount = timeBetweenSendSync;
		SendSyncTimePacket();
	}
}

void TimeSceneY::Draw()
{
	ping_Label.Draw();
	myTime_Label.Draw("Host tick count: " + to_string((unsigned long)GetTickCount()));
	sendTime_Label.Draw();
	receiveTime_Label.Draw();
	timeDistanceWithServer_Label.Draw();
	serverTime_Label.Draw("Server time: " + to_string(serverTime));
}

void TimeSceneY::SendSyncTimePacket()
{
	OutputMemoryBitStream os;
	os.Write(Define::SyncTime, Define::bitOfTypePacket); // gửi PacketType với 2 bit
	os.Write((int)GetTickCount(), 32);
	TCPSocket->Send(os.GetBufferPtr(), os.GetByteLength());
}

void TimeSceneY::ReceivePacket()
{
	char* buff = static_cast<char*>(std::malloc(1024));
	size_t receivedByteCount = TCPSocket->Receive(buff, 1024);

	if (receivedByteCount > 0)
	{
		InputMemoryBitStream is(buff,
			static_cast<uint32_t> (receivedByteCount));

		// nếu là loại packet SyncTime thì đọc time của server
		{
			int typeOfPacket = 0;
			is.Read(typeOfPacket, Define::bitOfTypePacket);
			if (typeOfPacket == Define::SyncTime)
			{
				is.Read(serverTime, 32);
			}
		}
	}
	free(buff);
}

void TimeSceneY::InitSpritesAndLabels()
{
	ping_Label = Label("Ping: ms", 20, 10, D3DXVECTOR2(1170, 15));
	myTime_Label = Label("My time: ", 20, 10, D3DXVECTOR2(15, 15));
	sendTime_Label = Label("Send time: ", 20, 10, D3DXVECTOR2(15, 45));
	receiveTime_Label = Label("Receive time: ", 20, 10, D3DXVECTOR2(15, 75));
	timeDistanceWithServer_Label = Label("Time distance with Server: ", 20, 10, D3DXVECTOR2(15, 105));
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


