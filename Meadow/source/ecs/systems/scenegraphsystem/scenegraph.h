#pragma once
#include "ecs/core/ecstypes.h"
#include <vector>
#include <array>
/*
* Holds scene hierarchy
*/
using namespace ECSTypes;

class SceneGraph
{
public:
	struct Node {
		ECSTypes::Entity entity;
		std::vector<Node> children;
		bool transformStale;
		bool operator==(const Entity& ent) {
			return this->entity == ent;
		}
		Node(Entity entity = NullEntity): entity(NullEntity), transformStale(true) {};
	};
	SceneGraph();
	// if no parent provided then parented to root
	void addNode(Entity ent, Entity parent = NullEntity);
	void changeParent(Entity ent, Entity parent);
	void deleteNode(Entity ent);
	const Node& getGraph();
	Node* getNode(Entity ent);
	void markNodeTransformStale(Entity ent);
private:
	Node m_rootNode;
	void validate();
	void addNodeRec(Entity ent, Entity parent, Node& recNode);
	void changeParentRec(Entity ent, Entity parent, Node& recNode);
	void deleteNodeRec(Entity ent, Node& recNode);
	void getNodeRec(Entity ent, Node* recNode, Node*& result);
	bool parented(Entity child, Entity parent);
	void markNodeTransformStaleRec(Node& recNode);
	//std::array<Node, MAX_ENTITIES> m_nodes;
	//unsigned int m_nodeIdCtr;
};

