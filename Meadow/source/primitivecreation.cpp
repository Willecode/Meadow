#include "primitivecreation.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtx/string_cast.hpp>
#include "service_locator/loggerlocator.h"

std::unique_ptr<SubMesh> PrimitiveCreation::createCubeMesh()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
                              //pos                           //normal                      //texCoord
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f))); // back
    vertices.push_back(Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)));

    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f))); // front
    vertices.push_back(Vertex(glm::vec3( 0.5f, -0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)));

    vertices.push_back(Vertex(glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f))); // left
    vertices.push_back(Vertex(glm::vec3(-0.5f,  0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

    vertices.push_back(Vertex(glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f))); // right
    vertices.push_back(Vertex(glm::vec3( 0.5f,  0.5f,-0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3( 0.5f, -0.5f,-0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3( 0.5f, -0.5f, 0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f))); // bottom
    vertices.push_back(Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

    vertices.push_back(Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f))); // top
    vertices.push_back(Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

    indices = { // consider the CCW winding order
        0, 2, 1, 0, 3, 2,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 14, 13, 12, 15, 14,
        16, 17, 18, 16, 18, 19,
        20, 22, 21, 20, 23, 22
    };

    // Add tangents and bitangents
    for (int i = 0; i < indices.size(); i += 3) {
        addTangentBitangent(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
    }

    return std::make_unique<SubMesh>(vertices, indices, "Cube");
    
}

std::unique_ptr<SubMesh> PrimitiveCreation::createSphere(int sectorCount, int stackCount)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    float pi = static_cast<float>(M_PI);
    float sectorStep = 2 * pi / sectorCount;
    float stackStep = pi / stackCount;
    float sectorAngle, stackAngle;
    float x, y, z;

    for (int i = 0; i <= stackCount; i++) { 
        stackAngle = (-pi / 2) + i * stackStep;
        y = sinf(stackAngle);
        for (int j = 0; j <= sectorCount; j++) {
            sectorAngle = j * sectorStep;

            x = cosf(stackAngle) * cosf(sectorAngle);
            z = cosf(stackAngle) * sinf(sectorAngle);

            Vertex vert(
                glm::vec3(x, y, z),
                glm::vec3(x, y, z),
                glm::vec2(((float)j) / sectorCount, ((float)i) / stackCount));
            vertices.push_back(vert);
        }
    }
    for (int i = 0; i < stackCount; i++) {
        int stackFirst = (sectorCount + 1) * i ;
        int nextStackFirst = stackFirst + sectorCount + 1;
        for (int j = 0; j < sectorCount; j++) {
            indices.push_back(stackFirst + j);
            indices.push_back(nextStackFirst + j);
            indices.push_back(stackFirst + j + 1);

            indices.push_back(stackFirst + j + 1);
            indices.push_back(nextStackFirst + j);
            indices.push_back(nextStackFirst + j + 1);
        }
    }
    LoggerLocator::getLogger()->getLogger()->info("created a sphere with {} vertices", vertices.size());
    return std::make_unique<SubMesh>(vertices, indices, "Sphere");
}

std::unique_ptr<Mesh2D> PrimitiveCreation::createScreenQuad()
{
    std::vector<Vertex2D> vertices;
    std::vector<unsigned int> indices;
                                         //positon               //texcoords
    vertices.push_back(Vertex2D(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f))); // top left
    vertices.push_back(Vertex2D(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)));// bot left
    vertices.push_back(Vertex2D(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f))); // top right
    vertices.push_back(Vertex2D(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)));  // bot right

    indices = {
        0,1,2,
        1,3,2
    };

    return std::make_unique<Mesh2D>(vertices, indices, "Screen Quad");
}

void PrimitiveCreation::addTangentBitangent(Vertex& vert1, Vertex& vert2, Vertex& vert3)
{
    glm::vec3 edge1 = vert2.position - vert1.position;
    glm::vec3 edge2 = vert3.position - vert1.position;
    glm::vec2 deltaUV1 = vert2.texCoords - vert1.texCoords;
    glm::vec2 deltaUV2 = vert3.texCoords - vert1.texCoords;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent, bitangent;

    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    vert1.tangent = tangent;
    vert2.tangent = tangent;
    vert3.tangent = tangent;

    vert1.bitangent = bitangent;
    vert2.bitangent = bitangent;
    vert3.bitangent = bitangent;
}
