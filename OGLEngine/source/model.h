#pragma once
#include <vector>
#include <string>
#include "shader_s.h"
#include "mesh.h"

//-------------
// Asset loader
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Mesh define for debugging
struct Vertex;

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

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};