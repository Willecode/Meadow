#include "resourcemanager.h"
#include "fmt/format.h"

ResourceManager::ResourceManager()
{
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
	auto it = m_texMap.find(texId);
	if (it == m_texMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get texture, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

unsigned int ResourceManager::storeMesh(std::unique_ptr<Mesh> mesh)
{
	unsigned int newId = generateUniqueId(Asset::AssetType::MESH);
	mesh->setId(newId);
	mesh->generateBuffers();
	mesh->buffersPushData();
	m_meshMap.insert({ newId, std::move(mesh) });
	return newId;
}

Mesh* ResourceManager::getMesh(unsigned int meshId)
{
	auto it = m_meshMap.find(meshId);
	if (it == m_meshMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get mesh, id not found.\n");
		return nullptr;
	}
	return it->second.get();
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
	auto it = m_materialMap.find(materialId);
	if (it == m_materialMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get material, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

