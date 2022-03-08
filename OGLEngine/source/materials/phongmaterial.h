#pragma once
#include "material.h"
#include <memory>
class PhongMaterial: public Material
{
public:
	PhongMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess);
	PhongMaterial(
		std::shared_ptr<Texture> diffuseMap = nullptr,
		std::shared_ptr<Texture> specularMap= nullptr,
		float shininess = 0.75f * 128.0f);
	void passToShader(Shader* shader) override;

private:
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> specularMap;
	float shininess;
};

