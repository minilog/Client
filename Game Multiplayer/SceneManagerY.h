#pragma once
#include "SceneY.h"

class SceneManagerY
{
public:
	static SceneManagerY *GetInstance();
	~SceneManagerY();
	SceneY* GetCurrentScene();
	void Update(float dt);
	void ReplaceScene(SceneY *scene);

private:
	SceneManagerY();
	static SceneManagerY     *mInstace;
	SceneY                  *mCurrentScene;
};

