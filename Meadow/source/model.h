#pragma once
#include "mesh.h"
#include "materials/material.h"
#include <glm/glm.hpp>
class Model
{
public:
	static constexpr int MAX_MATERIAL_SLOTS = 10;

public:
	Model();
	void addMesh(Mesh* mesh);
	void addMaterial(Material* material);
	void draw(glm::mat4 modelMat);
private:
	std::vector<Mesh*> m_meshes;
};

