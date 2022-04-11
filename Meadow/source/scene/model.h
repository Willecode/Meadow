#pragma once
#include "assets/mesh.h"
#include "assets/material.h"
#include <glm/glm.hpp>
#include "resource_management/shadermanager.h"
class Model
{
public:
	//static constexpr int MAX_MATERIAL_SLOTS = 10;

public:
	Model();
	void addMesh(Mesh* mesh);
	void setMaterial(Material* material);
	void draw(glm::mat4 modelMat, ShaderManager* sdrMan);
private:
	std::vector<Mesh*> m_meshes;
	Material* m_material;
};

