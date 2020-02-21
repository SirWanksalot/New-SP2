#include "SceneManager.h"
SceneManager* SceneManager::instance = 0;
SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

SceneManager* SceneManager::getInstance() {
	if (!instance) {
		instance = new SceneManager();
		return instance;
	}
	else {
		return instance;
	}
}

void SceneManager::AddScene(Scene* scene) {
	Scenes.push_back (scene);
}

void SceneManager::SetNextScene(int sceneID) {

}
Scene* SceneManager::getCurrScene() {
	return Scenes[currSceneID];
}

void SceneManager::Update(double ElapsedTime) {
	Scenes[currSceneID]->Update(ElapsedTime);
	Scenes[currSceneID]->Render();
}

int SceneManager::getCurrSceneID() {
	return currSceneID;
}

void SceneManager::setCurrSceneID(int ID) {
	currSceneID = ID;
}