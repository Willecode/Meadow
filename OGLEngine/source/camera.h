#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 direction;
	float lookSensitivity;
	float yaw;
	float pitch;
	float speed;
	
	


	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) :
		position(position),
		yaw(-90.0f),
		pitch(0.f),
		worldUp(up),
		speed(0.5f),
		direction(0.f, 0.f, -1.0f),
		lookSensitivity(0.1f),
		fov(45.f)
	{
	}
	void processMouseMovement(float mouseOffsetX, float mouseOffsetY) {
		yaw += mouseOffsetX * lookSensitivity;
		pitch += -mouseOffsetY * lookSensitivity;

		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		direction = glm::normalize(direction);
	}
	glm::mat4 getViewMatrix() {

		return glm::lookAt(position, position + direction, worldUp);
	}
	void setFov(float fovIn) {
		fov = fovIn;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 45.0f)
			fov = 45.0f;
	}
	float getFov() {
		return fov;
	}
private:
	float fov;
};

