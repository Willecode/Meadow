#pragma once
#include "scenenode.h"
#include "camera.h"
#include <unordered_map>
/*
* Scene graph class
*/
class Scene
{
public:
	Scene();
	void update();
	unsigned int addNode(unsigned int parent = 0);
	SceneNode* getNode(unsigned int id);
	
private:
	Camera m_camera;
	// Map for accessing nodes with id. Rootnode is always 0 and added in initialization
	std::unordered_map<unsigned int, std::shared_ptr<SceneNode>> m_nodeMap;
	unsigned int m_nodeIdCtr;
private:
	void updateNode(SceneNode* node);
};

