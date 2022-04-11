#include "mesh.h"
#include "service_locator/locator.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices):
    vertices(vertices), indices(indices)
{

}

void Mesh::draw()
{
    Locator::getRenderer()->drawMesh(getId());
}

void Mesh::generateBuffers()
{
    Locator::getRenderer()->meshBuffersGenerate(getId());
}

void Mesh::buffersPushData()
{
    Locator::getRenderer()->meshBuffersPushData(getId(), vertices, indices);
}

void Mesh::deleteBuffers()
{
    Locator::getRenderer()->meshBuffersDelete(getId());
}

Mesh::AssetType Mesh::getAssetType()
{
    return AssetType::MESH;
}
