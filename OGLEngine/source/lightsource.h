#pragma once
#include <glm/glm.hpp>
#include "shader_s.h"
//--------------------------------------
// STRING FORMATTING
#define FMT_HEADER_ONLY
#include <fmt/core.h>
//--------------------------------------
class LightSource
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	virtual void passToShader(Shader* shader, GLuint lightID, glm::vec3 position) = 0;
};

class PointLight : public LightSource {
public:
	float constant;
	float linear;
	float quadratic;
	void passToShader(Shader* shader, GLuint lightID, glm::vec3 position) override {
		shader->setFloat3(fmt::format("pointLights[{}].position", lightID),  position);
		shader->setFloat3(fmt::format("pointLights[{}].ambient", lightID),   ambient);
		shader->setFloat3(fmt::format("pointLights[{}].diffuse", lightID),   diffuse);
		shader->setFloat3(fmt::format("pointLights[{}].specular", lightID),  specular);
		shader->setFloat (fmt::format("pointLights[{}].constant", lightID),  constant);
		shader->setFloat (fmt::format("pointLights[{}].linear", lightID),    linear);
		shader->setFloat (fmt::format("pointLights[{}].quadratic", lightID), quadratic);
	}
};

class DirectionalLight : public LightSource {
public:
	glm::vec3 direction;
	/*
	Right now shader supports only one directional light so lightID is redundant
	*/
	void passToShader(Shader* shader, GLuint lightID, glm::vec3 position) override {
		shader->setFloat3("dirLight.direction", direction);
		shader->setFloat3("dirLight.ambient",   ambient);
		shader->setFloat3("dirLight.diffuse",   diffuse);
		shader->setFloat3("dirLight.specular",  specular);
	}
};