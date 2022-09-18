#pragma once
#include <glm/vec3.hpp>
enum class LightType{
	POINTLIGHT = 0,
	DIRLIGHT
};
struct Light {
	LightType lightType;
	glm::vec3 color;
    glm::vec3 direction;
    // Attenuation related
    // ------------------------
    float constant;
    float linear;
    float quadratic;
    // ------------------------
    Light():
        lightType(LightType::POINTLIGHT),
        color(1.0f),
        direction(-1.0f),
        constant(1.0f),
        linear(0.09f),
        quadratic(0.032f)
    {}
};