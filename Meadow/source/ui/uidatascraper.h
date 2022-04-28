#pragma once
#include "scene/scene.h"
#include "resource_management/resourcemanager.h"
#include "ui/ui.h"
class UIDataScraper
{
private:
	std::map<Asset::AssetType, UI::UIAssetMap> m_UIAssetMaps;
	SceneNodeUI m_uiSceneGraph;
public:
	UIDataScraper();
	/*
	* Generate UI representation from back end data
	*/
	void update(const Scene* scene);
	SceneNodeUI* getUINodeGraph();
	std::map<Asset::AssetType, UI::UIAssetMap>* getUIAssets();
private:
	void scrapeNode(SceneNode* node, SceneNodeUI& uiNode, int uiElemId);

};

