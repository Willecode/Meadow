#pragma once
#include "shader_s.h"
#include "lightsource.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "mesh.h"
#include "materials/material.h"

class Object3D
{
public:
	int id;

	Object3D();
	void draw(std::unordered_map<Object3D*, LightSource*> sceneLights, int pointLightCount, int dirLightCount);
	void addLightSource(LightSource* lightSrc);
	void addMesh(const Mesh* mesh);
	void setModelMatrix(const glm::mat4* model);
	void setMaterial(Material* mat);
	void setShader(Shader* sdr);
	glm::vec3 getPosition();
	LightSource* getLightSource();

protected:
	std::string name;
	glm::mat4 modelMatrix;
	std::vector<Object3D*> childObjects;
	LightSource* light;
	std::vector<const Mesh*> meshes;
	Material* material;
	Shader* shader;
};

