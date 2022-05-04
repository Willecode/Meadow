#pragma once
#include "assets/submesh.h"
#include "assets/material.h"
#include <glm/glm.hpp>
#include "resource_management/shadermanager.h"
class Mesh
{
public:
	//static constexpr int MAX_MATERIAL_SLOTS = 10;
	std::vector<SubMesh*> meshes;
	Material* material;
public:
	Mesh();
	void draw(glm::mat4 modelMat, ShaderManager* sdrMan);
};

