#pragma once
#include "assets/mesh.h"
#include "assets/material.h"
#include <glm/glm.hpp>
#include "resource_management/shadermanager.h"
class Model
{
public:
	//static constexpr int MAX_MATERIAL_SLOTS = 10;
	std::vector<Mesh*> meshes;
	Material* material;
public:
	Model();
	void draw(glm::mat4 modelMat, ShaderManager* sdrMan);
};

