#include "uidatascraper.h"
#include "ecs/components/transform.h"
#include "ecs/components/light.h"
#include "ecs/components/model3d.h"
#include "ecs/components/camera.h"
UIDataScraper::UIDataScraper() : m_UIAssetMaps()
{}

void UIDataScraper::update(const SceneGraph::Node& graph, const PostProcessing* postproc, const ECSCoordinator& ecs)
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
	//m_sceneState.activeNode = std::nullopt;
	//for (auto const& child : graph.children)
	scrapeSceneGraph(graph, m_sceneRoot);

	//////////////////////////////////
	// GET COMPONENTS
	//////////////////////////////////
	m_componentMap.clear();
	for (auto const& child : graph.children)
		constructComponentMap(child, ecs);
	

	//////////////////////////////////
	// POSTPROCESSING
	//////////////////////////////////
	m_postprocFlags.grayscale = postproc->getGrayscale();
	m_postprocFlags.sharpness = postproc->getSharpen();
	m_postprocFlags.negative = postproc->getNegate();
	m_postprocFlags.MSAA = postproc->getMSAA();
}

EntityUI* UIDataScraper::getSceneGraph()
{
	return &m_sceneRoot;
}

UIAssetMaps* UIDataScraper::getUIAssets()
{
	return &m_UIAssetMaps;
}

PostprocessingFlags* UIDataScraper::getPostprocessingFlags()
{
	return &m_postprocFlags;
}

ComponentMapUI* UIDataScraper::getComponentMap()
{
	return &m_componentMap;
}

//void UIDataScraper::scrapeNode(const SceneGraph& node, EntityUI& uiNode, int uiElemId)
//{
//	/*
//	* Populate the UINode with data from the SceneNode
//	*/
//	uiNode = EntityUI();
//	uiNode.id = node->id;
//	uiNode.uiElemId = uiElemId;
//	uiNode.name = &node->name;
//	uiNode.scale = &node->scale;
//	uiNode.pos = &node->position;
//	uiNode.orientationEuler = &node->orientationEuler;
//	uiNode.wireframeMode = &node->wireframeMode;
//	uiNode.hasLightsource = node->hasLightSource;
//	uiNode.selected = node->selected;
//	uiNode.active = node->active;
//
//	/*
//	* Get mesh data
//	*/
//	Mesh* mesh = node->getMesh();
//	if (mesh == nullptr) {
//		uiNode.mesh = nullptr;
//	}
//	else {
//		uiNode.mesh = &m_UIAssetMaps.meshes.at(mesh->getId());
//	}
//
//	/*
//	* Check if the node is actice, if yes save a copy
//	*/
//	if (uiNode.active = node->active)
//		m_sceneState.activeNode = uiNode;
//
//	/*
//	* Now do the same for each of the SceneNodes children
//	*/
//	for (auto child : node->children) {
//		EntityUI uiChild;
//		uiNode.children.push_back(uiChild);
//		scrapeNode(child, uiNode.children.back(), uiElemId++);
//	}
//}

void UIDataScraper::scrapeSceneGraph(const SceneGraph::Node& node, EntityUI& uiNode)
{
	uiNode = EntityUI();
	uiNode.id = node.entity;
	for (auto const& child : node.children) {
		EntityUI uiChild;
		uiNode.children.push_back(uiChild);
		scrapeSceneGraph(child, uiNode.children.back());
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

void UIDataScraper::constructComponentMap(const SceneGraph::Node& node, const ECSCoordinator& ecs)
{
	Entity ent = node.entity;
	Signature sign = ecs.getEntitySignature(ent);
	auto transType = ecs.getComponentType<Transform>();
	auto modelType = ecs.getComponentType<Model3D>();

	if (sign.test(transType)) {
		auto trans = ecs.getComponent<Transform>(ent);
		auto transUI = std::make_unique<TransformComponentUI>();
		transUI->position = trans.position;
		transUI->orientation = trans.orientationEuler;
		transUI->scale = trans.scale;

		m_componentMap[ent].push_back(std::move(transUI));
	}
	if (sign.test(modelType)) {
		auto model = ecs.getComponent<Model3D>(ent);
		auto modelUI = std::make_unique<Model3DComponentUI>();

		m_componentMap[ent].push_back(std::move(modelUI));
	}
}
