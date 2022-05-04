#include "submesh.h"
#include "service_locator/locator.h"

SubMesh::SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string name):
    vertices(vertices), indices(indices), Asset(name)
{

}

void SubMesh::draw()
{
    Locator::getRenderer()->drawMesh(getId());
}

void SubMesh::generateBuffers()
{
    Locator::getRenderer()->meshBuffersGenerate(getId());
}

void SubMesh::buffersPushData()
{
    Locator::getRenderer()->meshBuffersPushData(getId(), vertices, indices);
}

void SubMesh::deleteBuffers()
{
    Locator::getRenderer()->meshBuffersDelete(getId());
}

SubMesh::AssetType SubMesh::getAssetType()
{
    return AssetType::SUBMESH;
}
