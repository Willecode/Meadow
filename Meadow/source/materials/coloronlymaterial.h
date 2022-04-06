#pragma once
#include "material.h"
class ColorOnlyMaterial: public Material
{
public:
	ColorOnlyMaterial();
	ColorOnlyMaterial(glm::vec3 color);
	void passToRenderer() override;
private:
	glm::vec3 color;
};

