#pragma once

#include <d3dx9.h>
#include <d3d9.h>

class Camera
{
	D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f);
	int width = 0, height = 0;

public:
	Camera(int _width, int _height);

	void SetPosition(D3DXVECTOR2 _pos);
	D3DXVECTOR2 GetPosition();
	int GetWidth();
	int GetHeight();
	RECT GetBound();
};
