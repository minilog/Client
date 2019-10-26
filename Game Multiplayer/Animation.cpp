#include "Animation.h"

void Animation::Update(float _dt)
{
	// Do have any frame info, return
	if (frameInfos.size() <= 1)
	{
		return;
	}

	count += _dt;

	if (count >= time)
	{
		frameIndex++;

		if (frameIndex >= (int)frameInfos.size())
		{
			frameIndex = 0;
		}

		count -= time;
	}
}

void Animation::Draw(const D3DXVECTOR2 & _position, const D3DXVECTOR2 & _offset, bool _isFlipHorizontal, bool _isFlipVertical, const D3DCOLOR & color)
{
	FrameInfo currentFrameInfo = frameInfos[frameIndex];

	Sprite* sprite = currentFrameInfo.sprite;
	sprite->SetRect(currentFrameInfo.rect);
	sprite->SetCenter(currentFrameInfo.center);

	sprite->SetPosition(_position);
	sprite->SetOffset(_offset);
	sprite->SetFipHorizontal(_isFlipHorizontal);
	sprite->SetFlipVertical(_isFlipVertical);

	sprite->Draw(color);
}

void Animation::Reset()
{
	frameIndex = 0;
	count = 0.f;
}
