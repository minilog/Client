#include "GameDebugDraw.h"
#include "GameGlobal.h"

GameDebugDraw::GameDebugDraw()
{
	mSpriteHandler = GameGlobal::XSprite;
	mSpriteHandler->GetDevice(&mddv);
	D3DXCreateLine(mddv, &LineDraw);

	//set size cho line
	LineDraw->SetWidth(5);

	//set mau cho line
	mColor = D3DCOLOR_XRGB(255, 0, 0);

}

GameDebugDraw::~GameDebugDraw()
{

}

void GameDebugDraw::Draw()
{

}

void GameDebugDraw::setLineSize(float width)
{
	LineDraw->SetWidth(width);
}

void GameDebugDraw::DrawLine(D3DXVECTOR2 lines[], int count)
{
	LineDraw->Begin();
	LineDraw->Draw(lines, count, mColor);
	LineDraw->End();
}

void GameDebugDraw::DrawRect(RECT rect, Camera *camera)
{

	int a = rect.top;
	rect.top = rect.bottom;
	rect.bottom = a;
	D3DXVECTOR3 trans = D3DXVECTOR3(0, 0, 0);


	D3DXVECTOR2 lines[] = { D3DXVECTOR2((float)rect.left, (float)GameGlobal::Height - rect.top),
		D3DXVECTOR2((float)rect.right, (float)GameGlobal::Height - rect.top),
		D3DXVECTOR2((float)rect.right, (float)GameGlobal::Height - rect.bottom),
		D3DXVECTOR2((float)rect.left, (float)GameGlobal::Height - rect.bottom),
		D3DXVECTOR2((float)rect.left, (float)GameGlobal::Height - rect.top) };

	DrawLine(lines, 5);
}

void GameDebugDraw::setColor(D3DCOLOR color)
{
	mColor = color;
}