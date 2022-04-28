#pragma once
#include "model.h"
#include <glm/glm.hpp>
#include "resource_management/shadermanager.h"
/*
* Represents a node in scene
*/
class SceneNode
{
public:
	unsigned int id;
	std::string name;
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
	SceneNode(unsigned int id = -1, std::string name = "Nameless Node");
	/*
	* Copy constr and assignment op
	*/
	SceneNode(const SceneNode& n1);
	SceneNode& operator=(const SceneNode& n1);

	/*
	* Set and get
	*/
	void setModel(std::unique_ptr<Model> model);
	Model* getModel();
	glm::mat4* getModelMatrix();
	/*
	* Update node transform
	*/
	void update(SceneNode* parent);

	/*
	* Render this node's model with set transform
	*/
	void render(ShaderManager* sdrMan);

private:
	std::unique_ptr<Model>m_model;
	//Controllercomponent m_controller;
	//PhysicsComponent m_physics;

	glm::mat4 m_modelMatrix;

private:
	
	/*
	* Update m_modelMatrix from position, scale, rotation members
	*/
	void updateModelMatrix(glm::mat4* accumulate);
};

