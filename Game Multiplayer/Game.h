#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include <dinput.h>
#include "GameTime.h"
#include <thread>

class Game
{
	float FPS;

public:
	Game(int _fps = 60);
	~Game() {}

	// tạo vòng lặp game
	void InitLoop();

	void InitLoop2();

private:
	// vẽ hình sau khi update
	void Render();

	// cập nhật game
	void Update(float dt);
};

