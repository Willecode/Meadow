#include "scene.h"

void Scene::addObject(Object3D* obj)
{
	sceneObjects.push_back(obj);
}

void Scene::drawScene()
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->draw();
	}
}

void Scene::gatherLightData()
{
}
