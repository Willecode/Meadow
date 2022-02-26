#include "scene.h"

Scene::Scene(): objIdCounter(0), pointLightCount(0), dirLightCount(0)
{
}

void Scene::addObject(Object3D* obj)
{
	// if object is new, give it an ID and add it
	obj->id = objIdCounter;
	sceneObjects.push_back(obj);
	objIdCounter++;
}

void Scene::drawScene()
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->draw(sceneLights, pointLightCount, dirLightCount);
	}
}
/*
TODO:
this is very inefficient
*/
void Scene::updateLighting()
{
	sceneLights.clear();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		if (sceneObjects[i]->getLightSource()) {
			sceneLights.insert(std::pair<Object3D*, LightSource*>(sceneObjects[i], sceneObjects[i]->getLightSource()));
			if (sceneObjects[i]->getLightSource()->getType() == LightSource::LightType::POINTLIGHT)
				pointLightCount++;
			else if (sceneObjects[i]->getLightSource()->getType() == LightSource::LightType::DIRLIGHT)
				dirLightCount++;
		}
	}
}
