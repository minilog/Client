#include "BattleScene.h"
#include "GameLog.h"
#include "TimeServer.h"

BattleScene::BattleScene(vector<bool> _playerInRoomList, float time)
{
	count_TimeUp = time;
	map = new GameMap("Resource files/map.tmx");

	// tạo plays
	for (int i = 0; i < 4; i++)
	{
		if (_playerInRoomList[i] == true)
		{
			Player* player = new Player(i);
			playerList.push_back(player);

			// tạo 4 bullet cho mỗi player
			for (int j = 0; j < 4; j++)
			{
				Bullet* bullet = new Bullet(j, i);
				bulletList.push_back(bullet);
			}
		}
	}

	// tạo 3 npcs
	for (int i = 0; i < 3; i++)
	{
		NPC* npc = new NPC(i);
		npcList.push_back(npc);
	}

	// tạo 10 small explosion
	for (int i = 0; i < 10; i++)
	{
		Explosion* e = new Explosion(false);
		smallExList.push_back(e);

		for (auto bullet : bulletList)
		{
			bullet->AddExpolostion(e);
		}
	}

	// tạo 5 big explosion
	for (int i = 0; i < 5; i++)
	{
		Explosion* e = new Explosion(true);
		bigExList.push_back(e);

		for (auto npc : npcList)
		{
			npc->AddExplosion(e);
		}
		for (auto player : playerList)
		{
			player->AddExplosion(e);
		}
	}

	// tạo 2 loại Protected Item và Upgrade Item
	protectItem = new ProtectItem(D3DXVECTOR2(250.0f, 250.0f));
	protectItem->IsDelete = true;
	upgradeItem = new UpgradeItem(D3DXVECTOR2(350.0f, 350.0f));
	upgradeItem->IsDelete = true;
	pointed = new Pointed();

	label_Player0 = Label("", 26, 13, D3DXVECTOR2(0, 0));
	label_Player1 = Label("", 26, 13, D3DXVECTOR2(0, 0));
	label_Player2 = Label("", 26, 13, D3DXVECTOR2(0, 0));
	label_Player3 = Label("", 26, 13, D3DXVECTOR2(0, 0));
	label_CountTime = Label("", 26, 13, D3DXVECTOR2(0, 0));
	label_TimeUp = Label("TIME UP!", 36, 18, D3DXVECTOR2(0, 0));

}

BattleScene::~BattleScene()
{
	delete map;
}

void BattleScene::Update(float dt)
{
	// đã nhận packet trước hàm Update này

	if (count_TimeUp < 0)
		return;

	count_TimeUp -= dt;

	// nhận keyboard
	for (auto player : playerList)
	{
		player->HandleKeyboard(keyboard, dt);
	}

	// áp dụng vận tốc
	for (auto player : playerList)
	{
		player->ApplyVelocity();
	}
	for (auto npc : npcList)
	{
		npc->ApplyVelocity();
	}
	for (auto bullet : bulletList)
	{
		bullet->ApplyVelocity();
	}	

	for (auto player : playerList)
	{
		if (!player->IsDelete)
		{	
			// players va chạm npcs
			for (auto npc : npcList)
			{
				if (!npc->IsDelete &&
					GameCollision::IsCollideInNextFrame(player, npc, dt))
				{
					player->ZeroVelocity();
					npc->ZeroVelocity();
				}
			}

			// players va chạm players
			for (auto player2 : playerList)
			{
				if (!player2->IsDelete &&
					player->ID != player2->ID &&
					GameCollision::IsCollideInNextFrame(player, player2, dt))
				{
					player->ZeroVelocity();
					player2->ZeroVelocity();
				}
			}
		}
	}

	for (auto player : playerList)
	{
		player->Update(dt);
	}

	for (auto brick : map->GetBrickList())
	{
		if (!brick->IsDelete)
		{
			// players va chạm bricks
			for (auto player : playerList)
			{
				player->CheckCollision(brick);
			}
		}
	}

	// diễn cái nhẹ XD
	{
		for (auto player : playerList)
		{
			player->ApplyVelocity_Compensation();
		}
		for (auto player : playerList)
		{
			if (!player->IsDelete)
			{
				// players va chạm npcs
				for (auto npc : npcList)
				{
					if (!npc->IsDelete &&
						GameCollision::IsCollideInNextFrame(player, npc, dt))
					{
						player->ZeroVelocity();
						npc->ZeroVelocity();
					}
				}

				// players va chạm players
				for (auto player2 : playerList)
				{
					if (!player2->IsDelete &&
						player->ID != player2->ID &&
						GameCollision::IsCollideInNextFrame(player, player2, dt))
					{
						player->ZeroVelocity();
						player2->ZeroVelocity();
					}
				}
			}
		}
		for (auto player : playerList)
		{
			player->Update_Compensation(dt);
		}
		for (auto npc : npcList)
		{
			npc->Update(dt);
		}
		for (auto brick : map->GetBrickList())
		{
			if (!brick->IsDelete)
			{
				// players va chạm bricks
				for (auto player : playerList)
				{
					player->CheckCollision(brick);
				}

				// npcs va chạm bricks
				for (auto npc : npcList)
				{
					npc->CheckCollision(brick);
				}
			}
		}
	}

	// update còn object còn lại
	{
		for (auto bullet : bulletList)
		{
			bullet->Update(dt);
		}
		for (auto explosion : smallExList)
		{
			explosion->Update(dt);
		}
		for (auto explosion : bigExList)
		{
			explosion->Update(dt);
		}
		protectItem->Update(dt);
		upgradeItem->Update(dt);
		pointed->Update(dt);
	}
}

void BattleScene::Draw()
{
	for (auto player : playerList)
	{
		player->Draw();
	}
	for (auto npc : npcList)
	{
		npc->Draw();
	}
	map->Draw();
	for (auto bullet : bulletList)
	{
		bullet->Draw();
	}
	for (auto e : smallExList)
	{
		e->Draw();
	}
	for (auto e : bigExList)
	{
		e->Draw();
	}
	protectItem->Draw();
	upgradeItem->Draw();
	for (auto player : playerList)
	{
		player->DrawArrow();
	}
	pointed->Draw();

	// vẽ các cột thông số bên phải
	for (auto player : playerList)
	{
		if (player->ID == 0)
		{
			label_Player0.SetPosition(D3DXVECTOR2(850.0f, 100.0f));
			if (player->IsMy)
			{
				label_Player0.Draw("Player 0: " + to_string(player->Score), D3DCOLOR_XRGB(255, 242, 0));
			}
			else
			{
				label_Player0.Draw("Player 0: " + to_string(player->Score));
			}
		}
		else if (player->ID == 1)
		{
			label_Player1.SetPosition(D3DXVECTOR2(850.0f, 150.0f));
			if (player->IsMy)
			{
				label_Player1.Draw("Player 1: " + to_string(player->Score), D3DCOLOR_XRGB(255, 242, 0));
			}
			else
			{
				label_Player1.Draw("Player 1: " + to_string(player->Score));
			}
		}
		else if (player->ID == 2)
		{
			label_Player2.SetPosition(D3DXVECTOR2(850.0f, 200.0f));
			if (player->IsMy)
			{
				label_Player2.Draw("Player 2: " + to_string(player->Score), D3DCOLOR_XRGB(255, 242, 0));
			}
			else
			{
				label_Player2.Draw("Player 2 " + to_string(player->Score));
			}
		}
		else if (player->ID == 3)
		{
			label_Player3.SetPosition(D3DXVECTOR2(850.0f, 250.0f));
			if (player->IsMy)
			{
				label_Player3.Draw("Player 3: " + to_string(player->Score), D3DCOLOR_XRGB(255, 242, 0));
			}
			else
			{
				label_Player3.Draw("Player 3: " + to_string(player->Score));
			}
		}

		label_CountTime.SetPosition(D3DXVECTOR2(850.0f, 500.0f));
		label_CountTime.Draw("Time Up In: " + to_string((int)(count_TimeUp + 0.8f)));
	}

	if (count_TimeUp < 0)
	{
		label_TimeUp.SetPosition(D3DXVECTOR2(400.0f, 350.0f));
		label_TimeUp.Draw("TIME UP!");	
	}
}

void BattleScene::ReceivePacket(InputMemoryBitStream& _is, int _packetType)
{
	if (_packetType == PT_World)
	{
		int receivedTime = 0;
		_is.Read(receivedTime, NBit_Time);

		int nFramePrevious = (int)((TimeServer::Instance()->GetServerTime() - receivedTime) / 16.7f); // đã bao nhiêu frame trôi qua từ lúc client gửi
		
		// test check packet
		InputMemoryBitStream isTest(_is);

		{
			for (auto player : playerList)
			{
				player->Read(isTest, false, receivedTime);
			}
			for (auto bullet : bulletList)
			{
				bullet->Read(isTest, false);
			}
			for (auto npc : npcList)
			{
				npc->Read(isTest, false);
			}
			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				isTest.Read(_isDelete);
			}
			protectItem->Read(isTest, false);
			upgradeItem->Read(isTest, false);
			pointed->Read(isTest, false);
		}
		int typeTest1 = 0;
		isTest.Read(typeTest1, NBit_PacketType);

		// không nhận các packet trễ & bị drop
		if (lastReceivedTime >= receivedTime || nFramePrevious >= 70 || typeTest1 != PT_World || count_TimeUp < 0)
		{
			for (auto player : playerList)
			{
				player->Read(_is, false, receivedTime);
			}
			for (auto bullet : bulletList)
			{
				bullet->Read(_is, false);
			}
			for (auto npc : npcList)
			{
				npc->Read(_is, false);
			}
			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				_is.Read(_isDelete);
			}
			protectItem->Read(_is, false);
			upgradeItem->Read(_is, false);
			pointed->Read(_is, false);

			int pType = 0;
			_is.Read(pType, NBit_PacketType);
		}
		// nhận packet
		else
		{
			lastReceivedTime = receivedTime;

			for (auto player : playerList)
			{
				player->Read(_is, true, receivedTime);
			}
			for (auto bullet : bulletList)
			{
				bullet->Read(_is, true);
			}
			for (auto npc : npcList)
			{
				npc->Read(_is, true);
			}
			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				_is.Read(_isDelete);
				brick->IsDelete = _isDelete;
			}
			protectItem->Read(_is, true);
			upgradeItem->Read(_is, true);
			pointed->Read(_is, true);

			int pType = 0;
			_is.Read(pType, NBit_PacketType);
		}
	}
}
