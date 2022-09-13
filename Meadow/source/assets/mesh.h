#pragma once
#include "assets/submesh.h"
#include "assets/materials/material.h"
#include <glm/glm.hpp>
#include "resource_management/shadermanager.h"
#include <map>
class Mesh : public Asset
{
public:
	/*
	* Contains all submeshes the mesh consists of, each assigned to a material.
	* Material is key to ease drawing (pass material props to shader, then draw all the submeshes using that material)
	* Duplicate submeshes not allowed.
	*/
	typedef std::map<Material*, std::vector<SubMesh*>> SubmeshMap;
	SubmeshMap submeshes;
public:
	Mesh(std::string name = "Nameless mesh");

	/*
	* Draw each submesh, passing their materials to the shader
	*/
	void draw(glm::mat4 modelMat);

	/*
	* Draw each submesh without passing materials
	*/
	void drawWithoutMaterial(glm::mat4 modelMat);

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

