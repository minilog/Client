#include "GameY.h"

#include "GameGlobal.h"
#include "SceneManagerY.h"
#include <thread>
#include "TimeSceneY.h"


GameY::GameY(int fps)
{
	mFPS = fps;

	SceneManagerY::GetInstance()->ReplaceScene(new TimeSceneY());

	InitLoop();
}


void GameY::Update(float dt)
{
	SceneManagerY::GetInstance()->GetCurrentScene()->Update(dt);
	Render();
}
void GameY::Render()
{
	auto device = GameGlobal::GetCurrentDevice();
	auto scene = SceneManagerY::GetInstance()->GetCurrentScene();
	device->Clear(0, NULL, D3DCLEAR_TARGET, scene->GetBackcolor(), 0.0f, 0);

	{
		device->BeginScene();

		//bat dau ve
		GameGlobal::GetCurrentSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);

		//draw here
		scene->Draw();

		//ket thuc ve
		GameGlobal::GetCurrentSpriteHandler()->End();

		device->EndScene();
	}

	device->Present(0, 0, 0, 0);
}

void GameY::InitLoop()
{
	MSG msg;

	//std::thread task_receive_packet(ReceivePacket);
	//task_receive_packet.detach();
	float tickPerFrame = 1.0f / 60, delta = 0;

	while (GameGlobal::isGameRunning)
	{
		SceneManagerY::GetInstance()->GetCurrentScene()->ReceivePacket();
	
		GameTime::GetInstance()->StartCounter();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		delta += GameTime::GetInstance()->GetCouter();

		if (delta >= tickPerFrame)
		{
			GameGlobal::fps = 1 / delta;
			Update(1.0f / 60);
			delta = 0;
		}
		else
		{
			int delta_time = tickPerFrame - delta;
			Sleep(delta_time);
			delta = tickPerFrame;
		}
	}
}

