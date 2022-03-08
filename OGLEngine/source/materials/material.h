#pragma once
#include <glm/vec3.hpp>
#include "../texture.h"
#include "../shader_s.h"




class Material
{
public:
	virtual void passToShader(Shader* shader) {};
protected:
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f);
};

