#include "camera.h"
#include "service_locator/locator.h"

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
	lookSensitivity(10.0f),
	fov(45.f)
{
}
void Camera::processMouseMovement(float mouseOffsetX, float mouseOffsetY, float deltaTime) {
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
float Camera::getFov() {
	return fov;
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