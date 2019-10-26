#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{
	currentScene = nullptr;
}

SceneManager* SceneManager::Instance()
{
	if (instance == nullptr)
		instance = new SceneManager();

	return instance;
}

Scene* SceneManager::GetCurrentScene()
{
	return currentScene;
}

void SceneManager::ReplaceScene(Scene *scene)
{
	delete currentScene;

	currentScene = scene;
}