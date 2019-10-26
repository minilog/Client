#pragma once
#include "Scene.h"

class SceneManager
{
	static SceneManager *instance;
	Scene *currentScene;

private:
	SceneManager();
public:
	static SceneManager* Instance();
	~SceneManager() {}
	Scene* GetCurrentScene();
	void ReplaceScene(Scene* _scene);
};

