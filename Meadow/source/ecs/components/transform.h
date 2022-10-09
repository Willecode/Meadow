#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
	glm::mat4 modelMatrix;
	Transform():
		position(glm::vec3(0.f)),
		scale(glm::vec3(1.f)),
		orientation(),
		modelMatrix(1.0f)
	{}
};