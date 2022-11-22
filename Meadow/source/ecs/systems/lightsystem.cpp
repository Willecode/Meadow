#include "lightsystem.h"
#include "ecs/components/transform.h"
#include "ecs/components/light.h"
#include "shader/shadermanager.h"
#include "service_locator/rendererlocator.h"

void LightSystem::init(ECSCoordinator* ecs)
{
	m_ecs = ecs;
}

void LightSystem::update(float deltaT)
{
	ShaderManager& sdrMan = ShaderManager::getInstance();
	int pointLightId = 0;
	int dirLightId = 0;
	for (auto& ent : m_entities) {
		Transform& trans = m_ecs->getComponent<Transform>(ent);
		Light& light = m_ecs->getComponent<Light>(ent);
		
		if (light.lightType == LightType::POINTLIGHT) {
			sdrMan.setFrameUniform(injectId("pointLights[].color",     pointLightId), light.color);
			sdrMan.setFrameUniform(injectId("pointLights[].position",  pointLightId), trans.position);
			sdrMan.setFrameUniform(injectId("pointLights[].constant",  pointLightId), light.constant);
			sdrMan.setFrameUniform(injectId("pointLights[].linear",    pointLightId), light.linear);
			sdrMan.setFrameUniform(injectId("pointLights[].quadratic", pointLightId), light.quadratic);
		}

		if (light.lightType == LightType::DIRLIGHT) {
			sdrMan.setFrameUniform(injectId("dirLight.color", pointLightId), light.color);
			sdrMan.setFrameUniform(injectId("dirLight.direction", pointLightId), light.color);

		}

		pointLightId++;
		dirLightId++;
	}
	sdrMan.setFrameUniform("pointLightCount", pointLightId);
}

std::string LightSystem::injectId(const std::string& str, int id)
{
	std::string ret = str;
	std::size_t t = ret.find("[]");
	if (t != std::string::npos) {
		ret.insert(t + 1, std::to_string(id));
	}
	return ret;
}
