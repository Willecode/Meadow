#pragma once
#include "ecs/core/ecscoordinator.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "ecs/core/editorcamera.h"
#include "input/inputgather.h"

class CameraSystem : public System
{
public:
	CameraSystem();
	void init();
	void update(float deltaT, ECSCoordinator& ecs, const InputGather& input);
	void setAspect(const int width, const int height);

private:
	glm::vec3 m_worldUp;
	glm::mat4 getViewMatrix(const glm::vec3& pos, const glm::vec3& dir);
	glm::mat4 getProjectionMatrix(const float& fov, const float& aspect, const float& zNear, const float& zFar);
	void cameraLock();
	void cameraUnlock();

	float m_aspectRatio;
	bool m_editorCameraMode;
	EditorCamera m_editorCamera;
	bool m_cameraLock;
};

