#if 1
#include "editorcamera.h"
//#include "input/inputevents.h"
EditorCamera::EditorCamera(float aspect, float zNear, float zFar, glm::vec3 position, glm::vec3 up) :
	aspect(aspect),
	zNear(zNear),
	zFar(zFar),
	position(position),
	yaw(-90.0f),
	pitch(0.f),
	worldUp(up),
	speed(2.0f),
	direction(0.f, 0.f, -1.0f),
	lookSensitivity(20.0f),
	fov(45.f),
	lastMouseX(-1.f),
	lastMouseY(-1.f),
	firstMouseMove(true)
{}
void EditorCamera::update(float dt, const InputGather& input, bool lockMovement)
{
	if (!lockMovement) {
		if (input.getInputFlag(InputGather::InputFlag::CameraUp))
			inputMoveUp(dt);
		if (input.getInputFlag(InputGather::InputFlag::CameraDown))
			inputMoveDown(dt);
		if (input.getInputFlag(InputGather::InputFlag::CameraForward))
			inputMoveForward(dt);
		if (input.getInputFlag(InputGather::InputFlag::CameraBackward))
			inputMoveBackward(dt);
		if (input.getInputFlag(InputGather::InputFlag::CameraLeft))
			inputMoveLeft(dt);
		if (input.getInputFlag(InputGather::InputFlag::CameraRight))
			inputMoveRight(dt);

		processMouseMovement(input.mouseX, input.mouseY, dt);
	}
	lastMouseX = input.mouseX;
	lastMouseY = input.mouseY;

}
void EditorCamera::processMouseMovement(float mouseX, float mouseY, float deltaTime) {
	
	float mouseOffsetX = mouseX - lastMouseX;
	float mouseOffsetY = mouseY - lastMouseY;
	yaw += mouseOffsetX * lookSensitivity * deltaTime;
	pitch += -mouseOffsetY * lookSensitivity * deltaTime;
	if (pitch < -89.0f)
		pitch = -89.0f;
	else if (89.0f < pitch)
		pitch = 89.0f;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	direction = glm::normalize(direction);

}
glm::mat4 EditorCamera::getViewMatrix() {

	return glm::lookAt(position, position + direction, worldUp);
}
glm::mat4 EditorCamera::getProjectionMatrix() {
	return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}
void EditorCamera::setFov(float fovIn) {
	fov = fovIn;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
void EditorCamera::setAspect(const int width, const int height)
{
	if (height != 0) // Prevent division by zero
		aspect = static_cast<float>(width) / static_cast<float>(height);
}

void EditorCamera::setAspect(float asp)
{
	aspect = asp;
}

float EditorCamera::getFov() {
	return fov;
}

void EditorCamera::updateMousePos(float mouseX, float mouseY)
{
	lastMouseX = mouseX;
	lastMouseY = mouseY;
}

void EditorCamera::inputMoveUp(float deltaTime) {
	position += worldUp * speed * deltaTime;
}
void EditorCamera::inputMoveDown(float deltaTime) {
	position -= worldUp * speed * deltaTime;
}
void EditorCamera::inputMoveLeft(float deltaTime) {
	position -= glm::normalize(glm::cross(direction, worldUp)) * speed * deltaTime;
}
void EditorCamera::inputMoveRight(float deltaTime) {
	position += glm::normalize(glm::cross(direction, worldUp)) * speed * deltaTime;
}
void EditorCamera::inputMoveForward(float deltaTime) {
	position += direction * speed * deltaTime;
}
void EditorCamera::inputMoveBackward(float deltaTime) {
	position -= direction * speed * deltaTime;
}
#endif