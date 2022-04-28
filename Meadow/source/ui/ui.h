#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 
#include <glm/glm.hpp>
#include <vector>
#include <map>
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
	int uiElemId;
	std::string* name;
	glm::vec3* scale;
	glm::vec3* pos;
	std::vector<SceneNodeUI> children;
	bool hasGraphics;
	AssetUI* mesh;
	AssetUI* material;
};

class UI
{
public:
	typedef std::map<const unsigned int, AssetUI> UIAssetMap;
public:
	UI();
	/*
	* Clean up
	*/
	~UI();
	/*
	* Lazy initialization
	*/
	void init(WindowManager* winMan);
	/*
	* Render UI
	*/
	void renderInterface(SceneNodeUI* node, std::map<Asset::AssetType, UI::UIAssetMap>* uiAssets);

private:
	/*
	* Event handlers
	*/
	void mouseButtonReleaseHandler();
	void mouseButtonPressHandler();
	void mousePosHandler(float x, float y);

	/*
	* Other
	*/
	void processNode(SceneNodeUI* node, std::map<Asset::AssetType, UI::UIAssetMap>* uiAssets);
};

