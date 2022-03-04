#pragma once
#include "material.h"
#include <memory>
class PhongMaterial: public Material
{
public:
	PhongMaterial();
	PhongMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess);
	PhongMaterial(std::shared_ptr<Texture> diffuseMap, std::shared_ptr<Texture> specularMap, float shininess = 0.75f * 128.0f);
	void passToShader(Shader* shader) override;

private:
	bool textured;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> specularMap;
	float shininess;
};

