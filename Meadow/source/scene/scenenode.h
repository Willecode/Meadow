#pragma once
#include "assets/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "resource_management/shadermanager.h"
#include "lightsource.h"
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
	glm::vec3 orientationEuler;
	glm::quat orientation;

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
	void setMesh(Mesh* mesh);
	Mesh* getMesh();
	void setLightSource(std::unique_ptr<LightSource> ls);
	LightSource* getLightsource();
	glm::mat4* getModelMatrix();
	/*
	* Update node transform
	*/
	void update(SceneNode* parent);

	/*
	* Render this node's mesh with set transform
	*/
	void render(ShaderManager* sdrMan);

private:
	Mesh* m_mesh;
	std::unique_ptr<LightSource> m_light;
	//Controllercomponent m_controller;
	//PhysicsComponent m_physics;

	glm::mat4 m_modelMatrix;

private:
	
	/*
	* Update m_modelMatrix from position, scale, rotation members
	*/
	void updateModelMatrix(glm::mat4* accumulate);
};

