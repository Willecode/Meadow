#include "scene.h"
#include <glm/gtc/type_ptr.hpp>
#include "materials/phongmaterial.h"
#include "primitivecreation.h"
#include "shadermanager.h"

Scene::Scene() :
	objIdCounter(0),
	pointLightCount(0),
	dirLightCount(0),
	sceneCamera(Camera((float)WindowManager::width / (float)WindowManager::height, 0.1f, 100.f)),
	outlineShader(Shader("shaders/object.vs", "shaders/coloronly.fs")),
	outlineMaterial(ColorOnlyMaterial(SceneConstants::DEFAULT_OUTLINE_COLOR))
{
	this->setupDefaultScene();
}

void Scene::addObject(std::shared_ptr<GraphicsComponent> obj)
{
	// if object is new, give it an ID and add it to the scene
	obj->id = objIdCounter;
	//sceneObjects.push_back(obj);
	sceneObjectsUnselected.insert({ obj->id, obj });
	objIdCounter++;
}

void Scene::drawScene()
{
	// Send shaders camera uniforms
	for (auto shader: sceneShaders) {
		shader->use();
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(sceneCamera.getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(sceneCamera.getProjectionMatrix()));
		shader->setFloat3("viewPos", sceneCamera.position); 

	}

	glEnable(GL_DEPTH_TEST);
	

	// Draw non selected (no outline)
	glStencilMask(0x00);
	for (auto it = sceneObjectsUnselected.begin(); it != sceneObjectsUnselected.end(); it++) {
		it->second->draw(sceneLights, pointLightCount, dirLightCount);
	}

	// Draw selected (outline effect)
	// Write selected obj fragments to stencil buffer for outline effect
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	for (auto it = sceneObjectsSelected.begin(); it != sceneObjectsSelected.end(); it++) {
		it->second->draw(sceneLights, pointLightCount, dirLightCount);
	}

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST); // to see outline through objects

	outlineShader.use();
	outlineMaterial.passToShader(&outlineShader);
	outlineShader.setMat4f("view", sceneCamera.getViewMatrix());
	outlineShader.setMat4f("projection", sceneCamera.getProjectionMatrix());
	//Draw outlines
	for (auto it = sceneObjectsSelected.begin(); it != sceneObjectsSelected.end(); it++) {
		glm::mat4 mat = it->second->getModelMatrix();
		glm::mat4 matScaled = glm::scale(mat, glm::vec3(SceneConstants::DEFAULT_OUTLINE_WIDTH_FAC));
		outlineShader.setMat4f("model", matScaled);
		std::vector<Mesh*> objMeshes = it->second->getMeshes();
		for (auto mesh : objMeshes) {
			mesh->draw();
		}
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);

}
/*
TODO:
this is very inefficient
*/
void Scene::updateLighting()
{
	sceneLights.clear();

	for (auto it = sceneObjectsUnselected.begin(); it != sceneObjectsUnselected.end(); it++) {
		if (it->second->getLightSource()) {
			sceneLights.insert(std::pair<GraphicsComponent*, LightSource*>(it->second.get(), it->second->getLightSource()));
			if (it->second->getLightSource()->getType() == LightSource::LightType::POINTLIGHT)
				pointLightCount++;
			else if (it->second->getLightSource()->getType() == LightSource::LightType::DIRLIGHT)
				dirLightCount++;
		}
	}

	for (auto it = sceneObjectsSelected.begin(); it != sceneObjectsSelected.end(); it++) {
		if (it->second->getLightSource()) {
			sceneLights.insert(std::pair<GraphicsComponent*, LightSource*>(it->second.get(), it->second->getLightSource()));
			if (it->second->getLightSource()->getType() == LightSource::LightType::POINTLIGHT)
				pointLightCount++;
			else if (it->second->getLightSource()->getType() == LightSource::LightType::DIRLIGHT)
				dirLightCount++;
		}
	}

}

void Scene::updateShaders()
{
	sceneShaders.clear();
	
	for (auto it = sceneObjectsUnselected.begin(); it != sceneObjectsUnselected.end(); it++) {
		Shader* s = it->second->getShader();
		if (s)
			sceneShaders.insert(s);
	}

	for (auto it = sceneObjectsSelected.begin(); it != sceneObjectsSelected.end(); it++) {
		Shader* s = it->second->getShader();
		if (s)
			sceneShaders.insert(s);
	}
}

void Scene::selectObject(int objId)
{
	auto it = sceneObjectsUnselected.find(objId);
	if (it != sceneObjectsUnselected.end()) {
		sceneObjectsSelected.insert({ objId, std::move(it->second) });
		sceneObjectsUnselected.erase(it);
	}
}

void Scene::deselectObject(int objId)
{
	auto it = sceneObjectsSelected.find(objId);
	if (it != sceneObjectsSelected.end()) {
		sceneObjectsUnselected.insert({ objId, std::move(it->second) });
		sceneObjectsUnselected.erase(it);
	}
}

void Scene::deselectAllObjects()
{
	for (auto const& pair : sceneObjectsSelected)
	{
		sceneObjectsUnselected.insert({ pair.first, pair.second });
	}
	sceneObjectsSelected.clear();
}

void Scene::setupDefaultScene()
{
	auto sphere = std::make_shared<GraphicsComponent>();
	auto sphereMat = std::make_shared<PhongMaterial>();
	sphere->addMesh(PrimitiveCreation::createSphere(10, 15), 0);
	sphere->setMaterial(sphereMat, 0);
	sphere->setShader(ShaderManager::getPhongShader());

	auto dirLightObj = std::make_shared<GraphicsComponent>();
	dirLightObj->addLightSource(std::make_shared<DirectionalLight>(glm::vec3(-0.5f, -0.5f, 0.0f)));

	this->addObject(sphere);
	this->addObject(dirLightObj);
}

