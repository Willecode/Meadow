#pragma once
#include <glm/vec3.hpp>
#include "../texture.h"
#include "../shader_s.h"

const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f, 102.f / 255, 178.f / 255);
class Material
{
public:
	virtual void passToShader(Shader* shader) {};

};

