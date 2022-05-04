#pragma once
#include "assets/submesh.h"
#include "assets/material.h"
#include <glm/glm.hpp>
#include "resource_management/shadermanager.h"
#include <map>
class Mesh : public Asset
{
public:
	//static constexpr int MAX_MATERIAL_SLOTS = 10;
	/*
	* Contains all submeshes the mesh consists of, each assigned to a material.
	* Material is key to ease drawing (pass material props to shader, then draw all the submeshes using that material)
	* Duplicate submeshes not allowed.
	*/
	typedef std::map<Material*, std::vector<SubMesh*>> SubmeshMap;
	SubmeshMap submeshes;
public:
	Mesh(std::string name = "Nameless mesh");
	void draw(glm::mat4 modelMat, ShaderManager* sdrMan);
	/*
	* If submesh not in map, add it and assign it to param material
	*/
	void addSubMesh(Material* material, SubMesh* submesh);

	/*
	* If submesh found in mesh, assign it to param material
	*/
	void setSubMeshMaterial(Material* material, SubMesh* submesh);
	void clearSubmeshes();
	AssetType getAssetType() override;
private:
	SubmeshMap::iterator findSubmesh(SubMesh* submesh);
};

