#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
private:
	std::vector<Scene*> Scenes;
	int currSceneID = 0;
	//int nextSceneID = 0;
	static SceneManager* instance;

public:

	SceneManager();
	~SceneManager();
	static SceneManager* getInstance();
	void AddScene(Scene* scene);
	void SetNextScene(int sceneID);
	Scene* getCurrScene();
	void Update(double ElapsedTime);
	int getCurrSceneID();
	void setCurrSceneID(int ID);
};

