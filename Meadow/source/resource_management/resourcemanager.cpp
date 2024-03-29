#include "resourcemanager.h"
#include "input/inputevents.h"
#include "fmt/format.h"
#include "service_locator/loggerlocator.h"
#include "importutility.h"
#include "imageloader.h"
#include "assets/materials/colormaterial.h"
#include "shader/shadermanager.h"

using Sound = Meadow::Sound;

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
	InputEvents::DeleteMaterialEvent::subscribe(
		std::bind(&ResourceManager::deleteMaterial, this, std::placeholders::_1)
	);

	/*
	* Create a fallback material
	*/
	m_fallbackMaterial = std::make_unique<ColorMaterial>("Engine default material");
	m_fallbackMaterial->setId(0);

}

unsigned int ResourceManager::generateUniqueId(Asset::AssetType type)
{
	if (type == Asset::AssetType::TEXTURE || type == Asset::AssetType::CUBEMAP) {
		m_texIdCtr++; // Texture and Cubemap share ID space, since they also share internal OGL id space
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
	else if (type == Asset::AssetType::MESH2D) { 
		m_submeshIdCtr++; // Submesh and mesh2D share ID space, since they also share internal OGL id space
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
	else if (type == Asset::AssetType::SOUND) {
		m_soundIdCtr++;
		return m_soundIdCtr;
	}
}

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager instance;
	return instance;
}

unsigned int ResourceManager::storeTexture(std::unique_ptr<Texture> texture)
{
	if (texture == nullptr)
		return 0;
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
		LoggerLocator::getLogger()->getLogger()->info("Resource manager: couldn't get texture, id not found.\n");
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
	if (smesh == nullptr)
		return 0;
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
		LoggerLocator::getLogger()->getLogger()->info("Resource manager: couldn't get submesh, id not found.\n");
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
	if (mesh == nullptr)
		return 0;
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
		LoggerLocator::getLogger()->getLogger()->info("Resource manager: couldn't get mesh, id not found.\n");
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
	if (material == nullptr)
		return 0;
	unsigned int newId = generateUniqueId(Asset::AssetType::MATERIAL);
	material->setId(newId);
	m_materialMap.insert({ newId, std::move(material) });
	return newId;
}

void ResourceManager::deleteMaterial(unsigned int materialId)
{
	removeMatFromMeshes(materialId);
	m_materialMap.erase(materialId);
	ShaderManager::getInstance().clearDrawUniforms(); // Material might have had unique properties

}

Material* ResourceManager::getMaterial(unsigned int materialId)
{
	if (materialId == 0)
		return nullptr;
	auto it = m_materialMap.find(materialId);
	if (it == m_materialMap.end()) {
		LoggerLocator::getLogger()->getLogger()->info("Resource manager: couldn't get material, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

Material* ResourceManager::getFallbackMaterial()
{
	return m_fallbackMaterial.get();
}

unsigned int ResourceManager::getMaterialId(Material* mat)
{
	for (auto const& m : m_materialMap) {
		if ((*m.second) == *mat) {
			return m.first;
		}
	}
	return 0;
}

const ResourceManager::MaterialMap* ResourceManager::getMaterialMap()
{
	return &m_materialMap;
}

unsigned int ResourceManager::storeMesh2D(std::unique_ptr<Mesh2D> mesh2D)
{
	if (mesh2D == nullptr)
		return 0;
	unsigned int newId = generateUniqueId(Asset::AssetType::MESH2D); 
	mesh2D->setId(newId);
	mesh2D->generateBuffers();
	mesh2D->buffersPushData();
	m_mesh2DMap.insert({ newId, std::move(mesh2D) });
	return newId;
}

Mesh2D* ResourceManager::getMesh2D(unsigned int Mesh2DId)
{
	if (Mesh2DId == 0)
		return nullptr;
	auto it = m_mesh2DMap.find(Mesh2DId);
	if (it == m_mesh2DMap.end()) {
		LoggerLocator::getLogger()->getLogger()->info("Resource manager: couldn't get mesh2D, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

unsigned int ResourceManager::storecubemap(std::unique_ptr<Cubemap> cubemap)
{
	if (cubemap == nullptr)
		return 0;
	unsigned int newId = generateUniqueId(Asset::AssetType::CUBEMAP);
	cubemap->setId(newId);
	cubemap->load();
	m_cubemapMap.insert({ newId, std::move(cubemap) });
	return newId;
}

Cubemap* ResourceManager::getcubemap(unsigned int texId)
{
	if (texId == 0)
		return nullptr;
	auto it = m_cubemapMap.find(texId);
	if (it == m_cubemapMap.end()) {
		LoggerLocator::getLogger()->getLogger()->info("Resource manager: couldn't get cubemap, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

unsigned int ResourceManager::storeSound(std::unique_ptr<Sound> sound)
{
	if (sound == nullptr)
		return 0;
	unsigned int newId = generateUniqueId(Asset::AssetType::SOUND);
	sound->setId(newId);
	m_soundMap.insert({ newId, std::move(sound) });
	return newId;
}

Sound* ResourceManager::getSound(unsigned int id)
{
	if (id == 0)
		return nullptr;
	auto it = m_soundMap.find(id);
	if (it == m_soundMap.end()) {
		LoggerLocator::getLogger()->getLogger()->info("Resource manager: couldn't get sound, id not found.\n");
		return nullptr;
	}
	return it->second.get();
}

const ResourceManager::SoundMap* ResourceManager::getSoundMap()
{
	return &m_soundMap;
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

void ResourceManager::removeMatFromMeshes(unsigned int matId)
{
	for (auto &mesh : m_meshMap) {
		for (auto& submeshes : mesh.second->submeshes) {
			if (submeshes.first->getId() == matId) {
				// Copy the submeshes of found material
				std::vector<SubMesh*> newVec;
				newVec = submeshes.second;

				// Append them to the fallback material submeshes
				mesh.second->submeshes[m_fallbackMaterial.get()].insert(mesh.second->submeshes[m_fallbackMaterial.get()].end(), newVec.begin(), newVec.end());

				//Remove the material from the mesh
				mesh.second->submeshes.erase(submeshes.first);
				break;
			}
				
		}
	}
}
