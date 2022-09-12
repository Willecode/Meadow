#include "uidatascraper.h"

UIDataScraper::UIDataScraper() : m_UIAssetMaps()
{}

void UIDataScraper::update(const Scene* scene, const PostProcessing* postproc)
{
	//////////////////////////////////
	// ASSETS
	//////////////////////////////////
	/*
	* Clear existing data
	*/
	m_UIAssetMaps.materials.clear();
	m_UIAssetMaps.meshes.clear();
	m_UIAssetMaps.submeshes.clear();
	m_UIAssetMaps.textures.clear();
	/*
	* Fetch resource maps
	*/
	auto mats = ResourceManager::getMaterialMap();
	auto meshes = ResourceManager::getMeshMap();
	auto submeshes = ResourceManager::getSubmeshMap();
	auto textures =ResourceManager::getTextureMap();

	/*
	* Scrape maps for data to populate the UI representation with
	*/
	for (auto const& asset : *mats) {
		if (asset.second->exposeToUI)
			m_UIAssetMaps.materials.insert({ asset.first,constructMaterialUI(asset.second.get())});
	}
	for (auto const& asset : *submeshes) {
		if (asset.second->exposeToUI) {
			SubmeshUI newAss(asset.second->name, asset.first);
			m_UIAssetMaps.submeshes.insert({ asset.first,newAss });
		}
		
	}
	for (auto const& asset : *meshes) {
		if (asset.second->exposeToUI) {
			MeshUI newAss(asset.second->name, asset.first);
			for (auto const& mat : asset.second->submeshes) {
				for (auto const& smesh : mat.second) {
					newAss.submeshes[(smesh->getId())] = mat.first->getId();
				}
			}
			m_UIAssetMaps.meshes.insert({ asset.first,newAss });
		}
	}
	for (auto const& asset : *textures) {
		if (asset.second->exposeToUI) {
			TextureUI newAss(asset.second->name, asset.first);
			m_UIAssetMaps.textures.insert({ asset.first,newAss });
		}
	}

	//////////////////////////////////
	// SCENE GRAPH AND STATE
	//////////////////////////////////
	m_sceneState.activeNode = std::nullopt;
	SceneNode* root = scene->getNode(0);
	scrapeNode(root, m_uiSceneGraph, 0);

	//////////////////////////////////
	// POSTPROCESSING
	//////////////////////////////////
	m_postprocFlags.grayscale = postproc->getGrayscale();
	m_postprocFlags.sharpness = postproc->getSharpen();
	m_postprocFlags.negative = postproc->getNegate();
	m_postprocFlags.MSAA = postproc->getMSAA();
}

SceneNodeUI* UIDataScraper::getUINodeGraph()
{
	return &m_uiSceneGraph;
}

UIAssetMaps* UIDataScraper::getUIAssets()
{
	return &m_UIAssetMaps;
}

PostprocessingFlags* UIDataScraper::getPostprocessingFlags()
{
	return &m_postprocFlags;
}

SceneState* UIDataScraper::getSceneState()
{
	return &m_sceneState;
}

void UIDataScraper::scrapeNode(SceneNode* node, SceneNodeUI& uiNode, int uiElemId)
{
	/*
	* Populate the UINode with data from the SceneNode
	*/
	uiNode = SceneNodeUI();
	uiNode.id = node->id;
	uiNode.uiElemId = uiElemId;
	uiNode.name = &node->name;
	uiNode.scale = &node->scale;
	uiNode.pos = &node->position;
	uiNode.orientationEuler = &node->orientationEuler;
	uiNode.wireframeMode = &node->wireframeMode;
	uiNode.hasLightsource = node->hasLightSource;
	uiNode.selected = node->selected;
	uiNode.active = node->active;

	/*
	* Get mesh data
	*/
	Mesh* mesh = node->getMesh();
	if (mesh == nullptr) {
		uiNode.mesh = nullptr;
	}
	else {
		uiNode.mesh = &m_UIAssetMaps.meshes.at(mesh->getId());
	}

	/*
	* Check if the node is actice, if yes save a copy
	*/
	if (uiNode.active = node->active)
		m_sceneState.activeNode = uiNode;

	/*
	* Now do the same for each of the SceneNodes children
	*/
	for (auto child : node->children) {
		SceneNodeUI uiChild;
		uiNode.children.push_back(uiChild);
		scrapeNode(child, uiNode.children.back(), uiElemId++);
	}
}

MaterialUI UIDataScraper::constructMaterialUI(Material* mat)
{
	MaterialUI newAss(mat->name, mat->getId());
	newAss.propertiesf = mat->getExposedPropertiesf();
	newAss.propertiesv3 = mat->getExposedPropertiesv3();
	auto matTextures = mat->getTextures();
	for (auto const& tex : matTextures) {
		if (tex.second != nullptr) {
			newAss.textures[tex.first] = tex.second->getId();
		}
		else {
			newAss.textures[tex.first] = 0;
		}
	}

	return newAss;
}
