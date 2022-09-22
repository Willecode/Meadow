
#pragma once
#include "postprocessing/postprocessing.h"
#include "resource_management/resourcemanager.h"
#include "ui/ui.h"
#include "ecs/systems/scenegraphsystem/scenegraph.h"
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
	EntityUI m_sceneRoot;
	/*
	* Postprocessing flags for UI
	*/
	PostprocessingFlags m_postprocFlags;
	/*
	* Scene's state
	*/
	SceneState m_sceneState;
public:
	UIDataScraper();
	/*
	* Generate UI representation from back end data
	*/
	void update(const SceneGraph::Node& graph, const PostProcessing* postproc);
	/*
	* Get UI scene graph
	*/
	EntityUI* getSceneGraph();
	/*
	* Get the list of all stored assets in a UI representation format
	*/
	UIAssetMaps* getUIAssets();
	/*
	* Get postproc flags
	*/
	PostprocessingFlags* getPostprocessingFlags();
	/*
	* Get scene state
	*/
	SceneState* getSceneState();
private:
	/*
	* param SceneNode* node: Back end node that exists in the scene
	* param SceneNodeUI& uiNode : Front end UINode which needs to represent the backend node
	* param uiElemId: UI id for the UI element displaying the uiNode data. shouldn't be here.
	* 
	* description: Scrape the SceneNode from all relevant data that the uiNode should have, so
	*			   that it can represent the SceneNode in UI
	*/
	//void scrapeNode(const SceneGraph& node, EntityUI& uiNode, int uiElemId);
	void scrapeSceneGraph(const SceneGraph::Node& node, EntityUI& uiNode);
	MaterialUI constructMaterialUI(Material*);
};
