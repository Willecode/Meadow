#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 
#include <glm/glm.hpp>
#include "windowmanager.h"
#include "assets/asset.h"

/*
* Contains the data for a UI element representing an asset 
*/
struct AssetUI {
	std::string name;
	const Asset::AssetType type;
	const unsigned int id;
	AssetUI(std::string name, const Asset::AssetType type, const unsigned int id):
		name(name), type(type), id(id)
	{}
};
/*
* Containst the data for a UI element representing a single scene node (links to other nodes through children)
*/
struct SceneNodeUI {
	int id;
	std::string* name;
	glm::vec3* scale;
	glm::vec3* pos;
	std::vector<SceneNodeUI> children;
	bool hasGraphics;
	std::vector<std::string> meshes;
	std::string material;
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
	void renderInterface(SceneNodeUI* node, std::vector<AssetUI>* uiAssets);

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

