
#include "renderingutils.h"
#include "service_locator/rendererlocator.h"
#include "ecs/components/model3d.h"
#include "ecs/components/transform.h"
float RenderingUtils::outlineWidth = 1.05f;

void RenderingUtils::maskMeshOutlines(Entity ent, ECSCoordinator& ecs)
{
	auto sign = ecs.getEntitySignature(ent);
	if (!sign.test(ecs.getComponentType<Model3D>())) {
		return;
	}
	auto model = ecs.getComponent<Model3D>(ent);
	if (model.mesh == nullptr)
		return;

	Renderer* rend = RendererLocator::getRenderer();
	rend->depthTesting(false); // disable depth testing
	rend->depthMask(false); // disable depth writing
	rend->setColorMask(false); // disable color writing

	rend->setStencilFunc(Renderer::TestingFuncs::ALWAYS, 1, 0xFF);
	rend->setStencilOp(
		Renderer::TestingActions::ZERO,
		Renderer::TestingActions::ZERO,
		Renderer::TestingActions::ZERO); // We're rendering 0's to the stencil buf

	auto t = ecs.getComponent<Transform>(ent);
	glm::mat4 modelMat = t.modelMatrix; // copy model matrix
	glm::mat4 scaledModelMat = glm::scale(modelMat, glm::vec3(outlineWidth)); // scale the modelmat

	model.mesh->drawWithoutMaterial(scaledModelMat); // render scaled mesh

	rend->setStencilFunc(Renderer::TestingFuncs::ALWAYS, 1, 0xFF);
	rend->setStencilOp(
		Renderer::TestingActions::REPLACE,
		Renderer::TestingActions::REPLACE,
		Renderer::TestingActions::REPLACE); // Render mesh in 1's

	model.mesh->drawWithoutMaterial(modelMat); // render mesh
	
	rend->setStencilMask(0);

	return;
}
