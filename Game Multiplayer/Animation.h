#pragma once

#include "Sprite.h"
#include <vector>

class FrameInfo
{
public:
	Sprite* sprite;
	RECT rect;
	D3DXVECTOR2 center;

public:
	FrameInfo(Sprite* _sprite, int _left, int _right, int _top, int _bottom, const D3DXVECTOR2& _center)
	{
		sprite = _sprite;
		rect.left = _left;
		rect.right = _right;
		rect.top = _top;
		rect.bottom = _bottom;
		center = _center;
	}
};

class Animation
{
	std::vector<FrameInfo> frameInfos; // thông tin về các frame
	int frameIndex = 0; // frame hiện tại
	float time = 0.f; // thời gian chuyển frame
	float count = 0.f; // đếm => chuyển frame

public:
	// khởi tạo animation với: thời gian chuyển frame
	Animation(float _time = 0.f) : time(_time) {}
	~Animation() {}

	// thêm vào animation các frame
	void AddFrameInfo(const FrameInfo& _frameInfo)
	{
		frameInfos.push_back(_frameInfo);
	}

	void Update(float _dt);
	void Draw(const D3DXVECTOR2& _position, const D3DXVECTOR2& _offset = D3DXVECTOR2(0.0f, 0.0f), bool _isFlipHorizontal = false,
		bool _isFlipVertical = false, const D3DCOLOR& color = D3DCOLOR_ARGB(255, 255, 255, 255));

	void Reset();
};

