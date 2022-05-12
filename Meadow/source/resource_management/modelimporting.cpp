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
	return std::make_unique<SubMesh>(vertices, indices);
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
	glm::mat4 transf = matAssimpToGlm(ainode->mTransformation);
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
			unsigned int newSubmeshId = resourceMan.storeSubmesh(std::move(newSubmesh));
			SubMesh* newSubmeshPtr = resourceMan.getSubmesh(newSubmeshId);
			newMesh->addSubMesh(resourceMan.getMaterial(aiMatToMeadowMatId.at(aimesh->mMaterialIndex)), newSubmeshPtr);
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
		aiString path;
		aiReturn ret = aiscene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (ret != aiReturn_SUCCESS) {
			Locator::getLogger()->getLogger()->info("Assimp failed to get texture type {} texture in folder: {}/\n", "diffuse", directory);
		}
		else {
			/*
			* Load the texture file
			*/
			ImageData imgdata = imgLoader.loadImage(std::string(directory + "/" + path.C_Str()));
			if (imgdata.bytes == nullptr)
				return false;
			auto vecptr = std::make_unique<std::vector<unsigned char>>();
			for (int i = 0; i < imgdata.width * imgdata.height * imgdata.nrChannels; i++) {
				vecptr->push_back(imgdata.bytes[i]);
			}
			Renderer::ImageFormat imgForm;
			if (imgdata.nrChannels = 3) {
				imgForm = Renderer::ImageFormat::RGB;
			}
			else if (imgdata.nrChannels = 4) {
				imgForm = Renderer::ImageFormat::RGBA;
			}
			
			/*
			* Create new Texture
			*/
			auto newTex = std::make_unique<Texture>(std::move(vecptr), imgdata.width, imgdata.height, imgForm, path.C_Str());
			unsigned int newTexId = resourceMan.storeTexture(std::move(newTex));
			Texture* newTexPtr = resourceMan.getTexture(newTexId);
			/*
			* Create new Material
			*/
			auto newMat = std::make_unique<Material>(aiscene->mMaterials[i]->GetName().C_Str());
			unsigned int newMatId = resourceMan.storeMaterial(std::move(newMat));
			Material* newMatPtr = resourceMan.getMaterial(newMatId);
			newMatPtr->defaultPhong();
			newMatPtr->setTexture(newTexPtr, Texture::TextureType::DIFFUSE_MAP);

			/*
			* Create mapping from aimaterial id to Meadow material id
			*/
			aiMatToMeadowMatId[i] = newMatId;
		}

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