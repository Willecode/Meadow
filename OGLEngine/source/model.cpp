#include "model.h"
Model::Model(const char* path): cache()
{
	loadModel(path);
}

Model::Model()
{	
	std::vector<Vertex> verts;
	Vertex vert1, vert2, vert3;
	vert1.position = glm::vec3(1.0f, 0.0f, 0.0f);
	vert1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vert1.texCoords = glm::vec2(0.0f, 0.0f);
	verts.push_back(vert1);

	vert2.position = glm::vec3(-1.0f, 0.0f, 0.0f);
	vert2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vert2.texCoords = glm::vec2(0.0f, 0.0f);
	verts.push_back(vert2);

	vert3.position = glm::vec3(0.0f, 1.0f, 0.0f);
	vert3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vert3.texCoords = glm::vec2(0.0f, 0.0f);
	verts.push_back(vert3);

	std::vector<unsigned int> indices = { 0, 1, 2 };
	std::vector<Texture> textures;

	Mesh debugMesh(verts, indices, textures);
	meshes.push_back(debugMesh);
}

void Model::draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader);
	return;
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
	return;
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Loop through vertices and copy position, normal, texturecoords
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
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
		vertex.position = position;
		vertex.normal = normal;
		vertex.texCoords = texCoords;
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

	// Here we can choose what map types to load
	//std::vector<Texture> diffuseMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "diffuse_map");
	std::vector<Texture> diffuseMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "diffuse_map");
	std::vector<Texture> specularMaps = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "specular_map");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
	Mesh retmesh(vertices, indices, textures);
	
	return retmesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
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
	}
	
	return textures;
}

Texture Model::loadTextureFromFile(std::string path, std::string typeName)
{
	unsigned int textureMap;
	glGenTextures(1, &textureMap);
	glBindTexture(GL_TEXTURE_2D, textureMap);
	// -----------------------
	// Set texture parameters here:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// -----------------------
	// image loading
	std::string fullpath = directory + "/" + path;
	CacheData* cacheData = cache.loadImage(fullpath);
	if (cacheData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cacheData->width, cacheData->height, 0, GL_RGB, GL_UNSIGNED_BYTE, cacheData->dataPtr);
		if (typeName == "diffuse_map") {
			unsigned char* p = cacheData->dataPtr;
			for (int i = 0; i < 10; i++) {
				/*std::cout << static_cast<unsigned int>(*p) << std::endl;*/
				p++;
			}
		}
		
		
	}

	return Texture(textureMap, typeName);
}
