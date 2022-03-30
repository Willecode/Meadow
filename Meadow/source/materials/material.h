#pragma once
#include <glm/vec3.hpp>
#include "../texture.h"
#include "../shader_s.h"


namespace MaterialConstants {
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f);
}

class Material
{
public:
	virtual void passToShader(Shader* shader) {};
protected:
	
};

