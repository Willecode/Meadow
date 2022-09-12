#include "colormaterial.h"

std::array<Texture::TextureType, 6> ColorMaterial::textureInputs = {
	Texture::TextureType::ALBEDO_MAP
};
ShaderManager::ShaderType ColorMaterial::shaderType = ShaderManager::ShaderType::COLOR_ONLY;

ColorMaterial::ColorMaterial(std::string name)
{
	setDefault();
}

void ColorMaterial::setDefault()
{
	clearProperties();
	setProperty("material.albedo", MaterialConstants::DEFAULT_COLOR, true);
	for (int i = 0; i < textureInputs.size(); i++) {
		m_textures[textureInputs[i]] = nullptr;
	}
}

void ColorMaterial::setTexture(Texture* tex, Texture::TextureType type)
{
	auto it = std::find(textureInputs.begin(), textureInputs.end(), type);
	if (it != textureInputs.end()) {
		m_textures[type] = tex;
	}
}

std::unordered_map<Texture::TextureType, Texture*> ColorMaterial::getTextures()
{
	return m_textures;
}

ShaderManager::ShaderType ColorMaterial::getShaderType()
{
	return shaderType;
}
