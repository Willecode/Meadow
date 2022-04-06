#pragma once
#include "material.h"
#include <memory>
class PhongMaterial: public Material
{
public:
	float shininess;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	PhongMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess = 10.0f);
	PhongMaterial(
		std::shared_ptr<Texture> diffuseMap = nullptr,
		std::shared_ptr<Texture> specularMap= nullptr,
		float shininess = 10.0f);
	void passToRenderer() override;

private:
	std::shared_ptr<Texture> m_diffuseMap;
	std::shared_ptr<Texture> m_specularMap;
	Shader* m_shader;
};

