#pragma once
#include "shader_s.h"
#include "lightsource.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "mesh.h"
#include "material.h"

class Object3D
{
public:
	Object3D();
	void draw();
	void addLightSource(LightSource light);
	void setModelMatrix(const glm::mat4* model);
	void setMaterial(Material* mat);
	void setShader(Shader* sdr);

protected:
	glm::mat4 modelMatrix;
	std::vector<Object3D*> childObjects;
	//std::shared_ptr<LightSource> light;
	std::vector<Mesh> meshes;
	Material* material;
	Shader* shader;
};

