#include "Entity.h"

void Entity::Write(OutputMemoryBitStream& os)
{
	os.Write((int)Type, 6); // dao động từ 0 - 63 => 6 bit
	os.Write(NetworkID, 10); // dao động từ 0 - 1023 => 10 bit
}

D3DXVECTOR2 Entity::GetPosition()
{
	return D3DXVECTOR2(x, y);
}

RECT Entity::GetBound()
{
	RECT bound;

	bound.left = (long)(x - width / 2.f);
	bound.right = (long)(x + width / 2.f);
	bound.top = (long)(y - height / 2.f);
	bound.bottom = (long)(y + height / 2.f);

	return bound;
}

void Entity::SetPosition(D3DXVECTOR2 pos)
{
	x = pos.x;
	y = pos.y;
}

void Entity::AddPosition(D3DXVECTOR2 pos)
{
	this->SetPosition(this->GetPosition() + pos);
}

void Entity::SetWidth(int width)
{
	this->width = width;
}

int Entity::GetWidth()
{
	return width;
}

void Entity::SetHeight(int height)
{
	this->height = height;
}

int Entity::GetHeight()
{
	return height;
}

float Entity::GetVx()
{
	return vx;
}

void Entity::SetVx(float vx)
{
	this->vx = vx;
}

void Entity::AddVx(float vx)
{
	this->vx += vx;
}

float Entity::GetVy()
{
	return vy;
}

void Entity::SetVy(float vy)
{
	this->vy = vy;
}

void Entity::AddVy(float vy)
{
	this->vy += vy;
}
