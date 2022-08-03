#include "modelimporting.h"
#if 1
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/matrix_decompose.hpp>
#include "imageloader.h"
#include "service_locator/locator.h"
#include "resourcemanager.h"
/*
TODO:
Refactor/clean up this mess of a file
*/

glm::mat4 matAssimpToGlm(aiMatrix4x4 aimat) {
	return glm::transpose(glm::mat4(
		aimat[0][0], aimat[0][1], aimat[0][2], aimat[0][3],
		aimat[1][0], aimat[1][1], aimat[1][2], aimat[1][3],
		aimat[2][0], aimat[2][1], aimat[2][2], aimat[2][3],
		aimat[3][0], aimat[3][1], aimat[3][2], aimat[3][3]));
}

static std::unique_ptr<SubMesh> processMesh(aiMesh* mesh, const aiScene* scene)
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
			Locator::getLogger()->getLogger()->info("model encountered a non-triangulated face with vertex count of: {}", indexCount);
	}
	try
	{
		return std::make_unique<SubMesh>(vertices, indices);
	}
	catch (const std::invalid_argument&)
	{
		/*
		* vertices or indices were invalid
		*/
		return nullptr;
	}
}

static void processNode(unsigned int parentNodeId, Scene* scene, aiNode* ainode, const aiScene* aiscene, std::string directory, ResourceManager resourceMan, std::map<int, int> aiMatToMeadowMatId)
{
	unsigned int newNodeId = scene->addNode(parentNodeId);
	SceneNode* newNode = scene->getNode(newNodeId);
	/*
	* Set scenenode name
	*/
	newNode->name = ainode->mName.C_Str();
	/*
	* Set scenenode transform
	*/
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::mat4 transf = glm::scale(matAssimpToGlm(ainode->mTransformation), glm::vec3(1.0));
	glm::decompose(transf, newNode->scale, newNode->orientation, newNode->position, skew, perspective);

	
	/*
	* If there are aimeshes, the scenenode will be given a new Mesh and each aimesh will be linked as it's SubMesh
	*/
	if (ainode->mNumMeshes > 0) {
		std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>(ainode->mName.C_Str());
		// process all the ainode meshes (our submeshes)
		for (unsigned int i = 0; i < ainode->mNumMeshes; i++) {
			aiMesh* aimesh = aiscene->mMeshes[ainode->mMeshes[i]];
			auto newSubmesh = processMesh(aimesh, aiscene);
			if (newSubmesh != nullptr) {
				unsigned int newSubmeshId = resourceMan.storeSubmesh(std::move(newSubmesh));
				SubMesh* newSubmeshPtr = resourceMan.getSubmesh(newSubmeshId);
				newMesh->addSubMesh(resourceMan.getMaterial(aiMatToMeadowMatId.at(aimesh->mMaterialIndex)), newSubmeshPtr);
			}
			else {
				/*
				* Invalid submesh was found within the imported file, mesh will not be stored to resourceman
				*/
				newMesh = nullptr;
				break;
			}
		}
		unsigned int newMeshId = resourceMan.storeMesh(std::move(newMesh));
		newNode->setMesh(resourceMan.getMesh(newMeshId));
	}
	//// then do the same for each of the ainode's children
	for (unsigned int i = 0; i < ainode->mNumChildren; i++)
	{
		processNode(newNodeId, scene, ainode->mChildren[i], aiscene, directory, resourceMan, aiMatToMeadowMatId);
	}
	return;
}

bool processMaterials(std::map<int, int> &aiMatToMeadowMatId, const aiScene* aiscene, std::string directory, ResourceManager resourceMan) {
	/*
	* Get materials
	*/
	ImageLoader imgLoader;
	for (int i = 0; i < aiscene->mNumMaterials; i++) {
		aiMaterial* aimat = aiscene->mMaterials[i];
		Locator::getLogger()->getLogger()->info("Modelimporting: Importing material: {}/", aimat->GetName().C_Str());
		/*
		* Get colors
		*/
		aiColor3D diffuse = aiColor3D(0.f);
		aiColor3D specular = aiColor3D(0.f);
		aimat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimat->Get(AI_MATKEY_COLOR_SPECULAR, specular);

		/*
		* Get diffuse map
		*/
		Renderer::ImageFormat diffuseMapFormat;
		bool foundDiffuseMap = false;
		int widthDiffuseMap, heightDiffuseMap;
		auto vecptrDiffuseMap = std::make_unique<std::vector<unsigned char>>();
		aiString path;
		aiReturn ret = aimat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (ret != aiReturn_SUCCESS) {
			
		}
		else {
			Locator::getLogger()->getLogger()->info("Modelimporting: Material has a {}/", "diffuse map");
			foundDiffuseMap = true;
			/*
			* Load the texture file
			*/
			if (!imgLoader.loadImage(std::string(directory + "/" + path.C_Str()), widthDiffuseMap, heightDiffuseMap, diffuseMapFormat, *vecptrDiffuseMap.get()))
				return false;
		}

		/*
		* Get opacity map
		*/
		Renderer::ImageFormat opacityMapFormat;
		bool foundOpacityMap = false;
		int widthOpacityMap, heightOpacityMap;
		auto vecptrOpacityMap = std::make_unique<std::vector<unsigned char>>();
		ret = aimat->GetTexture(aiTextureType_OPACITY, 0, &path);
		if (ret != aiReturn_SUCCESS) {
			
		}
		else {
			Locator::getLogger()->getLogger()->info("Modelimporting: Material has an {}/", "opacity map");
			foundOpacityMap = true;
			/*
			* Load the texture file
			*/
			if (!imgLoader.loadImage(std::string(directory + "/" + path.C_Str()), widthOpacityMap, heightOpacityMap, opacityMapFormat, *vecptrOpacityMap.get()))
				return false;
		}

		/*
		* Create new Meadow material
		*/
		auto newMat = std::make_unique<Material>(aiscene->mMaterials[i]->GetName().C_Str());
		unsigned int newMatId = resourceMan.storeMaterial(std::move(newMat));
		Material* newMatPtr = resourceMan.getMaterial(newMatId);
		newMatPtr->defaultPhong();

		/*
		* Set colors
		*/
		newMatPtr->setProperty("material.diffuse", glm::vec3(diffuse.r, diffuse.g, diffuse.b), true);
		newMatPtr->setProperty("material.specular", glm::vec3(diffuse.r, diffuse.g, diffuse.b), true);

		/*
		* Store and set textures
		*/
		if (foundDiffuseMap) {
			/*
			* Create new Texture
			*/
			auto newTex = std::make_unique<Texture>(std::move(vecptrDiffuseMap), widthDiffuseMap, heightDiffuseMap, diffuseMapFormat, Renderer::ImageFormat::sRGBA, path.C_Str());
			unsigned int newTexId = resourceMan.storeTexture(std::move(newTex));
			Texture* newTexPtr = resourceMan.getTexture(newTexId);

			newMatPtr->setTexture(newTexPtr, Texture::TextureType::DIFFUSE_MAP);
		}
		if (foundOpacityMap) {
			/*
			* Create new Texture
			*/
			auto newTex = std::make_unique<Texture>(std::move(vecptrOpacityMap), widthOpacityMap, heightOpacityMap, opacityMapFormat, Renderer::ImageFormat::R, path.C_Str());
			unsigned int newTexId = resourceMan.storeTexture(std::move(newTex));
			Texture* newTexPtr = resourceMan.getTexture(newTexId);

			newMatPtr->setTexture(newTexPtr, Texture::TextureType::OPACITY_MAP);
		}

		/*
		* Create mapping from aimaterial id to Meadow material id
		*/
		aiMatToMeadowMatId[i] = newMatId;
	}
	return true;
}

void ModelImporting::objsFromFile(std::string path, Scene* scene, unsigned int parentId)
{
	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode)
	{
		Locator::getLogger()->getLogger()->error("ERROR::ASSIMP::{}", importer.GetErrorString());
		return;
	}
	std::string directory = path.substr(0, path.find_last_of('/'));
	ResourceManager resourceMan = ResourceManager::getInstance();
	std::map<int, int> aiMatToMeadowMatId;
	bool success = processMaterials(aiMatToMeadowMatId, aiscene, directory, resourceMan);
	if (success)
		processNode(parentId, scene, aiscene->mRootNode, aiscene, directory, resourceMan, aiMatToMeadowMatId);
	return;
}

#endif // if 0