#pragma once
#include "scenenode.h"
#include "camera.h"
#include <unordered_map>
#include "resource_management/shadermanager.h"
/*
* Scene graph class
*/
class Scene
{
public:
	Scene();

	/*
	* Update state of all nodes in the scene
	*/
	void update();

	/*
	* Render all nodes in the scene
	*/
	void render(ShaderManager* sdrMan);
	unsigned int addNode(unsigned int parent = 0);
	SceneNode* getNode(unsigned int id);
	
private:
	Camera m_camera;

	/*
	* Map for accessing nodes with id. Rootnode is always 0 and added in initialization
	*/
	std::unordered_map<unsigned int, std::shared_ptr<SceneNode>> m_nodeMap;
	unsigned int m_nodeIdCtr;
private:
	void updateNode(SceneNode* node, SceneNode* parent);
	void renderNode(SceneNode* node, ShaderManager* sdrMan);
};

