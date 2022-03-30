#pragma once
class Renderer
{
public:
	virtual bool initialize() = 0;
	virtual void setViewportSize(int width, int height) = 0;
};

