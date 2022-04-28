#pragma once
#include <unordered_map>
#include "scenenode.h"
#include "camera.h"
#include "resource_management/shadermanager.h"
#include "input/inputgather.h"
#include "ui/ui.h"
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
	void update(float deltatime, InputGather* input);

	/*
	* Render all nodes in the scene
	*/
	void render(ShaderManager* sdrMan);
	unsigned int addNode(unsigned int parent = 0);
	SceneNode* getNode(unsigned int id) const;

	/*
	* Get a UI representation of the scene graph with references to the real data //moved to a separate datascraper class
	*/
	//void scrapeData(SceneNodeUI &uiNode) const;
	
private:
	Camera m_camera;
	bool m_cameraLock;
	float m_deltatime;
	/*
	* Map for accessing nodes with id.
	* Scenenodes only exist in a scene, and are unique
	* Rootnode is always 0 and added in initialization
	*/
	std::unordered_map<unsigned int, std::shared_ptr<SceneNode>> m_nodeMap;
	unsigned int m_nodeIdCtr;

	/*
	* Data to provide to UI
	*/
	std::vector<SceneNodeUI> m_uiNodes;
private:
	/*
	* Event handlers
	*/
	void mousePosHandler(float x, float y);
	void mouseLockHandler();
	void mouseUnlockHandler();
	void addNodeHandler(unsigned int parent = 0);
	void setMeshHandler(unsigned int nodeid, unsigned int meshid);
	void setMaterialHandler(unsigned int nodeid, unsigned int matid);
	/*
	* Other funcs
	*/
	void updateNode(SceneNode* node, SceneNode* parent);
	void renderNode(SceneNode* node, ShaderManager* sdrMan);
	void handleCameraMovement(float deltatime, InputGather* input);
	//void scrapeNode(SceneNode* node, SceneNodeUI &uiNode, int uiElemId) const;
};

