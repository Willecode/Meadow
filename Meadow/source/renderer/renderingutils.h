
#pragma once
#include "ecs/core/ecscoordinator.h"

class RenderingUtils
{
public:
	/*
	* Paint mesh outlines into the stencil buffer as zeroes.
	* Assumes the following state:
	*	rend->setStencilMask(1);
	*	rend->stencilTesting(true);
	*/
	static void maskMeshOutlines(Entity ent, ECSCoordinator& ecs);

private:
	RenderingUtils() {};
	static float outlineWidth;
};

