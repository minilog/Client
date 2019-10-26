#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include <dinput.h>
#include "GameTime.h"
#include "TimingScene.h"

class Game
{
	float FPS;
	TimingScene* timingScene;

public:
	Game(int _fps = 60);
	~Game() {}

private:
	// tạo vòng lặp game
	void InitLoop();

	// vẽ hình sau khi update
	void Render();

	// cập nhật game
	void Update(float dt);

	// khởi tạo socket và kết nối với server
	void CreateSocketAndTryConnectToServer();
};

