#include "SceneManagerY.h"

SceneManagerY* SceneManagerY::mInstace = NULL;

SceneManagerY::SceneManagerY()
{
	mCurrentScene = nullptr;
}

SceneManagerY::~SceneManagerY()
{
}

SceneManagerY* SceneManagerY::GetInstance()
{
	if (!mInstace)
		mInstace = new SceneManagerY();

	return mInstace;
}

SceneY* SceneManagerY::GetCurrentScene()
{
	return mCurrentScene;
}

void SceneManagerY::Update(float dt)
{
	mCurrentScene->Update(dt);
}

void SceneManagerY::ReplaceScene(SceneY *scene)
{
	delete mCurrentScene;

	mCurrentScene = scene;
}