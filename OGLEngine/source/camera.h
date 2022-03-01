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
	
	Camera(float aspect, float zNear, float zFar, glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) :
		aspect(aspect),
		zNear(zNear),
		zFar(zFar),
		position(position),
		yaw(-90.0f),
		pitch(0.f),
		worldUp(up),
		speed(2.0f),
		direction(0.f, 0.f, -1.0f),
		lookSensitivity(6.0f),
		fov(45.f)
	{
	}
	void processMouseMovement(float mouseOffsetX, float mouseOffsetY, float deltaTime) {
		yaw += mouseOffsetX * lookSensitivity * deltaTime;
		pitch += -mouseOffsetY * lookSensitivity * deltaTime;

		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		direction = glm::normalize(direction);
	}
	glm::mat4 getViewMatrix() {

		return glm::lookAt(position, position + direction, worldUp);
	}
	glm::mat4 getProjectionMatrix() {
		return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
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
	void inputMoveUp(float deltaTime) {
		position += worldUp * speed * deltaTime;
	}
	void inputMoveDown(float deltaTime) {
		position -= worldUp * speed * deltaTime;
	}
	void inputMoveLeft(float deltaTime) {
		position -= glm::normalize(glm::cross(direction, worldUp)) * speed * deltaTime;
	}
	void inputMoveRight(float deltaTime) {
		position += glm::normalize(glm::cross(direction, worldUp)) * speed * deltaTime;
	}
	void inputMoveForward(float deltaTime) {
		position += direction * speed * deltaTime;
	}
	void inputMoveBackward(float deltaTime) {
		position -= direction * speed * deltaTime;
	}
private:
	float fov, aspect, zNear, zFar;
};

