#pragma once
#include "material.h"
class ColorOnlyMaterial: public Material
{
public:
	ColorOnlyMaterial();
	ColorOnlyMaterial(glm::vec3 color);
	void passToShader(Shader* shader)override;
private:
	glm::vec3 color;
};

