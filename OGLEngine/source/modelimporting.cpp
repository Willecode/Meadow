#include "modelimporting.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ImageCache.h"
#include "materials/phongmaterial.h"

/*
TODO:
Refactor/clean up this mess of a file
*/
static std::shared_ptr<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, ImageCache& textureCache, std::string directory)
{
	// Implement this comment block if you want to load multiple textures of the same type.
	// Remember to use imagecache.h to prevent loading the same texture multiple times!
	// ----

	/*std::vector<Texture> textures;
	aiString path;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiReturn ret = mat->GetTexture(type, i, &path);
		if (ret != aiReturn_SUCCESS) {
			std::cout << "GetTexture failed" << std::endl;
		}
		else {
			std::cout << path.C_Str() << std::endl;
			textures.push_back(loadTextureFromFile(std::string(path.C_Str()), typeName));
		}
	}*/
	// ----

	aiString path;
	aiReturn ret = mat->GetTexture(type, 0, &path);
	if (ret != aiReturn_SUCCESS) {
		fmt::print("Failed to load material type {} texture in folder: {}/\n", type, directory);
		return nullptr;
	}
	else {
		//textures.push_back(loadTextureFromFile(std::string(path.C_Str()), typeName));
		return std::make_shared<Texture>(
			std::string(directory + "/" + path.C_Str()),
			GL_TEXTURE_2D,
			textureCache);
		
	}
}

static std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, ImageCache& textureCache)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Loop through vertices and copy position, normal, texturecoords
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		glm::vec3 normal = glm::vec3(0.0f);
		if (mesh->HasNormals()) {
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
		}
		glm::vec2 texCoords = glm::vec2(0.0f);
		if (mesh->HasTextureCoords(0)) {
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;

		}
		Vertex vertex(position, normal, texCoords);
		vertices.push_back(vertex);
	}
	// Loop through indices and copy their data
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		// triangulated mesh expected
		unsigned int indexCount = mesh->mFaces[i].mNumIndices;
		if (indexCount == 3) {
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
		else
			std::cout << "model encountered a non-triangulated face with vertex count of: " + indexCount << std::endl;
	}
	// Implement this comment block if you want to load multiple textures of the same type.
	// Remember to use imagecache.h to prevent loading the same texture multiple times!
	// ----
		// Here we can choose what map types to load
	//std::vector<Texture> diffuseMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "diffuse_map");
	//std::vector<Texture> specularMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "specular_map");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// ----
	return std::make_shared<Mesh>(vertices, indices);
}
static void processNode(Object3D* obj, aiNode* node, const aiScene* scene, ImageCache &textureCache, std::string directory)
{
	std::vector<unsigned int> addedMaterials;
	std::vector<unsigned int>::iterator it;
	int materialCount = 0;
	int materialSlot = 0;
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		it = std::find(addedMaterials.begin(), addedMaterials.end(), aimesh->mMaterialIndex);
		if (it == addedMaterials.end()) {
			if (materialCount < obj->MAX_MATERIAL_SLOTS) {
				// Encountered a new material, create new material slot for object
				addedMaterials.push_back(aimesh->mMaterialIndex);
				obj->setMaterial(std::make_shared<PhongMaterial>(
					loadMaterialTextures(scene->mMaterials[aimesh->mMaterialIndex], aiTextureType_DIFFUSE, textureCache, directory),
					loadMaterialTextures(scene->mMaterials[aimesh->mMaterialIndex], aiTextureType_SPECULAR, textureCache, directory)), materialCount);
				materialCount++;
			}
			else {
				fmt::print(
					"ERROR: Tried to import object with more than the max of {} materials",
					obj->MAX_MATERIAL_SLOTS);
			}


		}
		else {
			materialSlot = std::distance(addedMaterials.begin(), it);
		}

		obj->addMesh(processMesh(aimesh, scene, textureCache), materialSlot);
		
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(obj, node->mChildren[i], scene, textureCache, directory);
	}

	
	return;
}

std::shared_ptr<Object3D> ModelImporting::importWavefront(std::string path, ImageCache &textureCache)
{
	auto objptr = std::make_shared<Object3D>();

	ImageCache cache;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	std::string directory = path.substr(0, path.find_last_of('/'));

	processNode(objptr.get(), scene->mRootNode, scene, textureCache, directory);
	// free image cache since all textures of the model are loaded at this point
	cache.freeAllData();

	return objptr;
}
