#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "assets/texture.h"
#include "assets/mesh.h"
#include "shader.h"
#include "assets/material.h"
#include "ui/ui.h"
/*
*  Singleton resource manager. For now meshes and textures stored here will be loaded to graphics memory
*/

class ResourceManager
{
public:
	typedef std::unordered_map<unsigned int, std::unique_ptr<Texture>>  TextureMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Mesh   >>  MeshMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Shader >>  ShaderMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Material>> MaterialMap;

public:
	static ResourceManager& getInstance();
	//static unsigned int loadTexture(std::string filePath, std::string textureName);
	static unsigned int storeTexture(std::unique_ptr<Texture> texture);
	static Texture* getTexture(unsigned int texId);
	static const TextureMap* getTextureMap();

	static unsigned int storeMesh(std::unique_ptr<Mesh> mesh);
	static Mesh* getMesh(unsigned int meshId);
	static const MeshMap* getMeshMap();

	static unsigned int storeMaterial(std::unique_ptr<Material> material);
	static Material* getMaterial(unsigned int materialId);
	static const MaterialMap* getMaterialMap();
	
	//static void scrapeData(std::vector<AssetUI> &assets);
private:
	/*enum class ResourceType {
		TEXTURE = 0,
		MESH,
		SHADER,
		MATERIAL
	};*/
	inline static unsigned int m_texIdCtr  = 0;
	inline static unsigned int m_meshIdCtr = 0;
	inline static unsigned int m_sdrIdCtr  = 0;
	inline static unsigned int m_matIdCtr  = 0;

	inline static TextureMap  m_texMap      = {};
	inline static MeshMap     m_meshMap     = {};
	inline static ShaderMap   m_shaderMap   = {};
	inline static MaterialMap m_materialMap = {};

private:
	ResourceManager();
	static unsigned int generateUniqueId(Asset::AssetType type);
};

