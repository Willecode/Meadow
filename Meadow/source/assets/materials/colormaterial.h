#pragma once
#include "assets/materials/material.h"
class ColorMaterial : public Material
{
public:
    ColorMaterial(std::string name = "Color material");
    void setDefault();
    void setTexture(Texture* tex, Texture::TextureType type) override;
    std::unordered_map<Texture::TextureType, Texture*> getTextures() override;
    ShaderManager::ShaderType getShaderType() override;
private:
    static std::array<Texture::TextureType, 6> textureInputs;
    static ShaderManager::ShaderType shaderType;
};

