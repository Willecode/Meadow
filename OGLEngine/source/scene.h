#pragma once
#include "Object3D.h"
#include <vector>
class Scene
{
public:
	void addObject(Object3D* obj);
	void drawScene();

private:
	std::vector<Object3D*> sceneObjects;
	void gatherLightData();
};

