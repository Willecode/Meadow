#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "shader_s.h"
#include "lightsource.h"
#include "mesh.h"
#include "materials/material.h"


class GraphicsComponent
{
public:
	static constexpr int MAX_MATERIAL_SLOTS = 10;
	int id; // unique id managed by the scene.
	std::string name;

	GraphicsComponent();
	void draw(std::unordered_map<GraphicsComponent*, LightSource*> sceneLights, int pointLightCount, int dirLightCount, glm::mat4 accTransform = glm::mat4(1.0f));
	void addLightSource(std::shared_ptr<LightSource> lightSrc);
	void addMesh(std::shared_ptr<Mesh> mesh, int materialSlot);
	void addChild(std::shared_ptr<GraphicsComponent> child);
	void setModelMatrix(const glm::mat4& model);
	void setMaterial(std::shared_ptr<Material> mat, int materialSlot);
	void setShader(Shader* sdr);
	Material* getMaterial(int materialSlot);
	glm::mat4 getModelMatrix();
	std::vector<Mesh*> getMeshes();

	std::shared_ptr<LightSource> getLightSourceOwnerhip();
	LightSource* getLightSource();
	glm::vec3 getPosition();
	Shader* getShader();

protected:
	struct MeshStruct {
		std::shared_ptr<Mesh> mesh;
		unsigned int materialSlot;
	};

	glm::mat4 modelMatrix;
	std::vector<std::shared_ptr<GraphicsComponent>> childObjects;
	std::shared_ptr<LightSource> light;
	std::vector<MeshStruct> meshes;
	std::array<std::shared_ptr<Material>, MAX_MATERIAL_SLOTS - 1> materialSlots;
	Shader* shader;
};

