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
#include "camera.h"
#include "texture.h"
//*******************
Application::Application(): m_windowManager(WindowManager()), m_renderer(OpenGLRenderer()), m_logger(Logger()), m_shaderManager(), m_scene(nullptr), appFailed(false)
{   

    /*
    * Provide service locator the addresses of the items it needs to locate
    */
    m_windowManager.createWindow("Meadow");
    appFailed = m_renderer.initialize();
    m_logger.init();
    Locator::provide(&m_logger);
    Locator::provide(&m_windowManager);
    Locator::provide(&m_renderer);

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
    m_scene = std::make_unique<Scene>();

    
#if 1

    //********************************** Stuff under this worked with the mahogany box
    unsigned int nodeId = m_scene->addNode();
    SceneNode* node = m_scene->getNode(nodeId);
    std::unique_ptr<Model> model = std::make_unique<Model>();
    
    auto mat = std::make_unique<Material>();
    auto matid = manager.storeMaterial(std::move(mat));
    auto mat2 = manager.getMaterial(matid);

    mat2->setProperty("color", glm::vec3(.0f, 1.f, 0.f));

    auto m = PrimitiveCreation::createCubeMesh();
    m->setId(1);
    m->generateBuffers();
    m->buffersPushData();
    unsigned int meshid = manager.storeMesh(std::move(m));
    Mesh* mesh = manager.getMesh(meshid);

    model->setMaterial(mat2);
    model->addMesh(mesh);
    node->setModel(std::move(model));

    Camera c(1920.0f / 1080.0f, 0.1f, 100.0f);
    auto renderer = Locator::getRenderer();
    //renderer->useShaderProgram(sdrId);
    unsigned int sdrId = m_shaderManager.getCurrentShader()->getId();
    /*renderer->setMat4f(sdrId, "view", c.getViewMatrix());
    renderer->setMat4f(sdrId, "projection", c.getProjectionMatrix());
    renderer->setMat4f(sdrId, "model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f)));*/
    m_shaderManager.setFrameUniform("view", c.getViewMatrix());
    m_shaderManager.setFrameUniform("projection", c.getProjectionMatrix());
    //m_shaderManager.setFrameUniform("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f)));
    //renderer->useShaderProgram(sdrId);

#if 0
    //ColorOnlyMaterial mat(glm::vec3(1.0f, 0.0f, 0.0f));
    //mat.passToRenderer();
    ImageLoader loader;
    ImageData imgdata = loader.loadImage("C:/dev/Meadow/data/images/Wood066_1K_Color.jpg");

    /*
    * Copy the bytes from imgdata (for science)
    */
    /*unsigned char arr[300 * 300 * 3];
    memcpy(arr, imgdata->dataPtr, imgdata->width * imgdata->height * 3);*/
    
    /*
    * converting to std::vector 
    */
    auto vecptr = std::make_unique<std::vector<unsigned char>>();
    for (int i = 0; i < imgdata.width * imgdata.height * imgdata.nrChannels; i++) {
        vecptr->push_back(imgdata.bytes[i]);
    }

    /*renderer->create2DTexture(1, imgdata->width, imgdata->height, Renderer::ImageFormat::RGB, &vec[0]);
    renderer->bindTo2DSampler(1, 0);*/
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
    tex->loadToGPU();
    tex->bindToSampler(0);

    renderer->setInt(sdrId, "texture_map", 0);


#endif
#endif
}

void Application::run()
{
    //m_scene->updateLighting();
    //m_scene->updateShaders();
    
    // render loop
    // -----------
    while (!m_windowManager.shouldClose())
    {
        /* Clear framebuffer */
        /* Get all objects that glow */
        /* Set up glow shader */
        /* Render */
        /* Get all objects that are lit */
        /* Set up light shader */
        /* Render */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_shaderManager.forwardFrameUniforms();
        m_scene->update(&m_shaderManager);
        //m->draw();
        //Locator::getRenderer()->setMat4f(sdrId, "model", glm::rotate(glm::mat4(1.0f),(float)glfwGetTime(), glm::vec3(0.f,1.f,0.f)));
        // glfw: swap buffers and poll IO events
        // -------------------------------------------------------------------------------
        m_windowManager.swapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
}
