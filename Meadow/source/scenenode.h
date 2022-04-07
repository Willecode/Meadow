#pragma once
#include "model.h"
#include <glm/glm.hpp>
/*
* Represents an entity in 3D space. Attach these to scene graph nodes.
*/
class SceneNode
{
public:
	/*
	* Scene hierarchy
	*/
	std::vector<SceneNode*> children;

	/*
	* 3D transform
	*/
	glm::vec3 position;
	glm::vec3 scale;
	std::vector<std::pair<float, glm::vec3>> rotations; // switch to quaternions at some point

public:
	SceneNode();
	void setModel(std::unique_ptr<Model> model);
	void update();

private:
	std::unique_ptr<Model>m_model;
	//Controllercomponent m_controller;
	//PhysicsComponent m_physics;

private:
	
	/*
	* Utility
	*/
	glm::mat4 createModelMatrix();
};

