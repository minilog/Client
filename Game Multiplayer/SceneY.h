#pragma once

#include "GameGlobal.h"

#include <vector>
#include <map>
#include "Label.h"

class SceneY
{
public:
	SceneY()
	{
		mBackColor = D3DCOLOR_XRGB(0, 0, 0);
	}

	~SceneY() {}

	virtual void Update(float _dt) {}
	virtual void Draw() {}
	virtual void ReceivePacket() {}

	virtual void OnKeyDown(int _keyCode) 
	{
		keyboard[_keyCode] = true; 
	}

	virtual void OnKeyUp(int _keyCode)
	{
		keyboard[_keyCode] = false;
	}

	D3DCOLOR GetBackcolor()
	{
		return mBackColor;
	}

protected:
	D3DCOLOR                mBackColor;
	std::map<int, bool> keyboard;
};

