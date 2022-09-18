#include "lightsystem.h"
#include "ecs/components/transform.h"
#include "ecs/components/light.h"
#include "resource_management/shadermanager.h"
void LightSystem::init()
{
}

void LightSystem::update(float deltaT, ECSCoordinator& ecs)
{
	ShaderManager& sdrMan = ShaderManager::getInstance();
	int pointLightId = 0;
	int dirLightId = 0;
	for (auto& ent : m_entities) {
		Transform& trans = ecs.getComponent<Transform>(ent);
		Light& light = ecs.getComponent<Light>(ent);
		
		if (light.lightType == LightType::POINTLIGHT) {
			sdrMan.setFrameUniform(injectId("pointLights[].color",     pointLightId), light.color);
			sdrMan.setFrameUniform(injectId("pointLights[].position",  pointLightId), trans.position);
			sdrMan.setFrameUniform(injectId("pointLights[].constant",  pointLightId), light.constant);
			sdrMan.setFrameUniform(injectId("pointLights[].linear",    pointLightId), light.linear);
			sdrMan.setFrameUniform(injectId("pointLights[].quadratic", pointLightId), light.quadratic);
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
