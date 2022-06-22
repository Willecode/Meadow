#include "application.h"
// DEBUG --------------------
#include "scene/directionallight.h"
#include "scene/pointlight.h"
#include "resource_management/modelimporting.h"
//---------------------------
Application::Application(): m_windowManager(), m_ui(), m_inputGather(), m_renderer(OpenGLRenderer()), m_logger(Logger()), m_shaderManager(), m_scene(nullptr), appFailed(false), m_UIScraper()
{   

    /*
    * Provide service locator the addresses of the items it needs to locate
    */
    m_windowManager.createWindow("Meadow");
    appFailed = m_renderer.initialize(&m_windowManager);
    m_logger.init();
    Locator::provide(&m_logger);
    //Locator::provide(&m_windowManager);
    Locator::provide(&m_renderer);

    /*
    * Initialize ui
    */
    m_ui.init(&m_windowManager);

    /*
    * Input gathering needs to locate logger and windowman so lazy init it here
    */
    m_inputGather.init(&m_windowManager);
    

    /*
    * Get a resourceman instance
    */
    ResourceManager manager = ResourceManager::getInstance();
    
    /*
    * Provide shaderman some shaders
    */
    auto phongSdr      = std::make_unique<Shader>(0, "shaders/object.vs", "shaders/phongtex.fs");
    auto colorSdr      = std::make_unique<Shader>(1, "shaders/object.vs", "shaders/coloronly.fs");
    auto depthSdr      = std::make_unique<Shader>(2, "shaders/object.vs", "shaders/depth.fs");
    auto screenQuadSdr = std::make_unique<Shader>(3, "shaders/2d.vs"    , "shaders/postprocessing.fs");
    m_shaderManager.provideShader("phong", std::move(phongSdr));
    m_shaderManager.provideShader("color", std::move(colorSdr));
    m_shaderManager.provideShader("depth", std::move(depthSdr));
    m_shaderManager.provideShader("postprocess", std::move(screenQuadSdr));
    m_shaderManager.setCurrentShader("phong"); // Set current shader to prevent nullptr

    /*
    * Create a scene for entities
    */
    m_scene = std::make_unique<Scene>();

    /*
    * Add some stuff into the scene
    */
#if 1

    /*
    * Add one node with all sorts of stuff in it
    */
    unsigned int nodeId = m_scene->addNode();
    SceneNode* node = m_scene->getNode(nodeId);
    node->name = "Cube";
    std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>("Cube");
    unsigned int newMeshId = manager.storeMesh(std::move(newMesh));
    Mesh* newMeshPtr = manager.getMesh(newMeshId);
    /*
    * create materials and store them
    */
    auto mat = std::make_unique<Material>("Woodblock");
    mat->defaultPhong();
    auto matid = manager.storeMaterial(std::move(mat));
    auto mat2 = manager.getMaterial(matid);
    
    auto mat3 = std::make_unique<Material>("Bricks");
    mat3->defaultPhong();
    matid = manager.storeMaterial(std::move(mat3));
    auto mat4 = manager.getMaterial(matid);

    auto m = PrimitiveCreation::createCubeMesh();
    auto smeshid = manager.storeSubmesh(std::move(m));
    SubMesh* smesh = manager.getSubmesh(smeshid);

    newMeshPtr->addSubMesh(mat2, smesh);

    node->setMesh(newMeshPtr);

    Camera c(1920.0f / 1080.0f, 0.1f, 100.0f);
    auto renderer = Locator::getRenderer();
    unsigned int sdrId = m_shaderManager.getCurrentShader()->getId();

    ImageLoader loader;
    Renderer::ImageFormat fmt1;
    int width1, height1;
    auto vecptr1 = std::make_unique<std::vector<unsigned char>>();
    Renderer::ImageFormat fmt2;
    int width2, height2;
    auto vecptr2 = std::make_unique<std::vector<unsigned char>>();

    loader.loadImage("C:/dev/Meadow/data/images/Wood066_1K_Color.jpg", width1, height1, fmt1, *vecptr1.get());
    loader.loadImage("C:/dev/Meadow/data/images/Bricks054_1K_Color.jpg", width2, height2, fmt2, *vecptr2.get());

    auto texPtr = std::make_unique<Texture>(std::move(vecptr1), width1, height1, fmt1, Renderer::ImageFormat::RGB, "WoodTex");
    auto texPtr2 = std::make_unique<Texture>(std::move(vecptr2), width2, height2, fmt2, Renderer::ImageFormat::RGB, "BrickTex");

    unsigned int texId = manager.storeTexture(std::move(texPtr));
    unsigned int texId2 = manager.storeTexture(std::move(texPtr2));

    Texture* tex = manager.getTexture(texId);
    Texture* tex2 = manager.getTexture(texId2);

    mat2->setTexture(tex, Texture::TextureType::DIFFUSE_MAP);
    mat4->setTexture(tex2, Texture::TextureType::DIFFUSE_MAP);


    /*
    * Add some transform to the node
    */
    node->scale = glm::vec3(4.0f, 4.0f, 1.0f);
    node->position = glm::vec3(0.0f, 2.0f, -1.0f);
    /*
    * Add another node
    */
    unsigned int node2Id = m_scene->addNode();
    SceneNode* node2 = m_scene->getNode(node2Id);
    node2->name = "Sphere";

    auto m2 = PrimitiveCreation::createSphere(15, 20);
    unsigned int mesh2id = manager.storeSubmesh(std::move(m2));
    SubMesh* mesh2 = manager.getSubmesh(mesh2id);
    
    unsigned int meshNode2id = manager.storeMesh(std::make_unique<Mesh>("Sphere"));
    Mesh* meshNode2 = manager.getMesh(meshNode2id);
    node2->setMesh(meshNode2);

    node2->getMesh()->addSubMesh(mat4, mesh2);
    /*
    * Transform the second node 
    */
    node2->position = glm::vec3(0.f);
    node2->scale = glm::vec3(3.0f, 0.1f, 3.0f);
    
    /*
    * Add third node and mesh and mat
    */
    unsigned int thirdMaterialID = manager.storeMaterial(std::make_unique<Material>("Color Material"));
    Material* thirdMaterial = manager.getMaterial(thirdMaterialID);
    thirdMaterial->defaultPhong();

    unsigned int sphereSubmeshID = manager.storeSubmesh(std::move(PrimitiveCreation::createSphere(30, 50)));
    SubMesh* sphereSubmesh = manager.getSubmesh(sphereSubmeshID);
    unsigned int thirdNodeID = m_scene->addNode();
    SceneNode* thirdNode = m_scene->getNode(thirdNodeID);
    unsigned int thirdMeshID = manager.storeMesh(std::move(std::make_unique<Mesh>("Lamp")));
    Mesh* thirdMesh = manager.getMesh(thirdMeshID);
    thirdMesh->addSubMesh(thirdMaterial, sphereSubmesh);
    thirdNode->setMesh(thirdMesh);
    thirdNode->scale = glm::vec3(0.2f);
    thirdNode->position = glm::vec3(0.9f, 2.5f, 1.5f);
    thirdNode->name = "LampNode";

    

    /*
    * Add some light
    */
    if (!DirectionalLight::maxInstanceCapacity()) {
        auto dirLight = std::make_unique<DirectionalLight>();
        m_scene->getNode(0)->setLightSource(std::move(dirLight));
    }
    if (!PointLight::maxInstanceCapacity()) {
        auto pointLight = std::make_unique<PointLight>();
        m_scene->getNode(thirdNodeID)->setLightSource(std::move(pointLight));
    }

    /*
    * import a model
    */
    ModelImporting::objsFromFile("C:/dev/Meadow/data/3dmodels/old-office-window/source/office window.fbx", m_scene.get(), 0);
    ModelImporting::objsFromFile("C:/dev/Meadow/data/3dmodels/gooby/only_LP_FIXING_MESH_FOR_BETTER_BAKING.obj", m_scene.get(), 0);

    //m_scene->getNode(4)->scale = glm::vec3(0.2f);
#endif
}

void Application::run()
{
    // Update loop
    // -----------
    float deltatime;
    float time;
    float lastFrameTime = 0.f;

    /*
    * Create texture to store render
    */
    Texture tex(m_windowManager.width, m_windowManager.height, "");
    tex.setId(10000);
    tex.loadToGPU();

    /*
    * Create framebuffer
    */
    m_renderer.createFrameBuffer(0, tex.getId(), tex.getWidth(), tex.getHeight());
    m_renderer.bindFrameBuffer(0);
    if (!m_renderer.checkFrameBufferStatus())
        Locator::getLogger()->getLogger()->error("Application: framebuffer not complete");

    /*
    * Create screen quad
    */
    ResourceManager manager = ResourceManager::getInstance();
    unsigned int screenQuad = manager.storeMesh2D(std::move(PrimitiveCreation::createScreenQuad()));
    Mesh2D* quadptr = manager.getMesh2D(screenQuad);
    manager.getMesh2D(screenQuad)->setTexture(&tex);

    while (!m_windowManager.shouldClose())
    {
        /*
        * Calculate deltatime
        */
        time = m_windowManager.getTime();
        deltatime = time - lastFrameTime;
        lastFrameTime = time;
        /*
        * Poll events
        */
        m_windowManager.pollEvents();

        /*
        * Update scene
        */
        m_scene->update(deltatime, &m_inputGather);

        /*
        * Render scene 
        */
        m_renderer.depthTesting(true);
        m_renderer.blending(true);
        m_renderer.bindFrameBuffer(0);
        m_shaderManager.setCurrentShader("phong");
        m_renderer.clearBuffer(m_renderer.getColorBuffBit() | m_renderer.getDepthBuffBit() | m_renderer.getStencilBuffBit());
        m_scene->render(&m_shaderManager);

        /*
        * Do postprocessing
        */
        m_renderer.depthTesting(false);
        m_renderer.blending(false);
        m_renderer.bindFrameBufferDefault();
        m_shaderManager.setCurrentShader("postprocess");
        manager.getMesh2D(screenQuad)->draw(&m_shaderManager);

        /*
        * Render UI
        */
        m_UIScraper.update(m_scene.get());
        m_ui.renderInterface(m_UIScraper.getUINodeGraph(), m_UIScraper.getUIAssets());

        m_windowManager.swapBuffers();
    }

    glfwTerminate();
    /////////////////////
    // Update loop sketching
    /////////////////////
    // 
    // rendering
        /* Clear framebuffer */
        /* Get all objects that glow */
        /* Set up glow shader */
        /* Render */
        /* Get all objects that are lit */
        /* Set up light shader */
        /* Render */
    // Scrape Scene data for UI
    // Provide scene data to UI
    // Render UI
    /////////////////////
}
