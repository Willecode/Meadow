#pragma once
#include "Object3D.h"
#include "camera.h"
#include <vector>
#include <unordered_set>
class Scene
{
public:
	Scene(Camera* camera);
	void addObject(std::shared_ptr<Object3D>);
	void drawScene();
	void updateLighting();
	void updateShaders();
private:
	int objIdCounter;
	std::vector<std::shared_ptr<Object3D>> sceneObjects;

	int pointLightCount;
	int dirLightCount;
	std::unordered_map<Object3D*, LightSource*> sceneLights; // TODO: optimize this?

	std::unordered_set<Shader*> sceneShaders;
	Camera* sceneCamera;
};

