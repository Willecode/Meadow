#include "coloronlymaterial.h"
#include "service_locator/locator.h"
#include "shader/shadermanager.h"

ColorOnlyMaterial::ColorOnlyMaterial(): ColorOnlyMaterial(MaterialConstants::DEFAULT_COLOR)
{
}

ColorOnlyMaterial::ColorOnlyMaterial(glm::vec3 color):
	color(color)
{
}

void ColorOnlyMaterial::passToRenderer()
{
	Locator::getRenderer()->setFloat3(ShaderManager::getColorShader()->getId(), "color", color);
}

//void ColorOnlyMaterial::passToShader(Shader* shader)
//{
//	//shader->setFloat3("color", color);
//}
