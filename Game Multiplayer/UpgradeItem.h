#pragma once

#include "Item.h"

class UpgradeItem : public Item
{
public:
	UpgradeItem(D3DXVECTOR2 _pos)
	{
		Type = ET_UpgradeItem;
		animation = new Animation();
		animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 96 + 32, 32, 32 + 32, 
			D3DXVECTOR2(16.f, 16.f)));
		BaseInit(_pos);
	}
	~UpgradeItem(){}
};

