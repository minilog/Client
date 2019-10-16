#include "BrickNormal.h"



BrickNormal::BrickNormal(D3DXVECTOR3 position)
{
	Init(position);
	Tag = EntityTypes::Brick;
	bound = Entity::GetBound();
}

BrickNormal::~BrickNormal()
{
	delete mSprite;
}


RECT BrickNormal::rect() {
	reg.left = 0;
	reg.right = this->GetWidth();
	reg.top = 128;
	reg.bottom = 128 + this->GetHeight();

	return reg;
}

void BrickNormal::Update()
{
}

void BrickNormal::CollisionWith(Entity* en)
{
	isDelete = true;
}



RECT BrickNormal::GetBound()
{
	return bound;
}
