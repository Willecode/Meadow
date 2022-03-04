#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "shader_s.h"
#include "lightsource.h"
#include "mesh.h"
#include "materials/material.h"

class Object3D
{
public:
	int id;

	Object3D();
	void draw(std::unordered_map<Object3D*, LightSource*> sceneLights, int pointLightCount, int dirLightCount);
	void addLightSource(std::shared_ptr<LightSource> lightSrc);
	void addMesh(std::shared_ptr<Mesh> mesh);
	void setModelMatrix(const glm::mat4& model);
	void setMaterial(std::shared_ptr<Material> mat);
	void setShader(Shader* sdr);

	std::shared_ptr<LightSource> getLightSourceOwnerhip();
	LightSource* getLightSource();
	glm::vec3 getPosition();
	Shader* getShader();

protected:
	std::string name;
	glm::mat4 modelMatrix;
	std::vector<std::shared_ptr<Object3D>> childObjects;
	std::shared_ptr<LightSource> light;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::shared_ptr<Material> material;
	Shader* shader;
};

