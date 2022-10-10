#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
	glm::vec3 position;
	glm::vec3 worldPos;
	glm::vec3 scale;
	glm::quat orientation;
	glm::quat worldOrientation;
	glm::mat4 modelMatrix;
	bool inheritPosOnly;
	Transform():
		position(glm::vec3(0.f)),
		worldPos(glm::vec3(0.f)),
		scale(glm::vec3(1.f)),
		orientation(),
		worldOrientation(),
		modelMatrix(1.0f),
		inheritPosOnly(false)
	{}
};