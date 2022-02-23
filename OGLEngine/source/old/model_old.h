#pragma once
#include <vector>
#include <string>
#include "shader_s.h"
#include "mesh.h"
#include "ImageCache.h"
#include "object3d.h"

//-------------
// Asset loader
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/* Loads a 3D model at the specified path into it's own data structures.
   The model can then be drawn using the specified shader.
*/
class Model: public Object3D
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
    Texture loadTextureFromFile(std::string path, std::string typeName);
};