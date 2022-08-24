#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <optional>
#include "windowmanager.h"
#include "assets/asset.h"
#include "ui/uiflags.h"

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
struct TextureUI : public AssetUI {
	TextureUI(std::string name, const unsigned int id) :
		AssetUI(name, Asset::AssetType::TEXTURE, id)
	{}
};
struct MaterialUI : public AssetUI {
	std::unordered_map<std::string, float>* propertiesf;
	std::unordered_map<std::string, glm::vec3>* propertiesv3;
	unsigned int diffuseMap;
	unsigned int specularMap;
	unsigned int opacityMap;
	MaterialUI(std::string name, const unsigned int id) :
		AssetUI(name, Asset::AssetType::MATERIAL, id),
		propertiesf(),
		propertiesv3(),
		diffuseMap(0),
		specularMap(0),
		opacityMap(0)
	{}
};
struct SubmeshUI : public AssetUI{
	SubmeshUI(std::string name, const unsigned int id):
		AssetUI(name, Asset::AssetType::SUBMESH, id)
	{}
};
struct MeshUI : public AssetUI{
	/*
	* Map submeshUI id -> materialUI id
	*/
	std::map<unsigned int, unsigned int> submeshes;
	MeshUI(std::string name, const unsigned int id) :
		AssetUI(name, Asset::AssetType::MESH, id)
	{}
};
struct LightSourceUI {

};

struct UIAssetMaps {
	std::map<const unsigned int, TextureUI>  textures;
	std::map<const unsigned int, MeshUI>	 meshes;
	std::map<const unsigned int, SubmeshUI>  submeshes;
	std::map<const unsigned int, MaterialUI> materials;
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
	glm::vec3* orientationEuler;
	std::vector<SceneNodeUI> children;
	MeshUI* mesh;
	bool* wireframeMode;
	bool hasLightsource;
	bool selected;
	bool active;
	SceneNodeUI() :
		id(-1),
		uiElemId(-1),
		name(nullptr),
		scale(nullptr),
		pos(nullptr),
		orientationEuler(nullptr),
		children({}),
		mesh(nullptr),
		wireframeMode(nullptr),
		hasLightsource(false),
		selected(false),
		active(false) {}

};

struct PostprocessingFlags {
	bool sharpness;
	bool grayscale;
	bool negative;
	bool MSAA;
};

/*
* Contains data about the scene's state
*/
struct SceneState
{
	// Optional because it can be empty (no active node)
	std::optional<SceneNodeUI> activeNode;
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
	void renderInterface(SceneNodeUI* node, SceneState* sceneState, UIAssetMaps* uiAssets, PostprocessingFlags* postprocFlags);

private:
	/*
	* Asset inspector things
	*/
	unsigned int m_chosenAssetId;
	Asset::AssetType m_chosenAssetType;

	/*
	* Various UI flags
	*/
	UIFlags m_uiFlags;

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
	void processNode(SceneNodeUI* node, UIAssetMaps* uiAssets);
	//SceneNodeUI* getNodeById(unsigned int id, SceneNodeUI* rootNode);
};

