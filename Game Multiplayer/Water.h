#pragma once
#include "Brick.h"
class Water : public Brick
{
public:
	Water(D3DXVECTOR3 position);
	~Water();


	RECT rect();
};

