#include "scene.h"
#include <glm/gtc/type_ptr.hpp>
Scene::Scene(Camera* camera): objIdCounter(0), pointLightCount(0), dirLightCount(0), sceneCamera(camera)
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
	// Send shaders camera uniforms
	for (auto shader: sceneShaders) {
		shader->use();
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(sceneCamera->getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(sceneCamera->getProjectionMatrix()));
		shader->setFloat3("viewPos", sceneCamera->position); 

	}
	// Draw all Objects
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

void Scene::updateShaders()
{
	sceneShaders.clear();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneShaders.insert(sceneObjects[i]->getShader());
	}
}
