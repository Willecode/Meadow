#include "PBRMaterial.h"

std::array<Texture::TextureType, 6> PBRMaterial::textureInputs = {
	Texture::TextureType::ALBEDO_MAP,
	Texture::TextureType::ROUGHNESS_MAP,
	Texture::TextureType::METALLIC_MAP,
	Texture::TextureType::NORMAL_MAP,
	Texture::TextureType::AO_MAP,
	Texture::TextureType::OPACITY_MAP
};
ShaderManager::ShaderType PBRMaterial::shaderType = ShaderManager::ShaderType::PBR;

PBRMaterial::PBRMaterial(std::string name): Material(name)
{
	setDefault();
}

void PBRMaterial::setDefault()
{
	clearProperties();
	setProperty("material.albedo", MaterialConstants::DEFAULT_COLOR, true);
	setProperty("material.metallic", 0.f, true);
	setProperty("material.roughness", 0.5f, true);
	for (int i = 0; i < textureInputs.size(); i++) {
		m_textures[textureInputs[i]] = nullptr;
	}

}

void PBRMaterial::setTexture(Texture* tex, Texture::TextureType type)
{
	auto it = std::find(textureInputs.begin(), textureInputs.end(), type);
	if (it != textureInputs.end()) {
		m_textures[type] = tex;
	}
}

std::unordered_map<Texture::TextureType, Texture*> PBRMaterial::getTextures()
{
	return m_textures;
}

ShaderManager::ShaderType PBRMaterial::getShaderType()
{
	return shaderType;
}
