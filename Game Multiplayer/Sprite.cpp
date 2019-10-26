#include "Sprite.h"
#include "GameGlobal.h"

Sprite::Sprite(const char * path, const D3DCOLOR & subColor)
{
	D3DXGetImageInfoFromFileA(path, &imageInfo);

	// sprite rect
	rect.left = 0;
	rect.right = imageInfo.Width;
	rect.top = 0;
	rect.bottom = imageInfo.Height;

	D3DXCreateTextureFromFileExA(
		GameGlobal::Device,
		path,
		imageInfo.Width,
		imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		subColor, // discard this color from Image
		&imageInfo,
		NULL,
		&texture);
}

Sprite::~Sprite()
{
	if (texture != NULL)
		texture->Release();
}

void Sprite::Draw(const D3DCOLOR & color)
{
	// create matrix from 2D info

	D3DXVECTOR2 scalingCenter = D3DXVECTOR2(position.x, position.y);
	D3DXVECTOR2 rotationCenter = scalingCenter;

	D3DXMatrixTransformation2D(
		&matrix,
		&scalingCenter, // scaling center
		0, // scale rotation
		&scale, // scale
		&rotationCenter, // rotation center
		rotation, // rotation
		&offset);

	D3DXMATRIX oldMatrix;

	// Draw from 3D info

	GameGlobal::XSprite->GetTransform(&oldMatrix);
	GameGlobal::XSprite->SetTransform(&matrix);

	GameGlobal::XSprite->Draw(
		texture,
		&rect,
		&center,
		&position,
		color);

	GameGlobal::XSprite->SetTransform(&oldMatrix); // set oldMatrix => Xprite only apply for this class
}

void Sprite::SetFipHorizontal(bool flipHor)
{
	if (isFlipHorizontal != flipHor)
	{
		isFlipHorizontal = flipHor;
		scale.x = -scale.x;
	}
}

void Sprite::SetFlipVertical(bool flipVer)
{
	if (isFlipVertical != flipVer)
	{
		isFlipVertical = flipVer;
		scale.y = -scale.y;
	}
}

void Sprite::SetRect(const RECT & _rect)
{
	rect = _rect;
}

void Sprite::SetOffset(const D3DXVECTOR2 & _offset)
{
	offset = D3DXVECTOR2((float)(int)_offset.x, (float)(int)_offset.y);
}

void Sprite::SetPosition(const D3DXVECTOR3 & _position)
{
	position = D3DXVECTOR3((float)(int)_position.x, (float)(int)_position.y, (float)(int)_position.z);
}

void Sprite::SetPosition(const D3DXVECTOR2 & _position)
{
	position = D3DXVECTOR3((float)(int)_position.x, (float)(int)_position.y, 0.0f);
}

void Sprite::SetCenter(const D3DXVECTOR3 & _center)
{
	center = D3DXVECTOR3((float)(int)_center.x, (float)(int)_center.y, (float)(int)_center.z);
}

void Sprite::SetCenter(const D3DXVECTOR2 & _center)
{
	center = D3DXVECTOR3((float)(int)_center.x, (float)(int)_center.y, 0.0f);
}

void Sprite::SetScale(const D3DXVECTOR2 & _scale)
{
	scale = _scale;
	if (isFlipHorizontal)
		scale.x = -scale.x;
	if (isFlipVertical)
		scale.y = -scale.y;
}
