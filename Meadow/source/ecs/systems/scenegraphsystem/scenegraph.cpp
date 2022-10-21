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

SceneGraph::Node* SceneGraph::getNode(Entity ent)
{
	Node* n = nullptr;
	getNodeRec(ent, &m_rootNode, n);
	return n;
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
	Node* n = getNode(ent);
	Node* p = getNode(parent);
	if (parented(parent, ent))
		return; // ent is parent's parent!
	if (n != nullptr && p != nullptr) { // ent and parent exist
		Node cp = *n;
		deleteNode(ent);
		p = getNode(parent); // get parent again after pointer invalidation
		p->children.push_back(cp);

	}
	
}

void SceneGraph::deleteNodeRec(Entity ent, Node& recNode)
{
	auto it = std::find(recNode.children.begin(), recNode.children.end(), ent);
	if (it != recNode.children.end()) {
		recNode.children.erase(it);
		return;
	}
	for (auto& Node : recNode.children) {
		deleteNodeRec(ent, Node);
	}
}

void SceneGraph::markNodeTransformStale(Entity ent)
{
	Node* n = getNode(ent);
	if (n != nullptr) {
		markNodeTransformStaleRec(*n);
	}
}

void SceneGraph::markNodeTransformStaleRec(Node& recNode) {
	recNode.transformStale = true;
	for (auto& child : recNode.children) {
		markNodeTransformStaleRec(child);
	}
}

bool SceneGraph::parented(Entity child, Entity parent)
{
	Node* ptr = nullptr;
	getNodeRec(child, getNode(parent), ptr);
	if (ptr)
		return true; // child is parent's child
	else
		return false;
}

//void SceneGraph::addNodeRec(Node node, Entity parent, Node& recNode)
//{
//	if (recNode.entity == parent) {
//		Node n;
//		n.entity = ent;
//		n.children = {};
//		recNode.children.push_back(n);
//		return;
//	}
//	else {
//		for (auto& Node : recNode.children) {
//			addNodeRec(ent, parent, recNode);
//		}
//	}
//
//}
void SceneGraph::getNodeRec(Entity ent, Node* recNode, Node*& result)
{
	if (recNode->entity == ent) {
		result = recNode;
	}
	else {
		for (int i = 0; i < recNode->children.size(); i++) {
			getNodeRec(ent, &recNode->children[i], result);
		}
	}
}
