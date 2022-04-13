#include "scene.h"
#include "input/inputevent.h"
Scene::Scene(Dispatcher* disp):
	m_nodeMap({ {0, std::make_shared<SceneNode>()} }),
	m_nodeIdCtr(1),
	m_camera(Camera(1920.0f / 1080.0f, 0.1f, 100.0f)),
	m_dispatcher(disp),
	m_deltatime(0.f)
{
	/*
	* Subscribe to relevant events
	*/
	//std::function<void(const char*)> f = std::bind(&Scene::eventHandler, this, std::placeholders::_1);
	//m_dispatcher->subscribe(CameraUpEvent::EVENT_TYPE, f);
	std::function<void(const char*, float, float)> mousefunc = std::bind(&Scene::mousePosHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_dispatcher->subscribe2f(MouseMoveEvent::EVENT_TYPE, mousefunc);
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
	m_nodeMap[m_nodeIdCtr] = std::make_shared<SceneNode>();
	m_nodeMap[parent]->children.push_back(m_nodeMap[m_nodeIdCtr].get());
	unsigned int ret = m_nodeIdCtr;
	m_nodeIdCtr++;
	return ret;
}

SceneNode* Scene::getNode(unsigned int id)
{
	auto it = m_nodeMap.find(id);
	if (it != m_nodeMap.end()) {
		return it->second.get();
	}
	return nullptr;
}

void Scene::eventHandler(const char* eventType)
{
	Locator::getLogger()->getLogger()->info("Scene event handler processing event: {}", eventType);
}

void Scene::mousePosHandler(const char* eventType, float x, float y)
{
	m_camera.processMouseMovement(x, y, m_deltatime);
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
