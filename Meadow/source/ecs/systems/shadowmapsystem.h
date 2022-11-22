#pragma once
#include "ecs/core/system.h"
#include "ecs/core/ecscoordinator.h"
#include <glm/vec3.hpp>
class ShadowMapSystem : public System
{
public:
	void init(ECSCoordinator* ecs);
	void update(float deltaT);
	struct DirectionalLight {
		glm::vec3 color;
		glm::vec3 direction;
		float shadowBias;
	};
	DirectionalLight* getDirectonalLight();
private:
	DirectionalLight m_dirLight;
	ECSCoordinator* m_ecs;
};

