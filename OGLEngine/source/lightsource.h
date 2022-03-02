#pragma once
#include <glm/glm.hpp>
#include "shader_s.h"
//--------------------------------------
// STRING FORMATTING
#define FMT_HEADER_ONLY
#include <fmt/core.h>
//--------------------------------------
#define DEFAULT_LIGHT_COLOR glm::vec3(1.0f)
class LightSource
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	enum class LightType {POINTLIGHT, DIRLIGHT};

	LightSource(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
		ambient(ambient), diffuse(diffuse), specular(specular){}
	virtual void passToShader(Shader* shader, GLuint lightID, glm::vec3 position) = 0;
	virtual LightType getType() = 0;
protected:

};

class PointLight : public LightSource {
public:
	float constant;
	float linear;
	float quadratic;

	PointLight():
		LightSource(
			glm::vec3(DEFAULT_LIGHT_COLOR * 0.1f),
			glm::vec3(DEFAULT_LIGHT_COLOR * 0.5f),
			glm::vec3(DEFAULT_LIGHT_COLOR)
		), constant(1.0f), linear(0.09f), quadratic(0.032f) {}

	PointLight(
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f
	):
		LightSource(ambient, diffuse, specular),
		constant(constant), linear(linear), quadratic(quadratic)
	{
		return;
	}
		
	void passToShader(Shader* shader, GLuint lightID, glm::vec3 position) override {
		shader->setFloat3(fmt::format("pointLights[{}].position", lightID),  position);
		shader->setFloat3(fmt::format("pointLights[{}].ambient", lightID),   ambient);
		shader->setFloat3(fmt::format("pointLights[{}].diffuse", lightID),   diffuse);
		shader->setFloat3(fmt::format("pointLights[{}].specular", lightID),  specular);
		shader->setFloat (fmt::format("pointLights[{}].constant", lightID),  constant);
		shader->setFloat (fmt::format("pointLights[{}].linear", lightID),    linear);
		shader->setFloat (fmt::format("pointLights[{}].quadratic", lightID), quadratic);
	}
	LightType getType() override {
		return LightType::POINTLIGHT;
	}
};

class DirectionalLight : public LightSource {
public:
	glm::vec3 direction;

	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction):
		LightSource(ambient, diffuse, specular), direction(direction)
	{}
	DirectionalLight(glm::vec3 direction):
		LightSource(
			glm::vec3(DEFAULT_LIGHT_COLOR * 0.1f),
			glm::vec3(DEFAULT_LIGHT_COLOR * 0.5f),
			glm::vec3(DEFAULT_LIGHT_COLOR)
		), direction(direction)
	{}

	void passToShader(Shader* shader, GLuint lightID, glm::vec3 position) override {
		shader->setFloat3(fmt::format("dirLight[{}].direction", lightID), direction);
		shader->setFloat3(fmt::format("dirLight[{}].ambient", lightID),   ambient);
		shader->setFloat3(fmt::format("dirLight[{}].diffuse", lightID),   diffuse);
		shader->setFloat3(fmt::format("dirLight[{}].specular", lightID),  specular);
	}
	LightType getType() override {
		return LightType::DIRLIGHT;
	}
};