#pragma once
#include "graphicscomponent.h"
#include "camera.h"
#include "materials/coloronlymaterial.h"
#include "windowmanager.h"

#include <vector>
#include <unordered_set>

namespace SceneConstants {
	const glm::vec3 DEFAULT_OUTLINE_COLOR = glm::vec3(.5f, .5f, .0f);
	const float DEFAULT_OUTLINE_WIDTH_FAC = 1.05f;
}
class Scene
{
public:
	Scene();
	
	void addObject(std::shared_ptr<GraphicsComponent>);
	void drawScene();
	void updateLighting();
	void updateShaders();
	void selectObject(int objId);
	void deselectObject(int objId);
	void deselectAllObjects();
private:
	int objIdCounter;
	//std::vector<std::shared_ptr<Object3D>> sceneObjects;
	std::unordered_map<int, std::shared_ptr<GraphicsComponent>> sceneObjectsSelected, sceneObjectsUnselected;
	int pointLightCount;
	int dirLightCount;
	std::unordered_map<GraphicsComponent*, LightSource*> sceneLights; // TODO: optimize this?
	std::unordered_set<Shader*> sceneShaders;
	Shader outlineShader;
	ColorOnlyMaterial outlineMaterial;
	Camera sceneCamera;

	void setupDefaultScene();
};

