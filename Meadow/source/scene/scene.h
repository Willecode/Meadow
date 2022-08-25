#pragma once
#include <unordered_map>
#include <set>
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
	unsigned int addNode(SceneNode* parent);
	SceneNode* getNode(unsigned int id) const;
	void duplicateNode(unsigned int id);

	SceneNode* getActiveNode();

	Camera* getCamera();
	
private:
	Camera m_camera;
	bool m_cameraLock;
	float m_deltatime;

	/*
	* Selected nodes and the active node ptrs for convenience
	*/
	SceneNode* m_activeNode;
	std::set<SceneNode*> m_selectedNodes;

	/*
	* Map for accessing nodes with id.
	* Scenenodes only exist in a scene, and are unique
	* Rootnode is always 0 and added in initialization
	*/
	//std::unordered_map<unsigned int, std::shared_ptr<SceneNode>> m_nodeMap;
	static constexpr unsigned int MAX_NODES = 200;
	std::array<std::unique_ptr<SceneNode>, MAX_NODES> m_nodes;
	unsigned int m_nodeIdCtr;

	/*
	* Nodes of the scene in drawing order.
	*/
	std::vector<SceneNode*> m_drawQueue;

	/*
	* Data to provide to UI
	*/
	//std::vector<SceneNodeUI> m_uiNodes;
private:
	/*
	* Event handlers
	*/
	void mousePosHandler(float x, float y);
	void mouseLockHandler();
	void mouseUnlockHandler();
	void addNodeHandler(unsigned int parent = 0);
	void duplicateNodeHandler(unsigned int parent);
	void setMeshHandler(unsigned int nodeid, unsigned int meshid);
	void removeNodeLightSource(unsigned int nodeid);
	void addNodeLightSource(unsigned int nodeid);
	void setActiveNode(unsigned int nodeid);
	void setNodeSelection(unsigned int nodeid, bool f);
	/*
	* Other funcs
	*/
	void updateNode(SceneNode* node, SceneNode* parent, const glm::vec3& cameraPos);
	void drawNodes(ShaderManager* sdrMan);
	void handleCameraMovement(float deltatime, InputGather* input);
	unsigned int findParent(SceneNode* node);
	void duplicateChildren(SceneNode* const source, SceneNode* destination);
	bool nodeIdInUse(unsigned int id) const;
	void sortDrawQueue();
};

