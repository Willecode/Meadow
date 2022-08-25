#include "mesh.h"

Mesh::Mesh(std::string name) :
	Asset(name),
	submeshes()
{
}

void Mesh::draw(glm::mat4 modelMat, ShaderManager* sdrMan)
{
	sdrMan->setUniformDrawSpecific("model", modelMat);
	for (auto const& materialMesh : submeshes) {
		materialMesh.first->passToShader(sdrMan);
		sdrMan->forwardUniformsDrawSpecific();
		for (auto const& submesh : materialMesh.second) {
			submesh->draw();
		}
	}
}

void Mesh::drawWithoutMaterial(glm::mat4 modelMat, ShaderManager* sdrMan)
{
	sdrMan->setUniformDrawSpecific("model", modelMat);
	for (auto const& materialMesh : submeshes) {
		sdrMan->forwardUniformsDrawSpecific();
		for (auto const& submesh : materialMesh.second) {
			submesh->draw();
		}
	}
}

void Mesh::addSubMesh(Material* material, SubMesh* submesh)
{
	/*
	* Check if smesh already exists
	*/
	if(findSubmesh(submesh) == submeshes.end())
		submeshes[material].push_back(submesh);
}

void Mesh::setSubMeshMaterial(Material* material, SubMesh* submesh)
{
	/*
	* Find submesh material iterator
	*/
	auto it = findSubmesh(submesh);
	/*
	* Remove submesh from that materials list of submeshes
	*/
	if (it != submeshes.end()) {
		auto itv = std::find(it->second.begin(), it->second.end(), submesh);
		it->second.erase(itv);
	}
	else
		return; // if submesh not found, return
	/*
	* Add the submesh to the param materials submeshes
	*/
	submeshes[material].push_back(submesh);
		
}

void Mesh::clearSubmeshes()
{
	submeshes.clear();
}

Asset::AssetType Mesh::getAssetType()
{
	return AssetType::MESH;
}

Mesh::SubmeshMap::iterator Mesh::findSubmesh(SubMesh* submesh)
{
	for (auto it = submeshes.begin(); it != submeshes.end(); it++) {
		for (auto const& smesh : it->second) {
			if (submesh == smesh)
				return it; // smesh found in mesh
		}
	}
	return submeshes.end();
}
