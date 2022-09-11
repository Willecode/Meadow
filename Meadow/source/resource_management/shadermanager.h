#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "shader.h"
#include "assets/texture.h"
class ShaderManager
{
public:
	enum class ShaderType {
		PBR = 0,
		COLOR_ONLY,
		POSTPROCESS,
		DEPTH,
		SKYBOX
	};

	ShaderManager();
	void init();
	//void provideShader(std::string name, std::unique_ptr<Shader> shader);
	//void setCurrentShader(std::string name);
	//Shader* getCurrentShader();
	Shader* getShader(ShaderType sdr);
	void bindShader(ShaderType sdr);

	/*
	* Set shader uniforms that should be forwarded every draw call
	*/
	void setUniformDrawSpecific(std::string uName, bool uValue);
	void setUniformDrawSpecific(std::string uName, int uValue);
	void setUniformDrawSpecific(std::string uName, unsigned int uValue);
	void setUniformDrawSpecific(std::string uName, float uValue);
	void setUniformDrawSpecific(std::string uName, glm::vec3 uValue);
	void setUniformDrawSpecific(std::string uName, glm::mat4 uValue);

	/*
	* Set shader uniforms that should be forwarded once per frame
	*/
	void setFrameUniform(std::string uName, bool uValue);
	void setFrameUniform(std::string uName, int uValue);
	void setFrameUniform(std::string uName, unsigned int uValue);
	void setFrameUniform(std::string uName, float uValue);
	void setFrameUniform(std::string uName, glm::vec3 uValue);
	void setFrameUniform(std::string uName, glm::mat4 uValue);

	void forwardUniformsDrawSpecific();
	void forwardFrameUniforms();

	unsigned int getTexSamplerId(Texture::TextureType type);
private:
	//std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaderMap;
	std::unordered_map<ShaderType, std::unique_ptr<Shader>> m_shaderMap;

	Shader* m_currentShader;

	/*
	* Shader uniform maps for uniforms that should be forwarded every draw call
	*/
	std::unordered_map<std::string, bool>         m_boolMapDraw;
	std::unordered_map<std::string, int>	      m_intMapDraw;
	std::unordered_map<std::string, unsigned int> m_uintMapDraw;
	std::unordered_map<std::string, float>	      m_floatMapDraw;
	std::unordered_map<std::string, glm::vec3>    m_vec3MapDraw;
	std::unordered_map<std::string, glm::mat4>    m_mat4MapDraw;
	/*
	* Shader uniform maps for uniforms that should be forwarded once per frame
	*/
	std::unordered_map<std::string, bool>	      m_boolMapFrame;
	std::unordered_map<std::string, int>	      m_intMapFrame;
	std::unordered_map<std::string, unsigned int> m_uintMapFrame;
	std::unordered_map<std::string, float>	      m_floatMapFrame;
	std::unordered_map<std::string, glm::vec3>    m_vec3MapFrame;
	std::unordered_map<std::string, glm::mat4>    m_mat4MapFrame;

	/*
	* Maps texture type to the associated sampler.
	*/
	std::unordered_map<Texture::TextureType, unsigned int> m_texSamplerMap;

private:
	void hotReloadShader(ShaderType type);
};

