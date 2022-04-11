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
	
	Camera(float aspect, float zNear, float zFar, glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	
	/*
	* Camera matrix access
	*/
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	/*
	* Camera parameters
	*/
	void setFov(float fovIn);
	float getFov();

	/*
	* Input
	*/
	void processMouseMovement(float mouseOffsetX, float mouseOffsetY, float deltaTime);
	void inputMoveUp(float deltaTime);
	void inputMoveDown(float deltaTime);
	void inputMoveLeft(float deltaTime);
	void inputMoveRight(float deltaTime);
	void inputMoveForward(float deltaTime);
	void inputMoveBackward(float deltaTime);
private:
	float fov, aspect, zNear, zFar;
};

