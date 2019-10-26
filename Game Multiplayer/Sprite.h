#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Sprite
{
protected:
	D3DXIMAGE_INFO imageInfo;
	LPDIRECT3DTEXTURE9 texture;
	D3DXMATRIX matrix; // use for Draw

	D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // default Pos(0, 0, 0)
	D3DXVECTOR3 center = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // sprite center when you Draw it
	RECT rect;
	D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f); // default scale(1, 1)
	bool isFlipHorizontal = false; // default flip = false
	bool isFlipVertical = false;
	float rotation = 0.0f; // default angle = 0
	D3DXVECTOR2 offset = D3DXVECTOR2(0.0f, 0.0f); // default offset = (0, 0)

public:
	Sprite(const char* path, const D3DCOLOR &subColor = NULL);
	~Sprite();
	void Draw(const D3DCOLOR &color = D3DCOLOR_ARGB(255, 255, 255, 255));

	void SetFipHorizontal(bool flipHor);
	void SetFlipVertical(bool flipVer);
	void SetRect(const RECT& _rect);
	void SetOffset(const D3DXVECTOR2& _offset);
	void SetPosition(const D3DXVECTOR3& _position);
	void SetPosition(const D3DXVECTOR2& _position);
	void SetCenter(const D3DXVECTOR3& _center);
	void SetCenter(const D3DXVECTOR2& _center);
	void SetScale(const D3DXVECTOR2& _scale);
};
