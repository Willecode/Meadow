#include "camera.h"
#include "input/inputevents.h"
Camera::Camera(float aspect, float zNear, float zFar, glm::vec3 position, glm::vec3 up) :
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
{
	InputEvents::WindowDimensionsChangedEvent::subscribe(std::bind(&Camera::setAspect, this, std::placeholders::_1, std::placeholders::_2));
}
void Camera::processMouseMovement(float mouseX, float mouseY, float deltaTime) {
	if (firstMouseMove) {
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		firstMouseMove = false;
		return;
	}
	float mouseOffsetX = mouseX - lastMouseX;
	float mouseOffsetY = mouseY - lastMouseY;
	lastMouseX = mouseX;
	lastMouseY = mouseY;
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
glm::mat4 Camera::getViewMatrix() {

	return glm::lookAt(position, position + direction, worldUp);
}
glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}
void Camera::setFov(float fovIn) {
	fov = fovIn;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
void Camera::setAspect(const int width, const int height)
{
	aspect = static_cast<float>(width) / static_cast<float>(height);
}

float Camera::getFov() {
	return fov;
}

void Camera::updateMousePos(float mouseX, float mouseY)
{
	lastMouseX = mouseX;
	lastMouseY = mouseY;
}

void Camera::inputMoveUp(float deltaTime) {
	position += worldUp * speed * deltaTime;
}
void Camera::inputMoveDown(float deltaTime) {
	position -= worldUp * speed * deltaTime;
}
void Camera::inputMoveLeft(float deltaTime) {
	position -= glm::normalize(glm::cross(direction, worldUp)) * speed * deltaTime;
}
void Camera::inputMoveRight(float deltaTime) {
	position += glm::normalize(glm::cross(direction, worldUp)) * speed * deltaTime;
}
void Camera::inputMoveForward(float deltaTime) {
	position += direction * speed * deltaTime;
}
void Camera::inputMoveBackward(float deltaTime) {
	position -= direction * speed * deltaTime;
}