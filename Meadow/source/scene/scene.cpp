#include "scene.h"

Scene::Scene():
	m_nodeMap({ {0, std::make_shared<SceneNode>()} }),
	m_nodeIdCtr(1),
	m_camera(Camera(1920.0f / 1080.0f, 0.1f, 100.0f))
{
}

void Scene::update()
{
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
