#pragma once
#include "material.h"
class PhongMaterial: public Material
{
public:
	PhongMaterial();
	PhongMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess);
	PhongMaterial(Texture* diffuseMap, Texture* specularMap, float shininess = 0.75f * 128.0f);
	void passToShader(Shader* shader) override;

private:
	bool textured;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Texture* diffuseMap;
	Texture* specularMap;
	float shininess;
};

