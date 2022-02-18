#pragma once
#include "Object3D.h"
#include <vector>
class Scene
{
	void addObject(Object3D* obj);
	void drawScene();

private:
	void gatherLightData();
	std::vector<Object3D*> sceneObjects;
};

