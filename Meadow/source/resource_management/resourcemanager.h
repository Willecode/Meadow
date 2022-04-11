#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "assets/texture.h"
#include "assets/mesh.h"
#include "shader.h"
#include "assets/material.h"
/*
*  Singleton resource manager. For now meshes and textures stored here will be loaded to graphics memory
*/

class ResourceManager
{
public:
	static ResourceManager& getInstance();
	//static unsigned int loadTexture(std::string filePath, std::string textureName);
	static unsigned int storeTexture(std::unique_ptr<Texture> texture);
	static Texture* getTexture(unsigned int texId);

	static unsigned int storeMesh(std::unique_ptr<Mesh> mesh);
	static Mesh* getMesh(unsigned int meshId);

	static unsigned int storeMaterial(std::unique_ptr<Material> material);
	static Material* getMaterial(unsigned int materialId);
	
private:
	enum class ResourceType {
		TEXTURE = 0,
		MESH,
		SHADER,
		MATERIAL
	};
	inline static unsigned int m_texIdCtr  = 0;
	inline static unsigned int m_meshIdCtr = 0;
	inline static unsigned int m_sdrIdCtr  = 0;
	inline static unsigned int m_matIdCtr  = 0;

	inline static std::unordered_map<unsigned int, std::unique_ptr<Texture>> m_texMap      = {};
	inline static std::unordered_map<unsigned int, std::unique_ptr<Mesh   >> m_meshMap     = {};
	inline static std::unordered_map<unsigned int, std::unique_ptr<Shader >> m_shaderMap   = {};
	inline static std::unordered_map<unsigned int, std::unique_ptr<Material>>m_materialMap = {};

private:
	ResourceManager();
	static unsigned int generateUniqueId(Asset::AssetType type);
};

