#pragma once
#include "Object3D.h"
#include "camera.h"
#include "materials/coloronlymaterial.h"
#include <vector>
#include <unordered_set>

namespace SceneConstants {
	const glm::vec3 DEFAULT_OUTLINE_COLOR = glm::vec3(.5f, .5f, .0f);
	const float DEFAULT_OUTLINE_WIDTH_FAC = 1.05f;
}
class Scene
{
public:
	Scene(Camera* camera);
	void addObject(std::shared_ptr<Object3D>);
	void drawScene();
	void updateLighting();
	void updateShaders();
	void selectObject(int objId);
private:
	int objIdCounter;
	//std::vector<std::shared_ptr<Object3D>> sceneObjects;
	std::unordered_map<int, std::shared_ptr<Object3D>> sceneObjectsSelected, sceneObjectsUnselected;
	int pointLightCount;
	int dirLightCount;
	std::unordered_map<Object3D*, LightSource*> sceneLights; // TODO: optimize this?

	std::unordered_set<Shader*> sceneShaders;
	Shader outlineShader;
	ColorOnlyMaterial outlineMaterial;
	Camera* sceneCamera;
};

