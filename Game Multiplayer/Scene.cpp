#include "Scene.h"


Scene::Scene()
{
	mBackColor = D3DCOLOR_XRGB(0, 0, 0);
	LoadContent();
}

Scene::~Scene() {}
void Scene::Update(float dt)
{

}

void Scene::LoadContent()
{

}

void Scene::Draw()
{

}

void Scene::ReceivePakcet()
{
}

void Scene::OnKeyDown(int keyCode)
{

}

void Scene::OnKeyUp(int keyCode)
{

}

void Scene::SendData()
{
}

D3DCOLOR Scene::GetBackcolor()
{
	return mBackColor;
}
