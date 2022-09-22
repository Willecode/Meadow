#if 1
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <input/inputgather.h>
class EditorCamera
{
public:
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 direction;
	float lookSensitivity;
	float yaw;
	float pitch;
	float speed;
	
	EditorCamera(float aspect, float zNear, float zFar, glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	
	void update(float dt, const InputGather& input, bool lockMovement);

	/*
	* Camera matrix access
	*/
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	/*
	* Camera parameters
	*/
	void setFov(const float fovIn);
	void setAspect(const int width, const int height);
	void setAspect(float aspect);
	float getFov();

	/*
	* Input funcs
	*/
	void processMouseMovement(float mouseX, float mouseY, float deltaTime);
	/*
	* updateMousePos should be called before using processMouseMovement, if processMouseMovement wasn't called last frame
	*/
	void updateMousePos(float mouseX, float mouseY);
	void inputMoveUp(float deltaTime);
	void inputMoveDown(float deltaTime);
	void inputMoveLeft(float deltaTime);
	void inputMoveRight(float deltaTime);
	void inputMoveForward(float deltaTime);
	void inputMoveBackward(float deltaTime);
private:
	float fov, aspect, zNear, zFar, lastMouseX, lastMouseY;
	bool firstMouseMove;
};

#endif 0