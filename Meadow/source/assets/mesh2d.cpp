#include "mesh2d.h"
//#include "service_locator/locator.h"

Mesh2D::Mesh2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices, std::string name):
	vertices(vertices), indices(indices), Asset(name), m_texture(nullptr)
{
    /*
   * The vertice and indice count must be above zero
   */
    if (vertices.size() == 0 || indices.size() == 0)
        throw std::invalid_argument("Mesh2D: received an empty vertices or indices vector");
}

void Mesh2D::draw(ShaderManager* sdrMan)
{
    /*
    * 2D meshes have one texture to sample from in the shaders,
    * The sampler will always be sampler 0
    */
    m_texture->bindToSampler(0);
    sdrMan->setUniformDrawSpecific("screenTexture", 0);
    Locator::getRenderer()->wireframe(false); // There isn't really a reason to draw a 2D mesh in wireframe
    Locator::getRenderer()->drawMesh(getId());
}

void Mesh2D::generateBuffers()
{
    Locator::getRenderer()->meshBuffersGenerate(getId());
}

void Mesh2D::buffersPushData()
{
    Locator::getRenderer()->mesh2DBuffersPushData(getId(), vertices, indices);
}

void Mesh2D::deleteBuffers()
{
    Locator::getRenderer()->meshBuffersDelete(getId());
}

void Mesh2D::setTexture(Texture* tex)
{
    m_texture = tex;
}

Asset::AssetType Mesh2D::getAssetType()
{
    return AssetType::MESH2D;
}
