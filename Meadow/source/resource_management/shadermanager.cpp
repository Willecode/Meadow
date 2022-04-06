#include "shadermanager.h"

std::unique_ptr<Shader> ShaderManager::m_PhongSdr;
std::unique_ptr<Shader> ShaderManager::m_ColorSdr;
bool ShaderManager::initialized = false;
ShaderManager::ShaderManager()
{
}

void ShaderManager::initShaderManager()
{
	m_PhongSdr = std::make_unique<Shader>(0, "shaders/object.vs", "shaders/phongtex.fs");
	m_ColorSdr = std::make_unique<Shader>(1, "shaders/object.vs", "shaders/coloronly.fs");
	initialized = true;
}

Shader* ShaderManager::getPhongShader()
{
	if (!initialized)
		return nullptr;
	else
		return m_PhongSdr.get();
}

Shader* ShaderManager::getColorShader()
{
	if (!initialized)
		return nullptr;
	else
		return m_ColorSdr.get();
}
