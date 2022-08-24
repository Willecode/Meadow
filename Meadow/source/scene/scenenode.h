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
	bool selected;
	bool active;
	/*
	* Scene hierarchy
	*/
	std::vector<SceneNode*> children;
		
	/*
	* Local 3D transform
	*/
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 orientationEuler;
	glm::quat orientation;
	float distanceFromCamera;

	/*
	* How to render
	*/
	bool wireframeMode;

	/*
	* Lightsource?
	*/
	bool hasLightSource;

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
	glm::mat4* getModelMatrix();

	/*
	* Set lightsource. The variable put as the argument becomes unusable.
	* Pass in nullptr to remove light.
	*/
	void setLightSource(std::unique_ptr<LightSource> ls);
	LightSource* getLightsource();

	/*
	* Update node transform
	*/
	void update(SceneNode* parent, const glm::vec3& cameraPos);

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
	float calcDistanceFromCam(const glm::vec3& camPos);
	glm::vec3 getWorldPosition();
};

