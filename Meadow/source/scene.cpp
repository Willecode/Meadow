#include "scene.h"

Scene::Scene():
	m_nodeMap({ {0, std::make_shared<SceneNode>()} }),
	m_nodeIdCtr(1)
{
}

void Scene::update()
{
	updateNode(m_nodeMap[0].get());
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

void Scene::updateNode(SceneNode* node)
{
	node->update();
	for (auto child : node->children) {
		updateNode(child);
	}
}
