#pragma once
#include <glm/vec3.hpp>
#include "../texture.h"
#include "../shader.h"


namespace MaterialConstants {
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f);
}

class Material : public Asset
{
public:
	virtual void passToRenderer() = 0;
	AssetType getAssetType() override;

};