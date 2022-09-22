#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include "windowmanager.h"
#include "assets/asset.h"
#include "ui/uiflags.h"
#include <backends/imgui_impl_opengl3.h>
#include "assets/texture.h"
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
	std::unordered_map <Texture::TextureType, unsigned int> textures;
	MaterialUI(std::string name, const unsigned int id) :
		AssetUI(name, Asset::AssetType::MATERIAL, id),
		propertiesf(),
		propertiesv3(),
		textures()
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

struct UIAssetMaps {
	std::map<const unsigned int, TextureUI>  textures;
	std::map<const unsigned int, MeshUI>	 meshes;
	std::map<const unsigned int, SubmeshUI>  submeshes;
	std::map<const unsigned int, MaterialUI> materials;
};
/*
* Containst the data for a UI element representing a single entity (links to other entities through children)
*/
struct EntityUI {
	int id;
	int uiElemId;
	std::string name;
	std::vector<EntityUI> children;
	bool selected;
	bool active;
	EntityUI() :
		id(-1),
		uiElemId(-1),
		name("Placeholder name"),
		children({}),
		selected(false),
		active(false) {}

};

struct IComponentUI{
	virtual void render() = 0;
};
struct TransformComponentUI : public IComponentUI {
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 scale;
	void render() override {
		ImGui::Text("transform component");
	}
};
struct Model3DComponentUI : public IComponentUI {
	void render() override {
		ImGui::Text("model3d component");
	}
};
using ComponentMapUI = std::map<int, std::vector<std::unique_ptr<IComponentUI>>>;

struct PostprocessingFlags {
	bool sharpness;
	bool grayscale;
	bool negative;
	bool MSAA;
};

/*
* Contains data about the scene's state
*/
//struct SceneState
//{
//	// Optional because it can be empty (no active node)
//	std::optional<EntityUI> activeNode;
//};

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
	void renderInterface(EntityUI* node, UIAssetMaps* uiAssets, PostprocessingFlags* postprocFlags, const ComponentMapUI* componentMap);

private:
	/*
	* Asset inspector things
	*/
	unsigned int m_chosenAssetId;
	Asset::AssetType m_chosenAssetType;

	// for node inspector
	int m_activeNode;
	/*
	* Various UI flags
	*/
	UIFlags m_uiFlags;

	/*
	* Mapping from texture type to label string
	*/
	static std::map<Texture::TextureType, std::string> m_texLabels;

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
	void processNode(EntityUI* node, UIAssetMaps* uiAssets);
	void createSceneTree(EntityUI* rootNode, ImGuiTreeNodeFlags treeflags);
	void createMatTexCombos(MaterialUI* mat, UIAssetMaps* uiAssets);
};

