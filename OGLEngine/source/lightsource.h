#pragma once
#include <glm/glm.hpp>
struct LightSource
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 color;
};

struct PointLight : public LightSource {
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight : public LightSource {
	glm::vec3 direction;
};