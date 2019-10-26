#include "Camera.h"

Camera::Camera(int _width, int _height)
{
	width = _width;
	height = _height;
}

void Camera::SetPosition(D3DXVECTOR2 _pos)
{
	position = _pos;
}

D3DXVECTOR2 Camera::GetPosition()
{
	return position;
}

RECT Camera::GetBound()
{
	RECT bound;

	bound.left = (long)(position.x - width / 2.f);
	bound.right = bound.left + width;;
	bound.top = (long)(position.y - height / 2.f);
	bound.bottom = bound.top + height;

	return bound;
}

int Camera::GetWidth()
{
	return width;
}

int Camera::GetHeight()
{
	return height;
}
