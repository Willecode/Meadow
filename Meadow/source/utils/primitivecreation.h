#pragma once
#include <memory>
#include "assets/submesh.h"
#include "assets/mesh2d.h"
class PrimitiveCreation
{
public:
	static std::unique_ptr<SubMesh> createCubeMesh();
	static std::unique_ptr<SubMesh> createPlane();
	static std::unique_ptr<SubMesh> createPlane(float widthHeight);
	static std::unique_ptr<SubMesh> createSphere(int sectorCount, int stackCount);

	/*
	* returns a quad that fills the screen, used for post processing
	*/
	static std::unique_ptr<Mesh2D> createScreenQuad();

private:
	/*
	* 
	*/
	static void addTangentBitangent(Vertex& vert1, Vertex& vert2, Vertex& vert3);
};