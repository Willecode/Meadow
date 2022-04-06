#pragma once
#include "worldentity.h"
#include <unordered_map>
/*
* Scene graph class
*/
class Scene
{
public:
	Scene() = default;
private:
	struct Node {
		WorldEntity* entity;
		Node* parent;
	};

	std::vector<WorldEntity> m_entities;
	std::unordered_map<unsigned int, Node> m_nodeMap;
};

