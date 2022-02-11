#pragma once
#include <vector>
#include <string>
#include "shader_s.h"
#include "mesh.h"
#include "ImageCache.h"

//-------------
// Asset loader
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
    Model(const char* path);

    //Debug constructor for testing drawing
    Model();

    void draw(Shader& shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    ImageCache cache;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
    Texture loadTextureFromFile(std::string path);
};