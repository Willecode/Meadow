#pragma once
#include <unordered_map>
#include "scenenode.h"
#include "camera.h"
#include "resource_management/shadermanager.h"
#include "input/observer.h"
#include "input/inputgather.h"
#include "input/dispatcher.h"
/*
* Scene graph class
*/
class Scene : public Observer
{
public:
	Scene(Dispatcher* disp);

	/*
	* Update state of all nodes in the scene
	*/
	void update(float deltatime, InputGather* input);

	/*
	* Render all nodes in the scene
	*/
	void render(ShaderManager* sdrMan);
	unsigned int addNode(unsigned int parent = 0);
	SceneNode* getNode(unsigned int id);
	
private:
	Camera m_camera;
	Dispatcher* m_dispatcher;
	float m_deltatime;
	/*
	* Map for accessing nodes with id. Rootnode is always 0 and added in initialization
	*/
	std::unordered_map<unsigned int, std::shared_ptr<SceneNode>> m_nodeMap;
	unsigned int m_nodeIdCtr;
private:
	/*
	* Override Observer class input handler func
	*/
	void eventHandler(const char* eventType) override;
	void mousePosHandler(const char* eventType, float x, float y);
	void updateNode(SceneNode* node, SceneNode* parent);
	void renderNode(SceneNode* node, ShaderManager* sdrMan);
	void handleCameraMovement(float deltatime, InputGather* input);
};

