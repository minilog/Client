#include "LobbyScene.h"

#include "SceneManager.h"
#include "SpriteList.h"
#include "TimeServer.h"
#include "BattleScene.h"
using namespace Define;

LobbyScene::LobbyScene()
{
	// init animations & labels
	{
		selectAnimation = new Animation();
		selectAnimation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Arrow, 0, 0 + 27, 0, 0 + 27,
			D3DXVECTOR2(13.f, 13.f)));

		// LOBBY
		label_RoomSelect = Label("Room select", 30, 20, D3DXVECTOR2(0.f, 0.f));
		label_Failed = Label("Can't join this room ", 30, 20, D3DXVECTOR2(0.f, 0.f));
		label_LobbyTitle = Label("", 50, 25, D3DXVECTOR2(0.f, 0.f));

		// ROOM
		label_Player0 = Label("", 26, 13, D3DXVECTOR2(0.f, 0.f));
		label_Player1 = Label("", 26, 13, D3DXVECTOR2(0.f, 0.f));
		label_Player2 = Label("", 26, 13, D3DXVECTOR2(0.f, 0.f));
		label_Player3 = Label("", 26, 13, D3DXVECTOR2(0.f, 0.f));
		label_ExitRoom = Label("", 26, 13, D3DXVECTOR2(0.f, 0.f));
		label_Ready = Label("", 26, 13, D3DXVECTOR2(0.f, 0.f));
		label_Cancel = Label("", 26, 13, D3DXVECTOR2(0.f, 0.f));
		label_ready = Label("ready", 18, 9, D3DXVECTOR2(0.f, 0.f));
	}
	
	// init 4 room views
	for (int i = 0; i < 4; i++)
	{
		RoomView* room = new RoomView(i);
		roomViewList.push_back(room);
	}
}

LobbyScene::~LobbyScene()
{
	delete selectAnimation;
	for (auto room : roomViewList)
		delete room;
}

void LobbyScene::Update(float _dt)
{
	if (!TimeServer::Instance()->GetIsSync())
		return;

	if (isPlaying && (TimeServer::Instance()->ServerTime() - serverTimeStarting >= time_StartGame))
	{
		// go to battle
		RoomView * room = roomViewList[GameGlobal::Socket->PlayerRoomID];
		SceneManager::Instance()->ReplaceScene(new BattleScene(room->playerInRoomList, (float)room->TimeUp));
		return;
	}


	// controll arrow
	{
		if (keyboard[VK_LEFT] && !isLeftPressed)
		{
			isLeftPressed = true;
			selectN--;
		}
		if (!keyboard[VK_LEFT])
		{
			isLeftPressed = false;
		}

		if (keyboard[VK_RIGHT] && !isRightPressed)
		{
			isRightPressed = true;
			selectN++;
		}
		if (!keyboard[VK_RIGHT])
		{
			isRightPressed = false;
		}

		if (GameGlobal::Socket->PlayerRoomID == -1)
		{
			if (selectN >= NCanSelect)
			{
				selectN = NCanSelect - 1;
			}
			else if (selectN < 0)
			{
				selectN = 0;
			}
		}
		else
		{
			if (selectN >= NCanSelectInRoom)
			{
				selectN = NCanSelectInRoom - 1;
			}
			else if (selectN < 0)
			{
				selectN = 0;
			}
		}
	}

	// you are in looby
	if (GameGlobal::Socket->PlayerRoomID == -1)
	{
		if (keyboard[VK_SPACE] && !isSpacePressed)
		{
			int _roomID = selectN;

			if (roomViewList[_roomID]->CanJoin())
			{
				// send request join room
				{
					OutputMemoryBitStream os;

					os.Write(PT_JoinRoom, NBit_PacketType);
					os.Write(_roomID, NBit_RoomID);

					GameGlobal::Socket->Send(os);
				}
			}

			isSpacePressed = true;
		}
	}
	// you are in room
	else
	{
		if (keyboard[VK_SPACE] && !isSpacePressed)
		{
			if (selectN == 0)
			{
				// send request ready or cancel
				{
					OutputMemoryBitStream os;

					os.Write(PT_ReadyCancel, NBit_PacketType);

					GameGlobal::Socket->Send(os);
				}
			}
			else if (selectN == 1)
			{
				// send request exit room
				{
					OutputMemoryBitStream os;

					os.Write(PT_ExitRoom, NBit_PacketType);

					GameGlobal::Socket->Send(os);
				}
			}

			isSpacePressed = true;
		}
	}

	if (!keyboard[VK_SPACE])
	{
		isSpacePressed = false;
	}

	count_SendUpdatePlayer += _dt;
	if (count_SendUpdatePlayer >= time_SendUpdatePlayer)
	{
		// request update rooms
		{
			OutputMemoryBitStream os;

			os.Write(PT_UpdateRooms, NBit_PacketType);

			GameGlobal::Socket->Send(os);
		}
		count_SendUpdatePlayer -= time_SendUpdatePlayer;
	}
}

void LobbyScene::Draw()
{
	if (!TimeServer::Instance()->GetIsSync())
	{
		label_LobbyTitle.SetPosition(D3DXVECTOR2(400.f, 300.f));
		label_LobbyTitle.Draw("Time synchronizing...");

		return;
	}

	// you are in looby
	if (GameGlobal::Socket->PlayerRoomID == -1)
	{
		label_LobbyTitle.SetPosition(D3DXVECTOR2(550.f, 50.f));
		label_LobbyTitle.Draw("LOBBY");

		// vẽ các room
		{
			D3DXVECTOR2 startPosition = D3DXVECTOR2(275.f, 385.f);
			int hor = 0;
			for (int i = 0; i < (int)roomViewList.size(); ++i)
			{
				hor = i % 4;
				roomViewList[i]->SetPosition(startPosition + D3DXVECTOR2(250.f * hor, 0.f));
				roomViewList[i]->Draw();
			}
		}

		// vẽ dấu Select
		{
			int hor = selectN % 4;
			D3DXVECTOR2 selectPosition = D3DXVECTOR2(275.f + hor * 250.f, 310.f);
			selectAnimation->Draw(selectPosition);
		}
	}
	// you are in room
	else 
	{
		if (isPlaying)
		{
			label_LobbyTitle.SetPosition(D3DXVECTOR2(440.f, 260.f));
			int remainingTime = time_StartGame - (TimeServer::Instance()->ServerTime() - serverTimeStarting);
			label_LobbyTitle.Draw("Game start in " + to_string(remainingTime));
		}
		else
		{
			label_LobbyTitle.SetPosition(D3DXVECTOR2(550.f, 50.f));
			label_LobbyTitle.Draw("ROOM " + to_string(GameGlobal::Socket->PlayerRoomID));
		}

		// draw players
		{
			D3DXVECTOR2 startPosition = D3DXVECTOR2(228.f, 372.f);
			label_Player0.SetPosition(startPosition);
			startPosition += D3DXVECTOR2(250.f, 0.f);
			label_Player1.SetPosition(startPosition);
			startPosition += D3DXVECTOR2(250.f, 0.f);
			label_Player2.SetPosition(startPosition);
			startPosition += D3DXVECTOR2(250.f, 0.f);
			label_Player3.SetPosition(startPosition);

			RoomView* room = roomViewList[GameGlobal::Socket->PlayerRoomID];

			int myID = GameGlobal::Socket->PlayerID;

			if (room->playerInRoomList[0])
			{
				if (myID == 0)
				{
					label_Player0.Draw("Player 0", D3DCOLOR_XRGB(255, 242, 0));
				}
				else
				{
					label_Player0.Draw("Player 0");
				}

				if (room->playerReadyList[0])
				{
					label_ready.SetPosition(D3DXVECTOR2(228.f, 352.f));
					label_ready.Draw();
				}
			}

			if (room->playerInRoomList[1])
			{
				if (myID == 1)
				{
					label_Player1.Draw("Player 1", D3DCOLOR_XRGB(255, 242, 0));
				}
				else
				{
					label_Player1.Draw("Player 1");
				}

				if (room->playerReadyList[1])
				{
					label_ready.SetPosition(D3DXVECTOR2(228.f + 250.f, 352.f));
					label_ready.Draw();
				}
			}

			if (room->playerInRoomList[2])
			{
				if (myID == 2)
				{
					label_Player2.Draw("Player 2", D3DCOLOR_XRGB(255, 242, 0));
				}
				else
				{
					label_Player2.Draw("Player 2");
				}

				if (room->playerReadyList[2])
				{
					label_ready.SetPosition(D3DXVECTOR2(228.f + 250.f * 2, 352.f));
					label_ready.Draw();
				}
			}

			if (room->playerInRoomList[3])
			{
				if (myID == 3)
				{
					label_Player3.Draw("Player 3", D3DCOLOR_XRGB(255, 242, 0));
				}
				else
				{
					label_Player3.Draw("Player 3");
				}

				if (room->playerReadyList[3])
				{
					label_ready.SetPosition(D3DXVECTOR2(228.f + 250.f * 3, 352.f));
					label_ready.Draw();
				}
			}
		}

		label_ExitRoom.SetPosition(D3DXVECTOR2(228.f + 180.f, 372.f + 125.f));
		label_ExitRoom.Draw("Exit Room");

		if (!isReady)
		{
			label_Ready.SetPosition(D3DXVECTOR2(228.f, 372.f + 125.f));
			label_Ready.Draw("Ready");
		}
		else
		{
			label_Cancel.SetPosition(D3DXVECTOR2(228.f, 372.f + 125.f));
			label_Cancel.Draw("Cancel");
		}

		// draw arrow
		{
			int hor = selectN % 4;
			D3DXVECTOR2 selectPosition = D3DXVECTOR2(275.f + hor * 200.f, 475.f);
			selectAnimation->Draw(selectPosition);
		}
	}
}

void LobbyScene::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{   // (9 + 32) * 4 + 5 = 169 bit
	if (_packetType == PT_UpdateRooms)
	{
		// declare temp variables
		bool _inRoom = false;
		int _playerID = 0;
		int _playerRoomID = 0;

		for (auto room : roomViewList)
		{
			room->Read(_is);
		}
		_is.Read(_inRoom);
		_is.Read(_playerID, NBit_PlayerID);
		_is.Read(_playerRoomID, NBit_RoomID);

		// result
		if (_inRoom)
		{
			GameGlobal::Socket->PlayerID = _playerID;
			GameGlobal::Socket->PlayerRoomID = _playerRoomID;

			// set ready & start game
			{
				RoomView* myRView = roomViewList[_playerRoomID];
				if (_playerID == 0)
				{
					isReady = myRView->playerReadyList[0];
				}
				else if (_playerID == 1)
				{
					isReady = myRView->playerReadyList[1];
				}
				else if (_playerID == 2)
				{
					isReady = myRView->playerReadyList[2];
				}
				else if (_playerID == 3)
				{
					isReady = myRView->playerReadyList[3];
				}

				isPlaying = myRView->GetIsPlaying();
				serverTimeStarting = myRView->StartingTime;
			}

			// set variables when first in room
			if (lastRoomID != _playerRoomID)
			{
				isReady = false;
				selectN = 0;
				lastRoomID = _playerRoomID;
			}
		}
		else
		{
			GameGlobal::Socket->PlayerID = -1;
			GameGlobal::Socket->PlayerRoomID = -1;

			// set variables when first in lobby
			if (lastRoomID != -1)
			{
				isReady = false;
				selectN = 0;
				lastRoomID = -1;
			}
		}
	}
	if (_packetType == PT_PlayerInput)
	{
		int a = 3; // BUG GAME
		SceneManager::Instance()->ReplaceScene(new LobbyScene());
	}
}