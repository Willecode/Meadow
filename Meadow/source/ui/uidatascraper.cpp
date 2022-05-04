#include "uidatascraper.h"

UIDataScraper::UIDataScraper()
{
	m_UIAssetMaps[Asset::AssetType::MATERIAL] = {};
	m_UIAssetMaps[Asset::AssetType::MESH] = {};
	m_UIAssetMaps[Asset::AssetType::TEXTURE] = {};
}

void UIDataScraper::update(const Scene* scene)
{
	/*
	* Scrape resource manager
	*/
	auto mats = ResourceManager::getMaterialMap();
	auto meshes = ResourceManager::getMeshMap();
	auto textures =ResourceManager::getTextureMap();
	for (auto const& asset : *mats) {
		AssetUI newAss(asset.second->name, asset.second->getAssetType(), asset.first);
		m_UIAssetMaps.at(Asset::AssetType::MATERIAL).insert({asset.first,newAss});
	}
	for (auto const& asset : *meshes) {
		AssetUI newAss(asset.second->name, asset.second->getAssetType(), asset.first);
		m_UIAssetMaps.at(Asset::AssetType::MESH).insert({ asset.first,newAss });
	}
	for (auto const& asset : *textures) {
		AssetUI newAss(asset.second->name, asset.second->getAssetType(), asset.first);
		m_UIAssetMaps.at(Asset::AssetType::TEXTURE).insert({ asset.first,newAss });
	}

	/*
	* Scrape scene graph
	*/
	SceneNode* root = scene->getNode(0);
	scrapeNode(root, m_uiSceneGraph, 0);
}

SceneNodeUI* UIDataScraper::getUINodeGraph()
{
	return &m_uiSceneGraph;
}

std::map<Asset::AssetType, UI::UIAssetMap>* UIDataScraper::getUIAssets()
{
	return &m_UIAssetMaps;
}

void UIDataScraper::scrapeNode(SceneNode* node, SceneNodeUI& uiNode, int uiElemId)
{
	uiNode = SceneNodeUI();
	uiNode.id = node->id;
	uiNode.uiElemId = uiElemId;
	uiNode.name = &node->name;
	uiNode.scale = &node->scale;
	uiNode.pos = &node->position;
	Mesh* model = node->getModel();
	if (model != nullptr) {
		uiNode.hasGraphics = true;
		SubMesh* m = model->meshes[0]; // Models have multiple meshes, TODO: fix to mesh which contains submesh
		Material* mat = model->material;
		uiNode.mesh = &m_UIAssetMaps.at(Asset::AssetType::MESH).at(m->getId());
		uiNode.material = &m_UIAssetMaps.at(Asset::AssetType::MATERIAL).at(mat->getId());
	}
	else
		uiNode.hasGraphics = false;
	for (auto child : node->children) {
		SceneNodeUI uiChild;
		uiNode.children.push_back(uiChild);
		scrapeNode(child, uiNode.children.back(), uiElemId++);
	}
}
