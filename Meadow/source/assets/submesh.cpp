#include "submesh.h"
#include "service_locator/rendererlocator.h"
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
    RendererLocator::getRenderer()->drawMesh(getId());
}

void SubMesh::generateBuffers()
{
    RendererLocator::getRenderer()->meshBuffersGenerate(getId());
}

void SubMesh::buffersPushData()
{
    RendererLocator::getRenderer()->meshBuffersPushData(getId(), vertices, indices);
}

void SubMesh::deleteBuffers()
{
    RendererLocator::getRenderer()->meshBuffersDelete(getId());
}

SubMesh::AssetType SubMesh::getAssetType()
{
    return AssetType::SUBMESH;
}
