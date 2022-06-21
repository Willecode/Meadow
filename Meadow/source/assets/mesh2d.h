#pragma once
#include "vertex2d.h"
#include <vector>
#include <string>
//#include "assets/texture.h"
#include "resource_management/shadermanager.h"

/*
* Used to render in 2D
*/
class Mesh2D: public Asset
{
public:
	Mesh2D(
		std::vector<Vertex2D> vertices,
		std::vector<unsigned int> indices,
		std::string name = "Nameless mesh");

	std::vector<Vertex2D> vertices;
	std::vector<unsigned int> indices;

	void draw(ShaderManager* sdrMan);
	void generateBuffers(); // Generate graphics memory buffers for this mesh
	void buffersPushData(); // Push vertex data and indices into previously generated buffers
	void deleteBuffers();   // Free the graphics memory buffers this mesh is occupying
	void setTexture(Texture* tex);

	// Asset interface
	AssetType getAssetType() override;
private:
	Texture* m_texture;
};

