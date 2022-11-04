#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "assets/texture.h"
#include "assets/submesh.h"
#include "assets/mesh.h"
#include "shader/shader.h"
#include "assets/materials/material.h"
#include "ui/ui.h"
#include "assets/mesh2d.h"
#include "assets/cubemap.h"
#include "assets/sound.h"
/*
*  Singleton resource manager. For now meshes and textures stored here get loaded to graphics memory
*/

class ResourceManager
{
public:
	typedef std::unordered_map<unsigned int, std::unique_ptr<Texture>>  TextureMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Cubemap>>  CubemapMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Mesh>>		MeshMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Mesh2D>>	Mesh2DMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<SubMesh>>  SubmeshMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Shader >>  ShaderMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Material>> MaterialMap;
	typedef std::unordered_map<unsigned int, std::unique_ptr<Meadow::Sound>>    SoundMap;

public:
	static ResourceManager& getInstance();
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	//static unsigned int loadTexture(std::string filePath, std::string textureName);
	static unsigned int storeTexture(std::unique_ptr<Texture> texture);
	static Texture* getTexture(unsigned int texId);
	static const TextureMap* getTextureMap();

	static unsigned int storeSubmesh(std::unique_ptr<SubMesh> smesh);
	static SubMesh* getSubmesh(unsigned int smeshId);
	static const SubmeshMap* getSubmeshMap();
	
	static unsigned int storeMesh(std::unique_ptr<Mesh> mesh);
	static Mesh* getMesh(unsigned int meshId);
	static const MeshMap* getMeshMap();

	static unsigned int storeMaterial(std::unique_ptr<Material> material);
	void deleteMaterial(unsigned int materialId);
	static Material* getMaterial(unsigned int materialId);
	Material* getFallbackMaterial();
	static unsigned int getMaterialId(Material* mat);
	static const MaterialMap* getMaterialMap();

	static unsigned int storeMesh2D(std::unique_ptr<Mesh2D> Mesh2D);
	static Mesh2D* getMesh2D(unsigned int Mesh2DId);
	static const Mesh2DMap* getMesh2DMap();

	static unsigned int storecubemap(std::unique_ptr<Cubemap> cubemap);
	static Cubemap* getcubemap(unsigned int texId);
	static const CubemapMap* getcubemapMap();

	static unsigned int storeSound(std::unique_ptr<Meadow::Sound> sound);
	static Meadow::Sound* getSound(unsigned int id);
	static const SoundMap* getSoundMap();

	/*
	* Event handlers
	*/
	void setSubmeshMaterialHandler(unsigned int meshid, unsigned int submeshid, unsigned int materialid);
	void setMaterialTextureEventHandler(unsigned int materialid, unsigned int textureid, Texture::TextureType textureType);
	
	void removeMatFromMeshes(unsigned int mat);

private:

	/*
	* Counters for keeping track of used id's, 0 is invalid id
	*/
	inline static unsigned int m_texIdCtr     = 0;
	inline static unsigned int m_meshIdCtr    = 0;
	inline static unsigned int m_submeshIdCtr = 0;
	inline static unsigned int m_sdrIdCtr     = 0;
	inline static unsigned int m_matIdCtr     = 0;
	inline static unsigned int m_soundIdCtr     = 0;

	/*
	* Storages for assets.
	*/
	inline static TextureMap  m_texMap      = {};
	inline static CubemapMap  m_cubemapMap  = {};
	inline static MeshMap     m_meshMap     = {};
	inline static Mesh2DMap   m_mesh2DMap   = {};
	inline static SubmeshMap  m_submeshMap  = {};
	inline static ShaderMap   m_shaderMap   = {};
	inline static MaterialMap m_materialMap = {};
	inline static SoundMap    m_soundMap    = {};

	/*
	* Fallback assets
	*/
	std::unique_ptr<Material> m_fallbackMaterial;

private:
	ResourceManager();
	static unsigned int generateUniqueId(Asset::AssetType type);
};

