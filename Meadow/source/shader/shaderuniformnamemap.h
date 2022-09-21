#pragma once
#include <unordered_map>
#include <assets/texture.h>
#include "shaderuniformnames.h"
class ShaderUniformNameMap
{
public:
	ShaderUniformNameMap() {};
	std::string getTexMapName(Texture::TextureType type);
	std::string getHasTexMapName(Texture::TextureType type);
private:
	static std::unordered_map<Texture::TextureType, std::string> m_textureMaps;
	static std::unordered_map<Texture::TextureType, std::string> m_hasTextureMaps;

};

