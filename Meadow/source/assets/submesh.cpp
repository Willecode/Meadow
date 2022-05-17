#include "submesh.h"
#include "service_locator/locator.h"
#include <stdexcept>

SubMesh::SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string name):
    vertices(vertices), indices(indices), Asset(name)
{
    /*
    * The vertice and indice count must be above zero
    */
    if (vertices.size() == 0 || indices.size() == 0)
        throw std::invalid_argument("SubMesh: received an empty vertices or indices vector");

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
