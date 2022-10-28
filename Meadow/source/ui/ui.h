#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include "windowmanager.h"
#include "assets/asset.h"
#include "ui/uiflags.h"
#include <backends/imgui_impl_opengl3.h>
#include "assets/texture.h"
#include "input/inputevents.h"
#include "ecs/core/internalevents.h"

// Probably shouldn't be here
// ----------------------------------
#include "ecs/components/rigidbody.h"
// ----------------------------------

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
	std::unordered_map<std::string, glm::vec4>* propertiesv4;
	std::unordered_map <Texture::TextureType, unsigned int> textures;
	MaterialUI(std::string name, const unsigned int id) :
		AssetUI(name, Asset::AssetType::MATERIAL, id),
		propertiesf(),
		propertiesv3(),
		propertiesv4(),
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
	virtual void render(const int& activenode, const UIAssetMaps& assets) = 0;
};
struct TransformComponentUI : public IComponentUI {
	glm::vec3* position;
	glm::quat* orientation;
	glm::vec3* scale;
	bool* inheritPosOnly;

	void render(const int& activenode, const UIAssetMaps& assets) override {
		ImGui::Text("Transform component:");
		bool transChanged = false;
		if (ImGui::DragFloat3("Position", &((*position).x), 0.01f))
			transChanged = true;
        if (ImGui::DragFloat3("Scale", &((*scale).x), 0.01f))
			transChanged = true;
        if (ImGui::DragFloat4("Orientation", &((*orientation).x), 0.01f))
			transChanged = true;
		if (ImGui::Checkbox("Inherit position only", inheritPosOnly))
			transChanged = true;
		if (transChanged)
			InternalEvents::MarkNodeTransformStaleEvent::notify(activenode);
	}
};

struct Model3DComponentUI : public IComponentUI {
	MeshUI* mesh;
	bool* wireframe;
	void render(const int& activenode, const UIAssetMaps& assets) override {
		ImGui::Text("Model3D component:");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Remove 3D model"))
			InputEvents::Remove3DModelComponentEvent::notify(activenode);
		ImGui::PopStyleColor(3);

		const char* meshboxlabel;
		if (mesh == nullptr)
			meshboxlabel = "No mesh";
		else
			meshboxlabel = mesh->name.c_str();
		if (ImGui::BeginCombo("##inspmeshcombo", meshboxlabel)) {
		    if (ImGui::Selectable("No Mesh", false))
		    {
		        InputEvents::SetNodeMeshEvent::notify(activenode, 0);
		    }
		    for (auto const& mesh : assets.meshes) {
		        if (ImGui::Selectable(mesh.second.name.c_str(), false))
		        {
		            InputEvents::SetNodeMeshEvent::notify(activenode, mesh.second.id);
		        }
		    }
		    ImGui::EndCombo();
		}
		// Wireframe mode checkbox
		//ImGui::TableSetColumnIndex(2);
		ImGui::Checkbox("Wireframe", wireframe);

	}
};
struct LightComponentUI : public IComponentUI {
	int lightType;
	glm::vec3* color;
	glm::vec3* direction;
	// Attenuation related
	// ------------------------
	float* constant;
	float* linear;
	float* quadratic;
	void render(const int& activenode, const UIAssetMaps& assets) override {
		ImGui::Text("Light component:");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Remove light"))
			InputEvents::RemoveLightComponentEvent::notify(activenode);
		ImGui::PopStyleColor(3);

		if (lightType == 0) {// pointlight
			ImGui::Text("Type: pointlight");
			ImGui::DragFloat3("Color", &((*color).x), 0.1f);
			ImGui::DragFloat("Constant", &(*constant), 0.1f);
			ImGui::DragFloat("Linear", &(*linear), 0.1f);
			ImGui::DragFloat("Quadratic", &(*quadratic), 0.1f);
		} 
		else if (lightType == 1) { // dirlight
			ImGui::Text("Type: dirlight");
			ImGui::DragFloat3("Color", &((*color).x), 0.1f);
			ImGui::DragFloat3("Direction", &((*direction).x), 0.1f);
		}
	}
};
struct RigidBodyComponentUI : public IComponentUI {

	RigidBody::RigidBodyType t;
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 scale;
	std::unordered_map<RigidBody::RigidBodyType, const char*> typeNames;

	RigidBodyComponentUI():
		typeNames({
			{RigidBody::RigidBodyType::DBOX, "DBox"},
			{RigidBody::RigidBodyType::SBOX, "SBox"},
			{RigidBody::RigidBodyType::DCAPSULE, "DCapsule"},
			{RigidBody::RigidBodyType::SCAPSULE, "SCapsule"},
			{RigidBody::RigidBodyType::TRIANGLEMESH, "Trianglemesh"},
			{RigidBody::RigidBodyType::DSPHERE, "DSphere"},
			{RigidBody::RigidBodyType::SSPHERE, "SSphere"}
			})
	{}
	void render(const int& activenode, const UIAssetMaps& assets) override {
		ImGui::Text("RigidBody component:");
		if (ImGui::BeginCombo("RB type", typeNames.at(t))) {
			for (auto const& type : typeNames) {
				if (ImGui::Selectable(type.second, false))
				{
					InputEvents::AddRigidBodyComponentEvent::notify(activenode, type.first);
				}
			}
			ImGui::EndCombo();
		}
	}
};

struct CameraComponentUI : public IComponentUI {
	float* fov;
	float* zNear;
	float* zFar;
	bool active;
	bool* inheritOrientation;

	void render(const int& activenode, const UIAssetMaps& assets) override {
		ImGui::Text("Camera component:");
		if (ImGui::Button("Remove camera"))
			InputEvents::RemoveCameraEvent::notify(activenode);
		ImGui::DragFloat("FOV", fov, 0.01f);
		ImGui::DragFloat("zNear", zNear, 0.1f);
		ImGui::DragFloat("zFar", zFar, 0.1f);
		ImGui::Checkbox("Inherit orientation", inheritOrientation);
		if (!active) {
			if (ImGui::Button("Make active camera"))
				InputEvents::SceneCameraMakeActiveEvent::notify(activenode);
		}
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
	void sceneTreeRec(EntityUI* node, ImGuiTreeNodeFlags treeflags);
	void createMatTexCombos(MaterialUI* mat, UIAssetMaps* uiAssets);
	void setNoActiveNode();
};

