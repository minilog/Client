#include "GameY.h"

#include "GameGlobal.h"
#include "SceneManagerY.h"
#include <thread>
#include "TimeSceneY.h"
#include "GameLog.h"
#include "LobbyScene.h"
#include "RoomSceneY.h"

GameY::GameY(int fps)
{
	mFPS = fps;

	timeScene = new TimeSceneY();

	SceneManagerY::GetInstance()->ReplaceScene(new LobbyScene());

	InitLoop();
}


void GameY::Update(float dt)
{
	// nhận packet
	timeScene->ReceivePacket();
	//SceneManagerY::GetInstance()->GetCurrentScene()->ReceivePacket();

	// cập nhật Màn theo dt
	timeScene->Update(dt);
	SceneManagerY::GetInstance()->GetCurrentScene()->Update(dt);


	Render();
}
void GameY::Render()
{
	auto device = GameGlobal::GetCurrentDevice();
	auto scene = SceneManagerY::GetInstance()->GetCurrentScene();
	device->Clear(0, NULL, D3DCLEAR_TARGET, timeScene->GetBackcolor(), 0.0f, 0);

	{
		device->BeginScene();

		//bat dau ve
		GameGlobal::GetCurrentSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);

		//draw here
		scene->Draw();
		timeScene->Draw();

		//ket thuc ve
		GameGlobal::GetCurrentSpriteHandler()->End();

		device->EndScene();
	}

	device->Present(0, 0, 0, 0);
}

void GameY::InitLoop()
{
	MSG msg;

	float tickPerFrame = 1.0f / 60, delta = 0;

	while (GameGlobal::isGameRunning)
	{
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

