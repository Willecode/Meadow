#include "coloronlymaterial.h"

ColorOnlyMaterial::ColorOnlyMaterial(): ColorOnlyMaterial(MaterialConstants::DEFAULT_COLOR)
{
}

ColorOnlyMaterial::ColorOnlyMaterial(glm::vec3 color):
	color(color)
{
}

void ColorOnlyMaterial::passToShader(Shader* shader)
{
	shader->setFloat3("color", color);
}
