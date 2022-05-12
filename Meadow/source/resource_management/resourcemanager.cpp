#include "resourcemanager.h"
#include "input/inputevents.h"
#include "fmt/format.h"

ResourceManager::ResourceManager()
{
	/*
	* Subscribe to events
	*/
	InputEvents::SetSubmeshMaterialEvent::subscribe(
		std::bind(&ResourceManager::setSubmeshMaterialHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	);
	InputEvents::setMaterialTextureEvent::subscribe(
		std::bind(&ResourceManager::setMaterialTextureEventHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	);
	InputEvents::importTextureEvent::subscribe(
		std::bind(&ResourceManager::importTextureHandler, this)
	);
}

unsigned int ResourceManager::generateUniqueId(Asset::AssetType type)
{
	if (type == Asset::AssetType::TEXTURE) {
		m_texIdCtr++;
		return m_texIdCtr;
	}
	else if (type == Asset::AssetType::MESH) {
		m_meshIdCtr++;
		return m_meshIdCtr;
	}
	else if (type == Asset::AssetType::SUBMESH) {
		m_submeshIdCtr++;
		return m_submeshIdCtr;
	}

	else if (type == Asset::AssetType::SHADER) {
		m_sdrIdCtr++;
		return m_sdrIdCtr;
	}
	else if(type == Asset::AssetType::MATERIAL) {
		m_matIdCtr++;
		return m_matIdCtr;
	}
}

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager instance;
	return instance;
}

unsigned int ResourceManager::storeTexture(std::unique_ptr<Texture> texture)
{
	unsigned int newId = generateUniqueId(Asset::AssetType::TEXTURE);
	texture->setId(newId);
	texture->loadToGPU();
	m_texMap.insert({ newId, std::move(texture) });
	return newId;
}

Texture* ResourceManager::getTexture(unsigned int texId)
{
	if (texId == 0) {
		return nullptr;
	}
	auto it = m_texMap.find(texId);
	if (it == m_texMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get texture, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

const ResourceManager::TextureMap* ResourceManager::getTextureMap()
{
	return &m_texMap;
}

unsigned int ResourceManager::storeSubmesh(std::unique_ptr<SubMesh> smesh)
{
	unsigned int newId = generateUniqueId(Asset::AssetType::SUBMESH);
	smesh->setId(newId);
	smesh->generateBuffers();
	smesh->buffersPushData();
	m_submeshMap.insert({ newId, std::move(smesh) });
	return newId;
}

SubMesh* ResourceManager::getSubmesh(unsigned int meshId)
{
	if (meshId == 0)
		return nullptr;
	auto it = m_submeshMap.find(meshId);
	if (it == m_submeshMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get submesh, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

const ResourceManager::SubmeshMap* ResourceManager::getSubmeshMap()
{
	return &m_submeshMap;
}

unsigned int ResourceManager::storeMesh(std::unique_ptr<Mesh> mesh)
{
	unsigned int newId = generateUniqueId(Asset::AssetType::MESH);
	mesh->setId(newId);
	m_meshMap.insert({ newId, std::move(mesh) });
	return newId;
}

Mesh* ResourceManager::getMesh(unsigned int meshId)
{
	if (meshId == 0)
		return nullptr;
	auto it = m_meshMap.find(meshId);
	if (it == m_meshMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get mesh, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

const ResourceManager::MeshMap* ResourceManager::getMeshMap()
{
	return &m_meshMap;
}

unsigned int ResourceManager::storeMaterial(std::unique_ptr<Material> material)
{
	unsigned int newId = generateUniqueId(Asset::AssetType::MATERIAL);
	material->setId(newId);
	m_materialMap.insert({ newId, std::move(material) });
	return newId;
}

Material* ResourceManager::getMaterial(unsigned int materialId)
{
	if (materialId == 0)
		return nullptr;
	auto it = m_materialMap.find(materialId);
	if (it == m_materialMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get material, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

const ResourceManager::MaterialMap* ResourceManager::getMaterialMap()
{
	return &m_materialMap;
}

void ResourceManager::setSubmeshMaterialHandler(unsigned int meshid, unsigned int submeshid, unsigned int materialid)
{
	auto meshit = m_meshMap.find(meshid);
	auto smeshit = m_submeshMap.find(submeshid);
	auto matit = m_materialMap.find(materialid);
	if (   meshit     != m_meshMap.end()
		&& smeshit != m_submeshMap.end()
		&& matit   != m_materialMap.end()) {
		meshit->second->setSubMeshMaterial(matit->second.get(), smeshit->second.get());
	}
}

void ResourceManager::setMaterialTextureEventHandler(unsigned int materialid, unsigned int textureid, Texture::TextureType textureType)
{

	Material* mat = getMaterial(materialid);
	Texture* tex = getTexture(textureid);
	if (mat != nullptr) {
		mat->setTexture(tex, textureType);
	}
}

void ResourceManager::importTextureHandler()
{

}


