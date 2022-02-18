#pragma once
#include "shader_s.h"
#include "lightsource.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "mesh.h"

class Object3D
{
	Object3D();
	void draw();
	void addLightSource(LightSource light);

private:
	glm::mat4 modelMatrix;
	std::vector<Object3D*> childObjects;
	std::shared_ptr<LightSource> light;
	std::vector<std::shared_ptr<Mesh>> meshes;
};

