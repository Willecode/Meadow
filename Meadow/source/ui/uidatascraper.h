#pragma once
#include "scene/scene.h"
#include "postprocessing.h"
#include "resource_management/resourcemanager.h"
#include "ui/ui.h"
class UIDataScraper
{
private:
	/*
	* UI representation for all stored assets
	*/
	UIAssetMaps m_UIAssetMaps;
	/*
	* UI Scene graph, tree of nodes with child pointers. Properties of the nodes are pointers to UIAssets in m_UIAssetMaps
	*/
	SceneNodeUI m_uiSceneGraph;
	/*
	* Postprocessing flags for UI
	*/
	PostprocessingFlags m_postprocFlags;
public:
	UIDataScraper();
	/*
	* Generate UI representation from back end data
	*/
	void update(const Scene* scene, const PostProcessing* postproc);
	/*
	* Get UI scene graph
	*/
	SceneNodeUI* getUINodeGraph();
	/*
	* Get the list of all stored assets in a UI representation format
	*/
	UIAssetMaps* getUIAssets();
	/*
	* Get postproc flags
	*/
	PostprocessingFlags* getPostprocessingFlags();
private:
	/*
	* param SceneNode* node: Back end node that exists in the scene
	* param SceneNodeUI& uiNode : Front end UINode which needs to represent the backend node
	* param uiElemId: UI id for the UI element displaying the uiNode data. shouldn't be here.
	* 
	* description: Scrape the SceneNode from all relevant data that the uiNode should have, so
	*			   that it can represent the SceneNode in UI
	*/
	void scrapeNode(SceneNode* node, SceneNodeUI& uiNode, int uiElemId);
	MaterialUI constructMaterialUI(Material*);
};

