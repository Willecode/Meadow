#include "uidatascraper.h"

UIDataScraper::UIDataScraper() : m_UIAssetMaps()
{}

void UIDataScraper::update(const Scene* scene, const PostProcessing* postproc)
{
	//////////////////////////////////
	// SCENE RELATED
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
		/*MaterialUI newAss(asset.second->name, asset.first);*/
		//m_UIAssetMaps.materials.insert({ asset.first,newAss });
		m_UIAssetMaps.materials.insert({ asset.first,constructMaterialUI(asset.second.get())});
	}
	for (auto const& asset : *submeshes) {
		SubmeshUI newAss(asset.second->name, asset.first);
		m_UIAssetMaps.submeshes.insert({ asset.first,newAss });
	}
	for (auto const& asset : *meshes) {
		MeshUI newAss(asset.second->name, asset.first);
		for (auto const& mat : asset.second->submeshes) {
			for (auto const& smesh : mat.second) {
				newAss.submeshes[(smesh->getId())] = mat.first->getId();
			}
		}
		m_UIAssetMaps.meshes.insert({ asset.first,newAss });
	}
	for (auto const& asset : *textures) {
		TextureUI newAss(asset.second->name, asset.first);
		m_UIAssetMaps.textures.insert({ asset.first,newAss });
	}

	/*
	* Scrape scene graph
	*/
	SceneNode* root = scene->getNode(0);
	scrapeNode(root, m_uiSceneGraph, 0);

	//////////////////////////////////
	// POSTPROCESSING
	//////////////////////////////////
	m_postprocFlags.grayscale = postproc->getGrayscale();
	m_postprocFlags.sharpness = postproc->getSharpen();
	m_postprocFlags.negative = postproc->getNegate();
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
	Texture* diff = mat->getTexture(Texture::TextureType::DIFFUSE_MAP);
	Texture* spec = mat->getTexture(Texture::TextureType::SPECULAR_MAP);
	Texture* opac = mat->getTexture(Texture::TextureType::OPACITY_MAP);
	if (diff != nullptr)
		newAss.diffuseMap = diff->getId();
	if (spec != nullptr)
		newAss.specularMap = spec->getId();
	if (opac != nullptr)
		newAss.opacityMap = opac->getId();

	return newAss;
}
