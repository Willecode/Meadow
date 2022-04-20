#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 
#include "windowmanager.h"
#include <glm/glm.hpp>

/*
* Containst the data for a UI element representing a single UI node (links to other nodes through children)
*/
struct SceneNodeUI {
	int id;
	std::string* name;
	glm::vec3* scale;
	glm::vec3* pos;
	std::vector<SceneNodeUI> children;
};
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
	void renderInterface(SceneNodeUI* node);

private:
	/*
	* Event handlers
	*/
	void mouseButtonReleaseHandler(const char* eventType);
	void mouseButtonPressHandler(const char* eventType);
	void mousePosHandler(const char* eventType, float x, float y);

	/*
	* Other
	*/
	void processNode(SceneNodeUI* node);
};

