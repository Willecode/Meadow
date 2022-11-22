#include "shader/shaderuniformnamemap.h"

std::unordered_map<Texture::TextureType, std::string> ShaderUniformNameMap::m_textureMaps = {
	{Texture::TextureType::ALBEDO_MAP, ShaderUniformNames::MATERIAL_ALBEDOMAP},
	{Texture::TextureType::NORMAL_MAP, ShaderUniformNames::MATERIAL_NORMALMAP},
	{Texture::TextureType::METALLIC_MAP, ShaderUniformNames::MATERIAL_METALLICMAP},
	{Texture::TextureType::ROUGHNESS_MAP, ShaderUniformNames::MATERIAL_ROUGHNESSMAP},
	{Texture::TextureType::AO_MAP, ShaderUniformNames::MATERIAL_AOMAP},
	{Texture::TextureType::OPACITY_MAP, ShaderUniformNames::MATERIAL_OPACITYMAP},
	{Texture::TextureType::SHADOW_MAP, ShaderUniformNames::SHADOW_MAP}
};
std::unordered_map<Texture::TextureType, std::string> ShaderUniformNameMap::m_hasTextureMaps = {
	{Texture::TextureType::ALBEDO_MAP, ShaderUniformNames::MATERIAL_HAS_ALBEDOMAP},
	{Texture::TextureType::NORMAL_MAP, ShaderUniformNames::MATERIAL_HAS_NORMALMAP},
	{Texture::TextureType::METALLIC_MAP, ShaderUniformNames::MATERIAL_HAS_METALLICMAP},
	{Texture::TextureType::ROUGHNESS_MAP, ShaderUniformNames::MATERIAL_HAS_ROUGHNESSMAP},
	{Texture::TextureType::AO_MAP, ShaderUniformNames::MATERIAL_HAS_AOMAP},
	{Texture::TextureType::OPACITY_MAP, ShaderUniformNames::MATERIAL_HAS_OPACITYMAP}
};

std::string ShaderUniformNameMap::getTexMapName(Texture::TextureType type)
{
	return m_textureMaps.at(type);
}

std::string ShaderUniformNameMap::getHasTexMapName(Texture::TextureType type)
{
	return m_hasTextureMaps.at(type);
}
