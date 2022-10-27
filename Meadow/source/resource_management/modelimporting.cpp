#include "modelimporting.h"
#if 1
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <assimp/pbrmaterial.h>
#include <assimp/material.h>
#include <assimp/GltfMaterial.h>
#include <glm/gtx/matrix_decompose.hpp>
#include "imageloader.h"
#include "service_locator/loggerlocator.h"
#include "resourcemanager.h"
#include "assets/materials/PBRMaterial.h"
#include "utils/imagemanipulation.h"

#include "ecs/components/transform.h"
#include "ecs/components/model3d.h"
#include "ecs/components/light.h"

#include <stdexcept>
#include <unordered_map>
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
		position.x = mesh->mVertices[i].x; // position
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec3 tangent = glm::vec3(0.0f);
		glm::vec3 bitangent = glm::vec3(0.0f);
		if (mesh->HasNormals()) {
			normal.x = mesh->mNormals[i].x; // normals
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;

			tangent.x = mesh->mTangents[i].x; // tangents
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;

			bitangent.x = mesh->mBitangents[i].x; // bitangents
			bitangent.y = mesh->mBitangents[i].y;
			bitangent.z = mesh->mBitangents[i].z;

			
		}
		glm::vec2 texCoords = glm::vec2(0.0f);
		if (mesh->HasTextureCoords(0)) {
			texCoords.x = mesh->mTextureCoords[0][i].x; // tex coords
			texCoords.y = mesh->mTextureCoords[0][i].y;

		}
		Vertex vertex(position, normal, texCoords, tangent, bitangent);
		vertices.push_back(vertex);
	}
	// Loop through indices and copy their data
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		// triangulated mesh expected
		unsigned int indexCount = mesh->mFaces[i].mNumIndices;
		if (indexCount == 3) {
			indices.push_back(mesh->mFaces[i].mIndices[0]);	// indices
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
		else
			LoggerLocator::getLogger()->getLogger()->info("model encountered a non-triangulated face with vertex count of: {}", indexCount);
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

static void processNode(unsigned int parentNodeId, ECSCoordinator* ecs, aiNode* ainode, const aiScene* aiscene,
	std::string directory, ResourceManager& resourceMan, std::map<int, int> aiMatToMeadowMatId, std::unordered_map<std::string, Entity> &entMap,
	bool fbx, int hierarchyIndex)
{
	Entity ent = ecs->createEntityNamed(ainode->mName.C_Str());;
	InputEvents::SetNodeParentEvent::notify(ent, parentNodeId);
	entMap.insert({ ainode->mName.C_Str(), ent });
	/*
	* Set scenenode name
	*/
	//newNode->name = ainode->mName.C_Str();
	/*
	* Set scenenode transform
	*/
	auto& t = ecs->getComponent<Transform>(ent);
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::mat4 transf = matAssimpToGlm(ainode->mTransformation);
	
	glm::decompose(transf, t.scale, t.orientation, t.position, skew, perspective);
	if (fbx) {
		if (hierarchyIndex == 1) {
			t.scale *= 0.01;
			t.position *= 0.01;
		}
		hierarchyIndex++;
	}
	
	/*
	* If there are aimeshes, the scenenode will be given a new Mesh with x submeshes, x being the number of aimeshes
	*/
	if (ainode->mNumMeshes > 0) {
		Model3D model;
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
		model.mesh = (resourceMan.getMesh(newMeshId));
		ecs->addComponent(ent, model);
	}
	//// then do the same for each of the ainode's children
	for (unsigned int i = 0; i < ainode->mNumChildren; i++)
	{
		processNode(ent, ecs, ainode->mChildren[i], aiscene, directory, resourceMan, aiMatToMeadowMatId, entMap, fbx, hierarchyIndex);
	}
	return;
}

Texture* importTexture(aiTextureType type, aiMaterial* aimat,  const std::string& directory, ResourceManager& resourceMan, Renderer::ImageFormat engineFormat) {
	ImageLoader imgLoader;
	Renderer::ImageFormat textureFormat;
	int texWidth, texHeight;
	auto vecptrDiffuseMap = std::make_unique<std::vector<unsigned char>>();
	aiString path;
	aiReturn ret;
	bool combinedMetRough = false;
	if (type == aiTextureType_DIFFUSE_ROUGHNESS) {
		ret = aimat->GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &path);
		if (ret != aiReturn_SUCCESS) { 
			ret = aimat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &path); // Try for combined roughness metallic
			combinedMetRough = true;
		}
	}
	else if (type == aiTextureType_METALNESS){
		ret = aimat->GetTexture(AI_MATKEY_METALLIC_TEXTURE, &path);
		if (ret != aiReturn_SUCCESS) {
			ret = aimat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &path); // Try for combined roughness metallic
			combinedMetRough = true;
		}
	}

	else if (type == aiTextureType_DIFFUSE) {
		ret = aimat->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &path);
		if (ret != aiReturn_SUCCESS)
			ret = aimat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	}
	else if (type == aiTextureType_OPACITY)
		ret = aimat->GetTexture(AI_MATKEY_TRANSMISSION_TEXTURE, &path);
	else if (type == aiTextureType_AMBIENT_OCCLUSION)
		ret = aimat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &path); // Not sure if this works
	else
		ret = aimat->GetTexture(type, 0, &path);

	if (ret != aiReturn_SUCCESS) {
		return nullptr;
	}
	else {
		LoggerLocator::getLogger()->getLogger()->info("Modelimporting: Material has a texture:{}", path.C_Str());
		/*
		* Load the texture file
		*/
		if (!imgLoader.loadImage(std::string(directory + "/" + path.C_Str()), texWidth, texHeight, textureFormat, *vecptrDiffuseMap.get()))
			return nullptr;
	}

	// In case of joined roughness metallic, extract the relevant channel
	if (combinedMetRough) {
		if (type == aiTextureType_METALNESS) {
			int texW = 0;
			int texH = 0;
			ImageManipulation::extractChannel(&(*vecptrDiffuseMap)[0], texWidth, texHeight, *vecptrDiffuseMap.get(), texW, texH, textureFormat, ImageManipulation::IMGChannel::B);
			texWidth = texW;
			texH = texHeight;
		}
		else if (type == aiTextureType_DIFFUSE_ROUGHNESS) {
			int texW = 0;
			int texH = 0;
			ImageManipulation::extractChannel(&(*vecptrDiffuseMap)[0], texWidth, texHeight, *vecptrDiffuseMap.get(), texW, texH, textureFormat, ImageManipulation::IMGChannel::G);
			texWidth = texW;
			texH = texHeight;
		}
	}

	/*
	* Create new Texture
	*/
	auto newTex = std::make_unique<Texture>(std::move(vecptrDiffuseMap), texWidth, texHeight, textureFormat, engineFormat, path.C_Str());
	unsigned int newTexId = resourceMan.storeTexture(std::move(newTex));
	return resourceMan.getTexture(newTexId);
}

bool processMaterials(std::map<int, int> &aiMatToMeadowMatId, const aiScene* aiscene, std::string directory, ResourceManager& resourceMan) {
	/*
	* Get materials
	*/
	ImageLoader imgLoader;
	for (int i = 0; i < aiscene->mNumMaterials; i++) {
		aiMaterial* aimat = aiscene->mMaterials[i];
		// Debug
		int normCount = aimat->GetTextureCount(aiTextureType_NORMALS);
		int roughdiffcount1 = aimat->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
		int roughdiffcount2 = aimat->GetTextureCount(aiTextureType_UNKNOWN);
		// -----
		LoggerLocator::getLogger()->getLogger()->info("Modelimporting: Importing material: {}", aimat->GetName().C_Str());
		/*
		* Get colors
		*/
		aiColor3D diffuse = aiColor3D(0.f);
		aiColor3D specular = aiColor3D(0.f);
		aimat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimat->Get(AI_MATKEY_COLOR_SPECULAR, specular);

		/*
		* Create new Meadow material
		*/
		auto newMat = std::make_unique<PBRMaterial>(aiscene->mMaterials[i]->GetName().C_Str());
		unsigned int newMatId = resourceMan.storeMaterial(std::move(newMat));
		Material* newMatPtr = resourceMan.getMaterial(newMatId);
		//newMatPtr->defaultPhong();

		/*
		* Set colors
		*/
		newMatPtr->setProperty("material.diffuse", glm::vec3(diffuse.r, diffuse.g, diffuse.b), true);
		newMatPtr->setProperty("material.specular", glm::vec3(diffuse.r, diffuse.g, diffuse.b), true);

		/*
		* Import diffuse map
		*/
		{
			Texture* diffMap = importTexture(aiTextureType_DIFFUSE, aimat, directory, resourceMan, Renderer::ImageFormat::sRGBA);
			if (diffMap != nullptr) {
				newMatPtr->setTexture(diffMap, Texture::TextureType::ALBEDO_MAP);
			}
		}
		
		/*
		* Import opac map
		*/
		{
			Texture* opacMap = importTexture(aiTextureType_OPACITY, aimat, directory, resourceMan, Renderer::ImageFormat::R);
			if (opacMap != nullptr) {
				newMatPtr->setTexture(opacMap, Texture::TextureType::OPACITY_MAP);

			}
		}

		/*
		* Import normal map
		*/
		{
			Texture* normalMap = importTexture(aiTextureType_NORMALS, aimat, directory, resourceMan, Renderer::ImageFormat::RGB);
			if (normalMap != nullptr) {
				newMatPtr->setTexture(normalMap, Texture::TextureType::NORMAL_MAP);
			}
		}

		/*
		* Import rough map
		*/
		{
			Texture* roughMap = importTexture(aiTextureType_DIFFUSE_ROUGHNESS, aimat, directory, resourceMan, Renderer::ImageFormat::R);
			if (roughMap != nullptr) {
				newMatPtr->setTexture(roughMap, Texture::TextureType::ROUGHNESS_MAP);
			}
		}
		/*
		* Import metal map
		*/
		{
			Texture* metalMap = importTexture(aiTextureType_METALNESS, aimat, directory, resourceMan, Renderer::ImageFormat::R);
			if (metalMap != nullptr) {
				newMatPtr->setTexture(metalMap, Texture::TextureType::METALLIC_MAP);
			}
		}


		/*
		* Import ao map
		*/
		{
			Texture* aoMap = importTexture(aiTextureType_AMBIENT_OCCLUSION, aimat, directory, resourceMan, Renderer::ImageFormat::R);
			if (aoMap != nullptr) {
				LoggerLocator::getLogger()->getLogger()->info("Modelimporting: Found ao map");
				newMatPtr->setTexture(aoMap, Texture::TextureType::AO_MAP);
			}
		}
		/*
		* Create mapping from aimaterial id to Meadow material id
		*/
		aiMatToMeadowMatId[i] = newMatId;
	}
	return true;
}

void importLights(const aiScene* scene, std::unordered_map<std::string, Entity> entMap, ECSCoordinator* ecs) {
	for (unsigned int i = 0; i < scene->mNumLights; i++) {
		Light l;
		l.color.r = scene->mLights[i]->mColorDiffuse.r;
		l.color.g = scene->mLights[i]->mColorDiffuse.g;
		l.color.b = scene->mLights[i]->mColorDiffuse.b;
		auto it = entMap.find(scene->mLights[i]->mName.C_Str());
		if (it == entMap.end()) {
			LoggerLocator::getLogger()->getLogger()->error("Problem importing lights from file");
			break;
		}
		//ecs->addComponent<Light>(entMap[scene->mLights[i]->mName.C_Str()], l);
		ecs->addComponent<Light>((* it).second, l);
	}
}
void ModelImporting::objsFromFile(std::string path, ECSCoordinator* ecs)
{
	Assimp::Importer importer;

	bool fbx = false;
	std::size_t found = path.find(".fbx");
	if (found != std::string::npos)
		fbx = true;

	/*
	* When importing, triangulate, flip UVs and calculate tangent and bitangent vectors
	*/
	const aiScene* aiscene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	/*
	* Check for failures
	*/
	if (!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode)
	{
		LoggerLocator::getLogger()->getLogger()->error("ERROR::ASSIMP::{}", importer.GetErrorString());
		return;
	}

	/*
	* Find file directory
	*/
	std::string directory = path.substr(0, path.find_last_of('/'));
	
	/*
	* Import materials from the file
	*/
	ResourceManager& resourceMan = ResourceManager::getInstance();
	std::map<int, int> aiMatToMeadowMatId;
	bool success = processMaterials(aiMatToMeadowMatId, aiscene, directory, resourceMan);

	// Mapping from aiNode name -> Entity
	std::unordered_map<std::string, Entity> entMap;

	/*
	* If materials successfully imported, import other things
	*/
	if (success) {
		processNode(0, ecs, aiscene->mRootNode, aiscene, directory, resourceMan, aiMatToMeadowMatId, entMap, fbx, 0);
		if (aiscene->HasLights())
			importLights(aiscene, entMap, ecs);
	}

	return;
}

#endif // if 0