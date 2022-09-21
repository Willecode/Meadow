#include "scene.h"
#include <algorithm>
#include "input/inputevents.h"
#include "resource_management/resourcemanager.h"
#include "directionallight.h"
#include "pointlight.h"
#include "service_locator/loggerlocator.h"
Scene::Scene():
	m_nodeIdCtr(1),
	m_camera(Camera(1920.0f / 1080.0f, 0.1f, 100.0f)),
	m_deltatime(0.f),
	m_cameraLock(true),
	//m_uiNodes(),
	m_activeNode(nullptr),
	m_selectedNodes()
{
	// Initialize scene graph as just the root node
	m_nodes[0] = std::make_unique<SceneNode>(0, "root");
	/*
	* Subscribe to relevant events
	*/
	std::function<void(float, float)> mousefunc = std::bind(&Scene::mousePosHandler, this, std::placeholders::_1, std::placeholders::_2);
	InputEvents::MouseMoveEvent::subscribe(mousefunc);

	std::function<void()> mouseLockfunc = std::bind(&Scene::mouseLockHandler, this);
	InputEvents::MouseLockEvent::subscribe(mouseLockfunc);

	std::function<void()> mouseUnlockfunc = std::bind(&Scene::mouseUnlockHandler, this);
	InputEvents::MouseUnlockEvent::subscribe(mouseUnlockfunc);

	std::function<void(unsigned int)>  addNodefunc = std::bind(&Scene::addNodeHandler, this, std::placeholders::_1);
	InputEvents::AddNodeEvent::subscribe(addNodefunc);

	InputEvents::SetNodeMeshEvent::subscribe(std::bind(&Scene::setMeshHandler, this, std::placeholders::_1, std::placeholders::_2));
	InputEvents::DuplicateNodeEvent::subscribe(std::bind(&Scene::duplicateNodeHandler, this, std::placeholders::_1));
	InputEvents::SceneNodeLightsourceAddEvent::subscribe(std::bind(&Scene::addNodeLightSource, this, std::placeholders::_1));
	InputEvents::SceneNodeLightsourceRemoveEvent::subscribe(std::bind(&Scene::removeNodeLightSource, this, std::placeholders::_1));
	InputEvents::SetActiveNodeEvent::subscribe(std::bind(&Scene::setActiveNode, this, std::placeholders::_1));
	InputEvents::SetNodeSelectionEvent::subscribe(std::bind(&Scene::setNodeSelection, this, std::placeholders::_1, std::placeholders::_2));
}

void Scene::update(float deltatime, InputGather* input)
{
	/*
	* Update deltatime
	*/
	m_deltatime = deltatime;

	input->pollInputs();
	handleCameraMovement(m_deltatime, input);

	/*
	* Update each node, starting from root with root also as parent (root doesn't have transforms so it's ok)
	*/
	updateNode(m_nodes[0].get(), m_nodes[0].get(), m_camera.position);
}

void Scene::render()
{
	ShaderManager& sdrMan = ShaderManager::getInstance();
	/*
	* Set camera uniforms
	*/
	//sdrMan->setFrameUniform("view", m_camera.getViewMatrix()); // for vertex shader
	//sdrMan->setFrameUniform("projection", m_camera.getProjectionMatrix()); // for vertex shader
	//sdrMan->setFrameUniform("viewPos", m_camera.position); // for fragment shader

	/*
	* Set light uniforms
	*/
	DirectionalLight::passAllInstancesToShader(&sdrMan);
	PointLight::passAllInstancesToShader(&sdrMan);
	//Pointlight....
	//nodePassLights(m_nodeMap[0].get(), sdrMan);


	/*
	* This is a good spot to forward frame constant uniforms to GPU
	*/
	sdrMan.forwardFrameUniforms();

	/*
	* Sort the drawing order
	*/
	sortDrawQueue();
	
	/*
	* Render each node
	*/
	drawNodes(&sdrMan);
}

unsigned int Scene::addNode(unsigned int parent)
{
	if (m_nodeIdCtr >= MAX_NODES)
		throw std::length_error(std::string("Tried to add more than ") + std::to_string(MAX_NODES) + std::string(" nodes"));
	if (nodeIdInUse(parent)) {
		m_nodes[m_nodeIdCtr] = std::make_unique<SceneNode>(m_nodeIdCtr);
		m_nodes[parent]->children.push_back(m_nodes[m_nodeIdCtr].get());
		m_drawQueue.push_back(m_nodes[m_nodeIdCtr].get());
		unsigned int ret = m_nodeIdCtr;
		m_nodeIdCtr++;
		return ret;
	}
	else {
		throw std::invalid_argument("Tried to add child to nonexistent parent");
	}
}

unsigned int Scene::addNode(SceneNode* parent)
{
	/*
	* Make sure that the parent exists
	*/
	bool found = false;
	unsigned int parentId;
	for (int i = 0; i < MAX_NODES; i++) {
		if (m_nodes[i].get() == parent) {
			found = true;
			parentId = i;
			break;
		}
	}
	if (!found)
		throw std::invalid_argument("Tried to add child to nonexistent parent");
	else {
		return addNode(parentId);
	}
}


SceneNode* Scene::getNode(unsigned int id) const
{
	if (!nodeIdInUse(id))
		return nullptr;
	return m_nodes[id].get();

}

void Scene::duplicateNode(unsigned int id)
{
	SceneNode* node = getNode(id);
	if (node != nullptr) {
		/*
		* Create a new node with the same parent
		*/
		unsigned int parentId = findParent(node);
		unsigned int newNodeId = addNode(parentId);
		SceneNode* newNode = getNode(newNodeId);

		/*
		* Create a copy for each of the child nodes and make the copies children of the new node
		*/
		*newNode = *node;
		duplicateChildren(node, newNode);
	}
}

SceneNode* Scene::getActiveNode()
{
	return m_activeNode;
}

Camera* Scene::getCamera()
{
	return &m_camera;
}


void Scene::mousePosHandler(float x, float y)
{
	if (!m_cameraLock)
		m_camera.processMouseMovement(x, y, m_deltatime);
	else
		m_camera.updateMousePos(x, y);

}

void Scene::mouseLockHandler()
{
	m_cameraLock = false;
}


void Scene::mouseUnlockHandler()
{
	m_cameraLock = true;
}

void Scene::addNodeHandler(unsigned int parent)
{
	addNode(parent);
}

void Scene::duplicateNodeHandler(unsigned int parent)
{
	duplicateNode(parent);
}

void Scene::setMeshHandler(unsigned int nodeid, unsigned int meshid)
{
	if (nodeIdInUse(nodeid))
		getNode(nodeid)->setMesh(ResourceManager::getMesh(meshid));
	else
		LoggerLocator::getLogger()->getLogger()->error("Scene::setMeshHandler: Tried to set mesh to a nonexistent node");
}

void Scene::removeNodeLightSource(unsigned int nodeid)
{
	if (!nodeIdInUse(nodeid))
		return;
	else {
		m_nodes[nodeid]->setLightSource(nullptr);
	}
	
}

void Scene::addNodeLightSource(unsigned int nodeid)
{
	if (!nodeIdInUse(nodeid))
		return;
	else {
		if (!PointLight::maxInstanceCapacity()) {
			auto pointLight = std::make_unique<PointLight>();
			m_nodes[nodeid]->setLightSource(std::move(pointLight));
		}
	}
}

void Scene::setActiveNode(unsigned int nodeid)
{
	if (!nodeIdInUse(nodeid))
		return;
	else {
		if (m_activeNode != nullptr)
			m_activeNode->active = false; // remove old active status
		m_activeNode = m_nodes[nodeid].get(); // update pointer
		m_activeNode->active = true; // set new active status
	}
}

void Scene::setNodeSelection(unsigned int nodeid, bool f)
{
	if (!nodeIdInUse(nodeid))
		return;
	else {
		SceneNode* nodeptr = m_nodes[nodeid].get();

		// Select node
		if (f) {
			nodeptr->selected = true;
			m_selectedNodes.insert(nodeptr);
		}
		// Unselect node
		else
		{
			auto it = m_selectedNodes.find(nodeptr);
			if (it != m_selectedNodes.end()) {
				nodeptr->selected = false;
				m_selectedNodes.erase(it);
			}
				
		}
	}
}


void Scene::updateNode(SceneNode* node, SceneNode* parent, const glm::vec3& cameraPos)
{
	node->update(parent, cameraPos);
	for (auto child : node->children) {
		updateNode(child, node, cameraPos);
	}
}

void Scene::drawNodes(ShaderManager* sdrMan)
{
	for (int i = 0; i < m_drawQueue.size(); i++) {
		m_drawQueue[i]->render(sdrMan);
	}
}

void Scene::handleCameraMovement(float deltatime, InputGather* input)
{
	if (m_cameraLock)
		return;
	if (input->getInputFlag(InputGather::InputFlag::CameraUp))
		m_camera.inputMoveUp(m_deltatime);
	if (input->getInputFlag(InputGather::InputFlag::CameraDown))
		m_camera.inputMoveDown(deltatime);
	if (input->getInputFlag(InputGather::InputFlag::CameraForward))
		m_camera.inputMoveForward(deltatime);
	if (input->getInputFlag(InputGather::InputFlag::CameraBackward))
		m_camera.inputMoveBackward(deltatime);
	if (input->getInputFlag(InputGather::InputFlag::CameraLeft))
		m_camera.inputMoveLeft(deltatime);
	if (input->getInputFlag(InputGather::InputFlag::CameraRight))
		m_camera.inputMoveRight(deltatime);
}

unsigned int Scene::findParent(SceneNode* node)
{
	for (int i = 0; i < MAX_NODES; i++) {
		if (m_nodes[i] != nullptr) {
			auto it = std::find(m_nodes[i]->children.begin(), m_nodes[i]->children.end(), node);
			if (it != m_nodes[i]->children.end())
				return i;
		}
	}
}

void Scene::duplicateChildren(SceneNode* const source, SceneNode* destination)
{
	for (SceneNode* const child : source->children) {
		unsigned int newNodeId = addNode(destination);
		SceneNode* newNode = getNode(newNodeId);
		*newNode = *child;
		duplicateChildren(child, newNode);
	}
}

bool Scene::nodeIdInUse(unsigned int id) const
{
	if (id < MAX_NODES) {
		if (m_nodes[id] != nullptr)
			return true;
	}
	return false;
}

void Scene::sortDrawQueue()
{
	std::sort(m_drawQueue.begin(), m_drawQueue.end(), [](SceneNode* a, SceneNode* b) {
		return a->distanceFromCamera > b->distanceFromCamera;
		});
}

