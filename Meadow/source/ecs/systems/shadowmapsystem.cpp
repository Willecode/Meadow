#include "shadowmapsystem.h"
#include "service_locator/rendererlocator.h"
#include "ecs/components/transform.h"
#include "ecs/components/model3d.h"
#include <glm/ext/matrix_transform.hpp>

void ShadowMapSystem::init(ECSCoordinator* ecs)
{
	m_ecs = ecs;
	m_dirLight.direction = glm::vec3(-1.f);
	m_dirLight.shadowBias = 0.005;
	m_dirLight.color = glm::vec3(1.0f);
}

void ShadowMapSystem::update(float deltaT)
{
	ShaderManager& sdrMan = ShaderManager::getInstance();
	sdrMan.bindShader(ShaderManager::ShaderType::SHADOWMAP);

	glm::mat4 depthProjectionMatrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(0.f) - m_dirLight.direction, glm::vec3(0.f, 0.0f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 depthVP = depthProjectionMatrix * depthViewMatrix;
	sdrMan.setFrameUniform("shadowmapVP", depthVP);
	sdrMan.setFrameUniform("dirLightDir", m_dirLight.direction);
	sdrMan.setFrameUniform("dirLightColor", m_dirLight.color);
	sdrMan.setFrameUniform("shadowBias", m_dirLight.shadowBias);

	sdrMan.forwardFrameUniforms();

	for (auto& ent : m_entities) {
		Transform& trans = m_ecs->getComponent<Transform>(ent);
		Model3D& model = m_ecs->getComponent<Model3D>(ent);

		if (model.mesh != nullptr) {
			if (model.wireframe)
				RendererLocator::getRenderer()->wireframe(true);
			else
				RendererLocator::getRenderer()->wireframe(false);

			model.mesh->drawWithoutMaterial(trans.modelMatrix);
		}
	}
	
}

ShadowMapSystem::DirectionalLight* ShadowMapSystem::getDirectonalLight()
{
	return &m_dirLight;
}
