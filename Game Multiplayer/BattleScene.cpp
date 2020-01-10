#include "BattleScene.h"
#include "GameLog.h"
#include "TimeServer.h"

BattleScene::BattleScene(vector<bool> _playerInRoomList, float time)
{
	lastReceivedTime = TimeServer::Instance()->ServerTime();

	count_TimeUp = time;
	map = new GameMap("Resource files/map.tmx");

	// tạo players
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

		// tạo 1 BULLET cho mỗi NPC
		Bullet* bullet = new Bullet(i, 4);
		bulletList.push_back(bullet);
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
	if (count_TimeUp < 0)
		return;

	int time1 = (int)GetTickCount();

	count_TimeUp -= dt;

	for (auto p : playerList) // nhận keyboard
	{
		if (p->IsMy)
		{
			p->HandleKeyboard(keyboard, dt);
		}
	}

	for (auto p : playerList) // set vận tốc players dựa theo direction
	{
		p->ApplyVelocity();
	}

	for (auto n : npcList) // set vận tốc npcs dựa theo direction 
	{
		n->ApplyVelocity();
	}
	
	for (auto b : bulletList) // set vận tốc bullets dựa theo direction
	{
		b->ApplyVelocity();
	}	

	for (auto p : playerList)
	{
		if (!p->IsDelete)
		{	
			for (auto n : npcList) // players va chạm npcs
			{
				if (!n->IsDelete)
				{
					if (GameCollision::IsCollideInNextFrame(p, n, dt, 1))
					{ 
						p->ZeroVelocity();
					}
					if (GameCollision::IsCollideInNextFrame(n, p, dt, 1))
					{
						n->CheckCollision(p);
					}
				}
			}

			for (auto p2 : playerList) // players va chạm với nhau
			{
				if (!p2->IsDelete && p->ID != p->ID && GameCollision::IsCollideInNextFrame(p, p2, dt, 1))
				{
					p->ZeroVelocity();
				}
			}
		}
	}

	// sau khi check vận tốc có = 0 hay không => update tọa độ
	for (auto player : playerList)
	{
		player->Update(dt);
	}
	for (auto npc : npcList)
	{
		npc->Update(dt);
	}

	// sau khi có tọa độ mới tại frame này thì check va chạm với gạch
	for (auto brick : map->GetBrickList())
	{
		if (!brick->IsDelete)
		{
			for (auto player : playerList) // players va chạm bricks
			{
				player->CheckCollision(brick);
			}

			for (auto npc : npcList) // npcs va chạm bricks
			{
				npc->CheckCollision(brick);
			}
		}
	}

	// update các object còn lại
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

		int nFramePrevious = (int)((TimeServer::Instance()->ServerTime() - receivedTime) / 16.667f); // đã bao nhiêu frame trôi qua từ lúc client gửi

		// không nhận các packet trễ || bị drop
		if (lastReceivedTime >= receivedTime || nFramePrevious >= 70 || count_TimeUp < 0)
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
				npc->Read(_is, false, receivedTime);
			}
			for (auto brick : map->GetBrickNorList())
			{
				bool _isDelete = false;
				_is.Read(_isDelete);
			}
			protectItem->Read(_is, false);
			upgradeItem->Read(_is, false);
			pointed->Read(_is, false);
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
				npc->Read(_is, true, receivedTime);
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
		}
	}
}
