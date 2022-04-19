#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 
#include "windowmanager.h"
class UI
{
public:
	/*
	* Clean up
	*/
	~UI();
	/*
	* Lazy initialization
	*/
	void init(WindowManager* winMan, Dispatcher* disp);
	/*
	* Render UI
	*/
	void renderInterface();

private:
	void mouseButtonReleaseHandler(const char* eventType);
	void mouseButtonPressHandler(const char* eventType);
	void mousePosHandler(const char* eventType, float x, float y);
};

