#pragma once
#include "../mesh.h"

/*
* I only know OpenGL as I'm writing this class, so this may need to be
* modified to provide a more general Graphics api wrapper.
*/
class Renderer
{
public:
	virtual bool initialize() = 0;
	virtual void setViewportSize(int width, int height) = 0;
	virtual void meshBuffersGenerate(int meshId) = 0;
	virtual void meshBuffersPushData(int meshId,
		const std::vector<Vertex> &vertices,
		const std::vector<unsigned int> &indices) = 0;
	virtual void meshBuffersDelete(int meshId) = 0;
	virtual void draw(int meshId) = 0;
};

