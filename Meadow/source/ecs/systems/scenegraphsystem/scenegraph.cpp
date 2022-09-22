#include "scenegraph.h"

SceneGraph::SceneGraph()
{
	Node root;
	root.entity = NullEntity;
	root.children = {};
	m_rootNode = root;

}

void SceneGraph::addNode(Entity ent, Entity parent)
{
	addNodeRec(ent, parent, m_rootNode);
}

void SceneGraph::changeParent(Entity ent, Entity parent)
{
	changeParentRec(ent, parent, m_rootNode);
}

void SceneGraph::deleteNode(Entity ent)
{
	deleteNodeRec(ent, m_rootNode);
}

const SceneGraph::Node& SceneGraph::getGraph()
{
	return m_rootNode;
}

void SceneGraph::addNodeRec(Entity ent, Entity parent, Node& recNode)
{
	if (recNode.entity == parent) {
		Node n;
		n.entity = ent;
		n.children = {};
		recNode.children.push_back(n);
		return;
	}
	else {
		for (auto& Node : recNode.children) {
			addNodeRec(ent, parent, recNode);
		}
	}

}

void SceneGraph::changeParentRec(Entity ent, Entity parent, Node& recNode)
{
}

void SceneGraph::deleteNodeRec(Entity ent, Node& recNode)
{
}
