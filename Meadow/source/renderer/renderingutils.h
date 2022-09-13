#pragma once
#include "scene/scenenode.h"

class RenderingUtils
{
public:
	/*
	* Paint mesh outlines into the stencil buffer as zeroes.
	* Assumes the following state:
	*	rend->setStencilMask(1);
	*	rend->stencilTesting(true);
	*/
	static void maskMeshOutlines(SceneNode* node);

private:
	RenderingUtils() {};
	static float outlineWidth;
};

