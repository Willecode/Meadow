#include "shadermanager.h"
#include "service_locator/rendererlocator.h"
#include "input/inputevents.h"
#include "service_locator/loggerlocator.h"
ShaderManager::ShaderManager() :
	m_currentShader(nullptr),
	m_shaderMap(),
	m_boolMapDraw(),
	m_intMapDraw(),
	m_floatMapDraw(),
	m_vec3MapDraw(),
	m_mat4MapDraw(),
	m_boolMapFrame(),
	m_intMapFrame(),
	m_floatMapFrame(),
	m_vec3MapFrame(),
	m_mat4MapFrame(),
	m_texSamplerMap(),
	m_cameraUnifDependantShaders()
{
	/*
	* Texture sampler id mapping
	*/
	m_texSamplerMap.insert({ Texture::TextureType::CUBE_MAP,      0 });
	m_texSamplerMap.insert({ Texture::TextureType::ALBEDO_MAP,    0 });
	m_texSamplerMap.insert({ Texture::TextureType::OPACITY_MAP,   2 });
	m_texSamplerMap.insert({ Texture::TextureType::NORMAL_MAP,    3 });
	m_texSamplerMap.insert({ Texture::TextureType::METALLIC_MAP,  4 });
	m_texSamplerMap.insert({ Texture::TextureType::ROUGHNESS_MAP, 5 });
	m_texSamplerMap.insert({ Texture::TextureType::AO_MAP,		  6 });


	/*
	* Subscribe to events
	*/
	InputEvents::ShaderHotReloadEvent::subscribe(
		std::bind(&ShaderManager::hotReloadShader, this, std::placeholders::_1)
	);
}

ShaderManager& ShaderManager::getInstance()
{
	static ShaderManager instance;
	return instance;
}

void ShaderManager::init()
{
	/*
	* Shader compilation
	*/
	auto pbrSdr = std::make_unique<Shader>(0, "shaders/object.vs", "shaders/pbr.fs");
	auto colorSdr = std::make_unique<Shader>(1, "shaders/object.vs", "shaders/coloronly.fs");
	auto depthSdr = std::make_unique<Shader>(2, "shaders/object.vs", "shaders/depth.fs");
	auto screenQuadSdr = std::make_unique<Shader>(3, "shaders/2d.vs", "shaders/postprocessing.fs");
	auto skyboxSdr = std::make_unique<Shader>(4, "shaders/skybox.vs", "shaders/skybox.fs");
	auto colliderSdr = std::make_unique<Shader>(5, "shaders/object.vs", "shaders/collider.fs");

	m_shaderMap.insert({ ShaderType::COLOR_ONLY,  std::move(colorSdr) });
	m_shaderMap.insert({ ShaderType::DEPTH,	   std::move(depthSdr) });
	m_shaderMap.insert({ ShaderType::POSTPROCESS, std::move(screenQuadSdr) });
	m_shaderMap.insert({ ShaderType::PBR,		   std::move(pbrSdr) });
	m_shaderMap.insert({ ShaderType::SKYBOX,      std::move(skyboxSdr) });
	m_shaderMap.insert({ ShaderType::COLLIDER,      std::move(colliderSdr) });

	m_cameraUnifDependantShaders.push_back(ShaderType::COLOR_ONLY);
	//m_cameraUnifDependantShaders.push_back(ShaderType::DEPTH);
	m_cameraUnifDependantShaders.push_back(ShaderType::PBR);
	m_cameraUnifDependantShaders.push_back(ShaderType::SKYBOX);
	m_cameraUnifDependantShaders.push_back(ShaderType::COLLIDER);

	bindShader(ShaderType::PBR);

	// Logging
	// -------------
	LoggerLocator::getLogger()->getLogger()->info("ShaderManager initialized");
	// -------------

	return;
}


Shader* ShaderManager::getShader(ShaderType sdr)
{
	return m_shaderMap[sdr].get();
}

const std::vector<ShaderManager::ShaderType>* ShaderManager::getCameraDependant()
{
	return &m_cameraUnifDependantShaders;
}

void ShaderManager::bindShader(ShaderType sdr)
{
	Shader* s = m_shaderMap[sdr].get();
	if (s != nullptr) {
		RendererLocator::getRenderer()->useShaderProgram(s->getId());
		m_currentShader = s;
	}
	else {
		LoggerLocator::getLogger()->getLogger()->error("ShaderManager: tried to use nullptr shader");
	}
}

//void ShaderManager::setCurrentShader(std::string name)
//{
//	auto it = m_shaderMap.find(name);
//	if (it != m_shaderMap.end())
//		m_currentShader = (it->second).get();
//		RendererLocator::getRenderer()->useShaderProgram(it->second->getId());
//}

//Shader* ShaderManager::getCurrentShader()
//{
//	return m_currentShader;
//}

void ShaderManager::setUniformDrawSpecific(std::string uName, bool uValue)
{
	m_boolMapDraw[uName] = uValue;
}

void ShaderManager::setUniformDrawSpecific(std::string uName, int uValue)
{
	m_intMapDraw[uName] = uValue;
}

void ShaderManager::setUniformDrawSpecific(std::string uName, unsigned int uValue)
{
	m_intMapDraw[uName] = uValue;
}

void ShaderManager::setUniformDrawSpecific(std::string uName, float uValue)
{
	m_floatMapDraw[uName] = uValue;
}

void ShaderManager::setUniformDrawSpecific(std::string uName, glm::vec3 uValue)
{
	m_vec3MapDraw[uName] = uValue;
}

void ShaderManager::setUniformDrawSpecific(std::string uName, glm::mat4 uValue)
{
	m_mat4MapDraw[uName] = uValue;
}

void ShaderManager::setFrameUniform(std::string uName, bool uValue)
{
	m_boolMapFrame[uName] = uValue;
}

void ShaderManager::setFrameUniform(std::string uName, int uValue)
{
	m_intMapFrame[uName] = uValue;
}

void ShaderManager::setFrameUniform(std::string uName, unsigned int uValue)
{
	m_uintMapFrame[uName] = uValue;
}

void ShaderManager::setFrameUniform(std::string uName, float uValue)
{
	m_floatMapFrame[uName] = uValue;
}

void ShaderManager::setFrameUniform(std::string uName, glm::vec3 uValue)
{
	m_vec3MapFrame[uName] = uValue;
}

void ShaderManager::setFrameUniform(std::string uName, glm::mat4 uValue)
{
	m_mat4MapFrame[uName] = uValue;
}

void ShaderManager::forwardUniformsDrawSpecific()
{
	for (auto uniform : m_boolMapDraw)
		RendererLocator::getRenderer()->setBool(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_intMapDraw)
		RendererLocator::getRenderer()->setInt(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_uintMapDraw)
		RendererLocator::getRenderer()->setuInt(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_floatMapDraw)
		RendererLocator::getRenderer()->setFloat(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_vec3MapDraw)
		RendererLocator::getRenderer()->setFloat3(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_mat4MapDraw)
		RendererLocator::getRenderer()->setMat4f(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
}

void ShaderManager::forwardFrameUniforms()
{
	for (auto uniform : m_boolMapFrame)
		RendererLocator::getRenderer()->setBool(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_intMapFrame)
		RendererLocator::getRenderer()->setInt(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_uintMapFrame)
		RendererLocator::getRenderer()->setuInt(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_floatMapFrame)
		RendererLocator::getRenderer()->setFloat(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_vec3MapFrame)
		RendererLocator::getRenderer()->setFloat3(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
	for (auto uniform : m_mat4MapFrame)
		RendererLocator::getRenderer()->setMat4f(m_currentShader->getId(), uniform.first.c_str(), uniform.second);
}

void ShaderManager::clearDrawUniforms()
{
	m_boolMapDraw.clear();
	m_intMapDraw.clear();
	m_uintMapDraw.clear();
	m_floatMapDraw.clear();
	m_vec3MapDraw.clear();
	m_mat4MapDraw.clear();
}

void ShaderManager::clearFrameUniforms()
{
	m_boolMapFrame.clear();
	m_intMapFrame.clear();
	m_uintMapFrame.clear();
	m_floatMapFrame.clear();
	m_vec3MapFrame.clear();
	m_mat4MapFrame.clear();
}

unsigned int ShaderManager::getTexSamplerId(Texture::TextureType type)
{
	return m_texSamplerMap.at(type);
}

void ShaderManager::hotReloadShader(ShaderType type)
{
	auto it = m_shaderMap.find(type);
	if (it != m_shaderMap.end())
		it->second->hotReload();
}
