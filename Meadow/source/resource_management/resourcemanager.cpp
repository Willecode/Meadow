#include "resourcemanager.h"
#include "fmt/format.h"

ResourceManager::ResourceManager()
{
}

unsigned int ResourceManager::generateUniqueId(ResourceType type)
{
	if (type == ResourceType::TEXTURE) {
		m_texIdCtr++;
		return m_texIdCtr;
	}
	else if (type == ResourceType::MESH) {
		m_meshIdCtr++;
		return m_meshIdCtr;
	}
	else if (type == ResourceType::SHADER) {
		m_sdrIdCtr++;
		return m_sdrIdCtr;
	}
	else if(type == ResourceType::MATERIAL) {
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
	unsigned int newId = generateUniqueId(ResourceType::TEXTURE);
	texture->id = newId;
	m_texMap.insert({ newId, std::move(texture) });
	return newId;
}

Texture* ResourceManager::getTexture(unsigned int texId)
{
	auto it = m_texMap.find(texId);
	if (it == m_texMap.end()) {
		Locator::getLogger()->getLogger()->info("Resource manager: couldn't get texture, texture id not found.\n");
		return nullptr;
	}
	return it->second.get();
}