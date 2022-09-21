#if 0
#include "renderingutils.h"
#include "service_locator/rendererlocator.h"
float RenderingUtils::outlineWidth = 1.05f;

void RenderingUtils::maskMeshOutlines(SceneNode* node)
{
	Renderer* rend = RendererLocator::getRenderer();
	rend->depthTesting(false); // disable depth testing
	rend->depthMask(false); // disable depth writing
	rend->setColorMask(false); // disable color writing

	rend->setStencilFunc(Renderer::TestingFuncs::ALWAYS, 1, 0xFF);
	rend->setStencilOp(
		Renderer::TestingActions::ZERO,
		Renderer::TestingActions::ZERO,
		Renderer::TestingActions::ZERO); // We're rendering 0's to the stencil buf

	glm::mat4 modelMat = *node->getModelMatrix(); // copy model matrix
	glm::mat4 scaledModelMat = glm::scale(modelMat, glm::vec3(outlineWidth)); // scale the modelmat

	if (node->getMesh() != nullptr)
		node->getMesh()->drawWithoutMaterial(scaledModelMat); // render scaled mesh

	rend->setStencilFunc(Renderer::TestingFuncs::ALWAYS, 1, 0xFF);
	rend->setStencilOp(
		Renderer::TestingActions::REPLACE,
		Renderer::TestingActions::REPLACE,
		Renderer::TestingActions::REPLACE); // Render mesh in 1's

	if (node->getMesh() != nullptr)
		node->getMesh()->drawWithoutMaterial(modelMat); // render mesh
	
	rend->setStencilMask(0);

	return;
}
#endif