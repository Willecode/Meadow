#include "primitivecreation.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtx/string_cast.hpp>
#include "service_locator/locator.h"

std::unique_ptr<SubMesh> PrimitiveCreation::createCubeMesh()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
                                       //pos                           //normal                       //texCoord
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
    Locator::getLogger()->getLogger()->info("created a sphere with {} vertices", vertices.size());
    return std::make_unique<SubMesh>(vertices, indices, "Sphere");
}
