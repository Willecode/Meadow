#pragma once
#include "material.h"
#include "shader/shadermanager.h"
class PBRMaterial : public Material
{
public:
    PBRMaterial(std::string name = "PBR material");
    void setDefault();
    void setTexture(Texture* tex, Texture::TextureType type) override;
    std::unordered_map<Texture::TextureType, Texture*> getTextures() override;
    ShaderManager::ShaderType getShaderType() override;
private:
    static std::array<Texture::TextureType, 5> textureInputs;
    static ShaderManager::ShaderType shaderType;
};

