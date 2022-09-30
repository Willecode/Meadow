#include "RenderSystem.h"
#include "ecs/components/model3d.h"
#include "ecs/components/transform.h"

#include <glm/gtc/type_ptr.hpp>

void RenderSystem::init(ECSCoordinator* ecs)
{
	m_ecs = ecs;
}

void RenderSystem::update(float deltaT)
{
	for (auto& ent : m_entities) {
		Transform& trans = m_ecs->getComponent<Transform>(ent);
		Model3D& model = m_ecs->getComponent<Model3D>(ent);

		//double dArray[16] = { 0.0 };
		//if (ent == 2) {
		//	trans.modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		//}
		//const float* pSource = (const float*)glm::value_ptr(trans.modelMatrix);
		//for (int i = 0; i < 16; ++i)
		//	dArray[i] = pSource[i];

		model.mesh->draw(trans.modelMatrix);
	}
}
