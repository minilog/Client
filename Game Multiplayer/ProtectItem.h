#pragma once

#include "Item.h"

class ProtectItem : public Item
{
public:
	ProtectItem(D3DXVECTOR2 _pos)
	{
		Type = ET_ProtectItem;
		animation = new Animation();
		animation->AddFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 32, 32, 64, D3DXVECTOR2(16.f, 16.f)));
		BaseInit(_pos);
	}
	~ProtectItem() {}
};

