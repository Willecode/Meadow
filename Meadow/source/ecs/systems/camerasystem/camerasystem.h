#pragma once
#include "ecs/core/ecscoordinator.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "ecs/systems/camerasystem/editorcamera.h"
#include "input/inputgather.h"

class CameraSystem : public System
{
public:
	CameraSystem();
	void init(ECSCoordinator* ecs);
	void update(float deltaT, const InputGather& input);
	void setAspect(const int width, const int height);

private:
	ECSCoordinator* m_ecs;
	glm::vec3 m_worldUp;
	glm::mat4 getViewMatrix(const glm::vec3& pos, const glm::vec3& dir);
	glm::mat4 getProjectionMatrix(const float& fov, const float& aspect, const float& zNear, const float& zFar);
	void cameraLock();
	void cameraUnlock();
	void setActiveCamera(Entity ent);
	void sceneCameraTrue();
	void sceneCameraFalse();
	glm::vec3 rotateVecByQuat(const glm::vec3& v, const glm::quat& q);

	float m_aspectRatio;
	bool m_editorCameraMode;
	EditorCamera m_editorCamera;
	bool m_cameraLock;
	Entity m_activeCam;
};

