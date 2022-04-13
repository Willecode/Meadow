#include "application.h"
#include "resource_management/imageloader.h"

//--------------------------------------
// IMAGE LOADER
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>
//--------------------------------------
// MATH FUNCTIONS
//#define GLM_ENABLE_EXPERIMENTAL // for glm/ext.hpp
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/ext.hpp> // For printing glm vectors
//--------------------------------------
// STRING FORMATTING
//#define FMT_HEADER_ONLY
//#include <fmt/core.h>
//--------------------------------------
#include "resource_management/shadermanager.h"
#include "resource_management/resourcemanager.h"
#include "service_locator/locator.h"

/*
* Debug includes
* ******************/
#include "primitivecreation.h"
#include "scene/camera.h"
#include "assets/texture.h"
//*******************
Application::Application(): m_windowManager(), m_inputGather(), m_dispatcher(), m_renderer(OpenGLRenderer()), m_logger(Logger()), m_shaderManager(), m_scene(nullptr), appFailed(false)
{   

    /*
    * Provide service locator the addresses of the items it needs to locate
    */
    m_windowManager.createWindow("Meadow", &m_dispatcher);
    appFailed = m_renderer.initialize();
    m_logger.init();
    Locator::provide(&m_logger);
    Locator::provide(&m_windowManager);
    Locator::provide(&m_renderer);

    /*
    * Input gathering needs to locate logger and windowman so lazy init it here
    */
    m_inputGather.init(&m_dispatcher);
    

    /*
    * Get a resourceman instance
    */
    ResourceManager manager = ResourceManager::getInstance();
    
    /*
    * Provide shaderman some shaders
    */
    auto phongSdr = std::make_unique<Shader>(0, "shaders/object.vs", "shaders/phongtex.fs");
    auto colorSdr = std::make_unique<Shader>(1, "shaders/object.vs", "shaders/coloronly.fs");
    m_shaderManager.provideShader("phong", std::move(phongSdr));
    m_shaderManager.provideShader("color", std::move(colorSdr));
    m_shaderManager.setCurrentShader("color"); // Set current shader to prevent nullptr
    /*
    * Create a scene for entities
    */
    m_scene = std::make_unique<Scene>(&m_dispatcher);

    /*
    * Add some stuff into the scene
    */
#if 1

    /*
    * Add one node with all sorts of stuff in it
    */
    unsigned int nodeId = m_scene->addNode();
    SceneNode* node = m_scene->getNode(nodeId);
    std::unique_ptr<Model> model = std::make_unique<Model>();
    
    auto mat = std::make_unique<Material>();
    auto matid = manager.storeMaterial(std::move(mat));
    auto mat2 = manager.getMaterial(matid);

    mat2->setProperty("color", glm::vec3(.0f, 1.f, 0.f));

    auto m = PrimitiveCreation::createCubeMesh();
    m->setId(1);

    unsigned int meshid = manager.storeMesh(std::move(m));
    Mesh* mesh = manager.getMesh(meshid);

    model->setMaterial(mat2);
    model->addMesh(mesh);
    node->setModel(std::move(model));

    Camera c(1920.0f / 1080.0f, 0.1f, 100.0f);
    auto renderer = Locator::getRenderer();
    unsigned int sdrId = m_shaderManager.getCurrentShader()->getId();

    ImageLoader loader;
    ImageData imgdata = loader.loadImage("C:/dev/Meadow/data/images/Wood066_1K_Color.jpg");
    
    auto vecptr = std::make_unique<std::vector<unsigned char>>();
    for (int i = 0; i < imgdata.width * imgdata.height * imgdata.nrChannels; i++) {
        vecptr->push_back(imgdata.bytes[i]);
    }
    Renderer::ImageFormat imgForm;
    if (imgdata.nrChannels = 3) {
        imgForm = Renderer::ImageFormat::RGB;
    }
    else if (imgdata.nrChannels = 4) {
        imgForm = Renderer::ImageFormat::RGBA;
    }
    auto texPtr = std::make_unique<Texture>(std::move(vecptr), imgdata.width, imgdata.height, imgForm);
    unsigned int texId = manager.storeTexture(std::move(texPtr));
    Texture* tex = manager.getTexture(texId);
    mat2->setTexture(tex, Texture::TextureType::DIFFUSE_MAP);

    /*
    * Add some transform to the node
    */
    node->scale = glm::vec3(0.1f);
    node->position = glm::vec3(1.0f, 0.0f, 0.0f);
    /*
    * Add another node
    */
    unsigned int node2Id = m_scene->addNode();
    SceneNode* node2 = m_scene->getNode(node2Id);
    (*node2) = *node;
    /*
    * Transform the second node 
    */
    node2->position = glm::vec3(0.f);
    

#endif
}

void Application::run()
{
    // Update loop
    // -----------
    float deltatime;
    float time;
    float lastFrameTime = 0.f;

    while (!m_windowManager.shouldClose())
    {
        /*
        * Calculate deltatime
        */
        time = m_windowManager.getTime();
        deltatime = time - lastFrameTime;
        lastFrameTime = time;
        m_windowManager.pollEvents();
        /* Clear framebuffer */
        /* Get all objects that glow */
        /* Set up glow shader */
        /* Render */
        /* Get all objects that are lit */
        /* Set up light shader */
        /* Render */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_scene->update(deltatime, &m_inputGather);
        m_scene->render(&m_shaderManager);

        m_windowManager.swapBuffers();
    }

    glfwTerminate();
}
