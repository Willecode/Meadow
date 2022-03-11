#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "shader_s.h"
#include "lightsource.h"
#include "mesh.h"
#include "materials/material.h"


class Object3D
{
public:
	static constexpr int MAX_MATERIAL_SLOTS = 10;
	int id;

	Object3D();
	void draw(std::unordered_map<Object3D*, LightSource*> sceneLights, int pointLightCount, int dirLightCount);
	void addLightSource(std::shared_ptr<LightSource> lightSrc);
	void addMesh(std::shared_ptr<Mesh> mesh, int materialSlot);
	void setModelMatrix(const glm::mat4& model);
	void setMaterial(std::shared_ptr<Material> mat, int materialSlot);
	void setShader(Shader* sdr);
	Material* getMaterial(int materialSlot);

	std::shared_ptr<LightSource> getLightSourceOwnerhip();
	LightSource* getLightSource();
	glm::vec3 getPosition();
	Shader* getShader();

protected:
	struct MeshStruct {
		std::shared_ptr<Mesh> mesh;
		unsigned int materialSlot;
	};

	std::string name;
	glm::mat4 modelMatrix;
	//std::vector<std::shared_ptr<Object3D>> childObjects;
	std::shared_ptr<LightSource> light;
	std::vector<MeshStruct> meshes;
	std::array<std::shared_ptr<Material>, MAX_MATERIAL_SLOTS - 1> materialSlots;
	Shader* shader;
};

