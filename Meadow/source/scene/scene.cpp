#include "scene.h"
#include "input/inputevents.h"
#include "resource_management/resourcemanager.h"
Scene::Scene():
	m_nodeMap({ {0, std::make_shared<SceneNode>(0,"root")}}), // Initialize scene graph as just the root node
	m_nodeIdCtr(1),
	m_camera(Camera(1920.0f / 1080.0f, 0.1f, 100.0f)),
	m_deltatime(0.f),
	m_cameraLock(true),
	m_uiNodes()
{
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
	InputEvents::SetNodeMaterialEvent::subscribe(std::bind(&Scene::setMaterialHandler, this, std::placeholders::_1, std::placeholders::_2));
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
	updateNode(m_nodeMap[0].get(), m_nodeMap[0].get());
}

void Scene::render(ShaderManager* sdrMan)
{
	/*
	* Set camera uniforms
	*/
	sdrMan->setFrameUniform("view", m_camera.getViewMatrix());
	sdrMan->setFrameUniform("projection", m_camera.getProjectionMatrix());
	/*
	* This is a good spot to forward frame constant uniforms to GPU
	*/
	sdrMan->forwardFrameUniforms();

	/*
	* Render each node, starting from root
	*/
	renderNode(m_nodeMap[0].get(), sdrMan);
}

unsigned int Scene::addNode(unsigned int parent)
{
	if (m_nodeMap.find(parent) != m_nodeMap.end()) {
		m_nodeMap[m_nodeIdCtr] = std::make_shared<SceneNode>(m_nodeIdCtr);
		m_nodeMap[parent]->children.push_back(m_nodeMap[m_nodeIdCtr].get());
		unsigned int ret = m_nodeIdCtr;
		m_nodeIdCtr++;
		return ret;
	}
}


SceneNode* Scene::getNode(unsigned int id) const
{
	auto it = m_nodeMap.find(id);
	if (it != m_nodeMap.end()) {
		return it->second.get();
	}
	return nullptr;
}

//void Scene::scrapeData(SceneNodeUI &uiNode) const
//{
//	scrapeNode(m_nodeMap[0].get(), uiNode, 0);
//}


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

void Scene::setMeshHandler(unsigned int nodeid, unsigned int meshid)
{
	auto it = m_nodeMap.find(nodeid);
	if (it == m_nodeMap.end()) {
		Locator::getLogger()->getLogger()->error("Sene::setMeshHandler: Tried to set mesh to a nonexistent node");
		return;
	}
	else
	{
		it->second->getModel()->meshes = { ResourceManager::getMesh(meshid) };
	}

}

void Scene::setMaterialHandler(unsigned int nodeid, unsigned int matid)
{
	auto it = m_nodeMap.find(nodeid);
	if (it == m_nodeMap.end()) {
		Locator::getLogger()->getLogger()->error("Sene::setMaterialHandler: Tried to set material to a nonexistent node");
		return;
	}
	else
	{
		it->second->getModel()->material = ResourceManager::getMaterial(matid);
	}
}

void Scene::updateNode(SceneNode* node, SceneNode* parent)
{
	node->update(parent);
	for (auto child : node->children) {
		updateNode(child, node);
	}
}

void Scene::renderNode(SceneNode* node, ShaderManager* sdrMan)
{
	node->render(sdrMan);
	for (auto child : node->children) {
		renderNode(child, sdrMan);
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

//void Scene::scrapeNode(SceneNode* node, SceneNodeUI &uiNode, int uiElemId) const
//{
//	uiNode = SceneNodeUI();
//	uiNode.id = uiElemId;
//	uiNode.name = &node->name;
//	uiNode.scale = &node->scale;
//	uiNode.pos = &node->position;
//	Model* model = node->getModel();
//	if (model != nullptr) {
//		uiNode.hasGraphics = true;
//		for (auto const& x : model->meshes)
//			uiNode.meshes.push_back(x->name);
//		uiNode.material = model->material->name;
//	}
//	else
//		uiNode.hasGraphics = false;
//	for (auto child : node->children) {
//		SceneNodeUI uiChild;
//		uiNode.children.push_back(uiChild);
//		scrapeNode(child, uiNode.children.back(), uiElemId++);
//	}
//}
