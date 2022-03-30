#pragma once
#include "renderer.h"

class OpenGLRenderer : public Renderer
{
public:
	bool initialize() override;
	void setViewportSize(int width, int height) override;
};

