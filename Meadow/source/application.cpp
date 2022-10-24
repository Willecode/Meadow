#include "application.h"
#include "renderer/renderingutils.h"

// ECS ----------------------
#include "ecs/components/transform.h"
#include "ecs/components/model3d.h"
#include "ecs/components/camera.h"
#include "ecs/components/light.h"
#include "ecs/components/rigidbody.h"
#include "benchmarking/acomponent.h"
#include "benchmarking/bcomponent.h"

//---------------------------
// DEBUG --------------------
#include "scene/directionallight.h"
#include "scene/pointlight.h"
#include "resource_management/modelimporting.h"
#include "assets/materials/PBRMaterial.h"
#include "assets/materials/colormaterial.h"
//---------------------------
// BENCHMARK ----------------
#include "benchmarking/benchmarkutils.h"
//---------------------------


Application::Application() :
    m_ui(),
    m_inputGather(),
    m_renderer(OpenGLRenderer()),
    m_logger(Logger()),
    appFailed(false),
    m_UIScraper(),
    m_postProcessing(PostProcessing()),
    m_importer(),
    m_ecs(),
    m_componentEventHandler(),
    m_cameraSystem(nullptr),
    m_renderSystem(nullptr),
    m_sceneGraphSystem(nullptr),
    m_lightSystem(nullptr),
    m_physicsSystem(nullptr)
{   

    /*
    * Get a windowmanager instance
    */
    WindowManager& windowMan = WindowManager::getInstance();

    /*
    * Provide service locator the addresses of the items it needs to locate
    */
    windowMan.createWindow("Meadow");
    appFailed = m_renderer.initialize(&windowMan);
    m_logger.init();
    LoggerLocator::provide(&m_logger);
    RendererLocator::provide(&m_renderer);

    /*
    * Initialize ui
    */
    m_ui.init(&windowMan);

    /*
    * Input gathering needs to locate logger and windowman so lazy init it here
    */
    m_inputGather.init(&windowMan);
    

    /*
    * Get a resourceman instance
    */
    ResourceManager& manager = ResourceManager::getInstance();
    
    /*
    * Init shadermanager
    */
    ShaderManager::getInstance().init();

    /*
    * Initialize postprocessing
    */
    m_postProcessing.init();

    /*
    * Provide importer a scene to import files to
    */
    m_importer.init(&m_ecs);

    //ECS
    m_ecs.init();
    registerComponents();
    initSystems();
    m_componentEventHandler.init(&m_ecs);
    createDefaultScene();

}

void Application::run()
{
    float deltatime;
    float time;
    float lastFrameTime = 0.f;

    ResourceManager& manager = ResourceManager::getInstance();
    WindowManager& windowMan = WindowManager::getInstance();
    ShaderManager& sdrMan = ShaderManager::getInstance();
    /////////////////////////
    // Intermediate fb to blit to
    /////////////////////////
    /*
    * Create texture to store render
    */
    std::unique_ptr<Texture> texptr = std::make_unique<Texture>(windowMan.width, windowMan.height, false, "normal pass", false);
    unsigned int texId = manager.storeTexture(std::move(texptr));
    Texture* tex = manager.getTexture(texId);

    /*
    * Create framebuffer
    */
    m_renderer.createFrameBuffer(0, tex->getId(), tex->getWidth(), tex->getHeight());
    m_renderer.bindFrameBuffer(0);
    if (!m_renderer.checkFrameBufferStatus())
        LoggerLocator::getLogger()->getLogger()->error("Application: framebuffer not complete");

    /////////////////////////
    // Multisampled fb
    /////////////////////////
    /*
    * Create texture to store render
    */
    std::unique_ptr<Texture> texMSptr = std::make_unique<Texture>(windowMan.width, windowMan.height, true, "MS pass", false);
    unsigned int texMSId = manager.storeTexture(std::move(texMSptr));
    Texture* texMS = manager.getTexture(texMSId);
    /*
    * Create framebuffer
    */
    m_renderer.createFrameBufferMultisample(1,texMS->getId(), texMS->getWidth(), texMS->getHeight()); // think this again
    m_renderer.bindFrameBuffer(1);
    if (!m_renderer.checkFrameBufferStatus())
        LoggerLocator::getLogger()->getLogger()->error("Application: MS framebuffer not complete");

    /*
    * Create screen quad
    */
    unsigned int screenQuad = manager.storeMesh2D(std::move(PrimitiveCreation::createScreenQuad()));
    Mesh2D* quadptr = manager.getMesh2D(screenQuad);
    manager.getMesh2D(screenQuad)->setTexture(tex);

    /*
    * Create Skybox
    */
    ImageLoader loader;
    loader.flipOnLoad(false);
    int width, height;
    Renderer::ImageFormat fmt;

    std::array<std::unique_ptr<std::vector<unsigned char>>, 6> texArr;
    
    for (int i = 0; i < 6; i++) {
        auto vecptr = std::make_unique<std::vector<unsigned char>>();

        if (i == 0)
            loader.loadImage("C:/dev/Meadow/data/images/cloudy/bluecloud_rt.jpg", width, height, fmt, *vecptr.get());
        if (i == 1)
            loader.loadImage("C:/dev/Meadow/data/images/cloudy/bluecloud_lf.jpg", width, height, fmt, *vecptr.get());
        if (i == 2)
            loader.loadImage("C:/dev/Meadow/data/images/cloudy/bluecloud_up.jpg", width, height, fmt, *vecptr.get());
        if (i == 3)
            loader.loadImage("C:/dev/Meadow/data/images/cloudy/bluecloud_dn.jpg", width, height, fmt, *vecptr.get());
        if (i == 4)
            loader.loadImage("C:/dev/Meadow/data/images/cloudy/bluecloud_bk.jpg", width, height, fmt, *vecptr.get());
        if (i == 5)
            loader.loadImage("C:/dev/Meadow/data/images/cloudy/bluecloud_ft.jpg", width, height, fmt, *vecptr.get());

        texArr[i] = std::move(vecptr);
    }


    auto skybox = std::make_unique<Cubemap>(std::move(texArr), width, height);
    unsigned int skyboxId = manager.storecubemap(std::move(skybox));

    /*
    * Enable stencil testing
    */
    m_renderer.stencilTesting(true);
    /*m_renderer.setStencilClearValue(1);*/

    // Benchmarking
    std::array<std::chrono::time_point<std::chrono::steady_clock>, 14> times;
    bool renderUI = true;
    // Update loop
    // -----------
    while (!windowMan.shouldClose())
    {
        times[0] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------

        /*
        * Calculate deltatime
        */
        time = windowMan.getTime();
        deltatime = time - lastFrameTime;
        lastFrameTime = time;
        /*
        * Poll events
        */
        windowMan.pollEvents();
        m_inputGather.pollInputs();

        /*
        * Bind MSAA buffer or basic buffer
        */
        if (m_postProcessing.getMSAA())
            m_renderer.bindFrameBuffer(1);
        else
            m_renderer.bindFrameBuffer(0);
        
        /*
        * Set renderer viewport dimensions to match the framebuffer
        */
        int fbWidth, fbHeight;
        m_renderer.getFrameBufferDimensions(0, fbWidth, fbHeight);
        m_renderer.setViewportSize(fbWidth, fbHeight);

        /*
        * Clear buffers
        */
        m_renderer.setStencilClearValue(1); // clear stencil buffer with ones
        m_renderer.setStencilMask(1); // allow stencil buffer writing
        m_renderer.clearBuffer(m_renderer.getColorBuffBit() | m_renderer.getDepthBuffBit() | m_renderer.getStencilBuffBit());

        /*
        * Render active node highlighting
        */
        if (m_sceneGraphSystem->getActiveNode() != NullEntity)
        {
            //m_shaderManager.setCurrentShader("phong");
            sdrMan.bindShader(ShaderManager::ShaderType::PBR);
            RenderingUtils::maskMeshOutlines(m_sceneGraphSystem->getActiveNode(), m_ecs);
        }
            
        /*
        * Allow color rendering and disable rendering on stencil buffer's mask of 0's
        */
        m_renderer.setColorMask(true);
        m_renderer.setStencilFunc(Renderer::TestingFuncs::EQUAL, 1, 0xFF);
        m_renderer.setStencilOp(Renderer::TestingActions::KEEP, Renderer::TestingActions::KEEP, Renderer::TestingActions::REPLACE);

        times[1] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------
        m_sceneGraphSystem->update();
        times[2] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------
        m_lightSystem->update(deltatime);
        times[3] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------
        m_cameraSystem->update(deltatime, m_inputGather);
        times[4] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------
        m_physicsSystem->update(deltatime);
        times[5] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------
        
        // Benchmark system
        m_benchmarkSystem->update(deltatime);
        times[6] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------

        /*
        * Render skybox
        */
        sdrMan.bindShader(ShaderManager::ShaderType::SKYBOX);
        m_renderer.depthMask(false);
        m_renderer.depthTesting(false);
        manager.getcubemap(skyboxId)->draw();

        times[7] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------

        /*
        * Render scene 
        */
        m_renderer.depthTesting(true);
        m_renderer.blending(true);
        m_renderer.depthMask(true);
        sdrMan.bindShader(ShaderManager::ShaderType::PBR);
        //m_scene->render();
        m_renderSystem->update(deltatime);
        times[8] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------

        /*
        * Render colliders
        */
        m_renderer.depthTesting(false);
        m_renderer.wireframe(true);
        sdrMan.bindShader(ShaderManager::ShaderType::COLLIDER);
        m_physicsSystem->drawColliders();
        times[9] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------

        /*
        * If MSAA on then blit to intermediate frame buffer
        */
        if (m_postProcessing.getMSAA()) {
            m_renderer.bindFrameBufferRead(1);
            m_renderer.bindFrameBufferDraw(0);
            m_renderer.blitFramebuffer(texMS->getWidth(), texMS->getHeight());
        }
        times[10] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------


        /*
        * Set default framebuffer to render on screen
        */
        m_renderer.depthTesting(false);
        m_renderer.blending(false);
        m_renderer.bindFrameBufferDefault();
        m_renderer.clearBuffer(m_renderer.getColorBuffBit() | m_renderer.getDepthBuffBit() | m_renderer.getStencilBuffBit());

        /*
        * Do postprocessing pass
        */
        sdrMan.bindShader(ShaderManager::ShaderType::POSTPROCESS);
        m_renderer.setViewportSize(windowMan.width, windowMan.height);
        sdrMan.forwardFrameUniforms();
        manager.getMesh2D(screenQuad)->draw();
        times[11] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------

        /*
        * Render UI
        */
        if (renderUI)
            m_UIScraper.update(m_sceneGraphSystem->getSceneGraph(), &m_postProcessing, m_ecs);
        times[12] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------
        if (renderUI)
            m_ui.renderInterface(m_UIScraper.getSceneGraph(), m_UIScraper.getUIAssets(), m_UIScraper.getPostprocessingFlags(), m_UIScraper.getComponentMap());
        times[13] = std::chrono::steady_clock::now(); // TIME MEASURE --------------------------------------------------------

        /*
        * Process event queue
        */
        InputEvents::EventQueue::processQueue();

        windowMan.swapBuffers();

        LoggerLocator::getLogger()->getLogger()->debug("Benchmark times:");
        std::chrono::duration<double> diff = times.back() - times[0];

        LoggerLocator::getLogger()->getLogger()->debug("Frame time: {}", diff.count());
        LoggerLocator::getLogger()->getLogger()->debug("FPS: {}", 1.0f / diff.count());
        for (int i = 1; i < times.size(); i++) {
            std::chrono::duration<double> diff = times[i] - times[i-1];
            LoggerLocator::getLogger()->getLogger()->debug("Time {}: {}", i, diff.count());
        }
    }

}

void Application::initSystems()
{
    m_renderSystem = m_ecs.registerSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(m_ecs.getComponentType<Model3D>());
        signature.set(m_ecs.getComponentType<Transform>());
        m_ecs.setSystemSignature<RenderSystem>(signature);
    }
    m_renderSystem->init(&m_ecs);
    // -------------------------------------------------------------
    m_cameraSystem = m_ecs.registerSystem<CameraSystem>();
    {
        Signature signature;
        signature.set(m_ecs.getComponentType<Camera>());
        signature.set(m_ecs.getComponentType<Transform>());
        m_ecs.setSystemSignature<CameraSystem>(signature);
    }
    m_cameraSystem->init(&m_ecs);
    // -------------------------------------------------------------
    m_sceneGraphSystem = m_ecs.registerSystem<SceneGraphSystem>();
    {
        Signature signature;
        signature.set(m_ecs.getComponentType<Transform>());
        m_ecs.setSystemSignature<SceneGraphSystem>(signature);
    }
    m_sceneGraphSystem->init(&m_ecs);
    // -------------------------------------------------------------
    m_lightSystem = m_ecs.registerSystem<LightSystem>();
    {
        Signature signature;
        signature.set(m_ecs.getComponentType<Transform>());
        signature.set(m_ecs.getComponentType<Light>());
        m_ecs.setSystemSignature<LightSystem>(signature);
    }
    m_lightSystem->init(&m_ecs);
    // -------------------------------------------------------------
    m_physicsSystem = m_ecs.registerSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(m_ecs.getComponentType<Transform>());
        signature.set(m_ecs.getComponentType<RigidBody>());
        m_ecs.setSystemSignature<PhysicsSystem>(signature);
    }
    m_physicsSystem->init(&m_ecs);
     // -------------------------------------------------------------
    m_benchmarkSystem = m_ecs.registerSystem<BenchmarkSystem>();
    {
        Signature signature;
        signature.set(m_ecs.getComponentType<AComponent>());
        m_ecs.setSystemSignature<BenchmarkSystem>(signature);
    }
    m_benchmarkSystem->init(&m_ecs);

}

void Application::registerComponents()
{
    m_ecs.registerComponent<Model3D>();
    m_ecs.registerComponent<Camera>();
    m_ecs.registerComponent<Light>();
    m_ecs.registerComponent<RigidBody>();
    m_ecs.registerComponent<AComponent>();
    m_ecs.registerComponent<BComponent>();
}

void Application::createDefaultScene()
{
    {

        /*Camera cam;
        cam.fov = 45.f;
        cam.zNear = 0.1f;
        cam.zFar = 100.f;
        m_ecs.addComponent(entity, cam);*/

        ResourceManager& manager = ResourceManager::getInstance();

        // Create material 1
        Material* mat;
        {
            auto temp = std::make_unique<PBRMaterial>("PBR 1");
            mat = manager.getMaterial(manager.storeMaterial(std::move(temp)));
        }
        // Create material 2
        Material* mat2;
        {
            auto temp = std::make_unique<PBRMaterial>("PBR 2");
            mat2 = manager.getMaterial(manager.storeMaterial(std::move(temp)));
        }
        // Create material 3
        Material* mat3;
        {
            auto temp = std::make_unique<ColorMaterial>("Color");
            mat3 = manager.getMaterial(manager.storeMaterial(std::move(temp)));
        }
        // Create material 4
        Material* mat4;
        {
            auto temp = std::make_unique<PBRMaterial>("Floor");
            mat4 = manager.getMaterial(manager.storeMaterial(std::move(temp)));
        }
        /*
       * Add some textures to the materials
       */
        ImageLoader loader;
        auto vecptr = std::make_unique<std::vector<unsigned char>>();
        struct Tex {
            std::string path;
            Renderer::ImageFormat fmt;
        };

        // Material 1 textures
        {
            Tex t1;
            t1.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_basecolor.png";
            t1.fmt = Renderer::ImageFormat::sRGB;
            Tex t2;
            t2.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_metallic.png";
            t2.fmt = Renderer::ImageFormat::R;
            Tex t3;
            t3.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_normal.png";
            t3.fmt = Renderer::ImageFormat::RGB;
            Tex t4;
            t4.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_roughness.png";
            t4.fmt = Renderer::ImageFormat::R;

            std::vector<Tex> texVec = { t1, t2, t3, t4 };
            std::vector<unsigned int> texIdVec;
            int w, h;
            Renderer::ImageFormat inFmt;
            for (auto& t : texVec) {
                loader.loadImage(t.path, w, h, inFmt, *vecptr.get());
                auto texPtr = std::make_unique<Texture>(std::move(vecptr), w, h, inFmt, t.fmt, t.path);
                unsigned int texId = manager.storeTexture(std::move(texPtr));
                texIdVec.push_back(texId);
                vecptr = std::make_unique<std::vector<unsigned char>>();
            }

            mat->setTexture(manager.getTexture(texIdVec[0]), Texture::TextureType::ALBEDO_MAP);
            mat->setTexture(manager.getTexture(texIdVec[1]), Texture::TextureType::METALLIC_MAP);
            mat->setTexture(manager.getTexture(texIdVec[2]), Texture::TextureType::NORMAL_MAP);
            mat->setTexture(manager.getTexture(texIdVec[3]), Texture::TextureType::ROUGHNESS_MAP);
        }
        // Material 2 textures
        {
            Tex t1;
            t1.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_albedo.png";
            t1.fmt = Renderer::ImageFormat::sRGB;
            Tex t2;
            t2.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_metallic.png";
            t2.fmt = Renderer::ImageFormat::R;
            Tex t3;
            t3.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_normal-ogl.png";
            t3.fmt = Renderer::ImageFormat::RGB;
            Tex t4;
            t4.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_roughness.png";
            t4.fmt = Renderer::ImageFormat::R;
            Tex t5;
            t5.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_ao.png";
            t5.fmt = Renderer::ImageFormat::R;

            std::vector<Tex> texVec = { t1, t2, t3, t4, t5 };
            std::vector<unsigned int> texIdVec;
            int w, h;
            Renderer::ImageFormat inFmt;
            for (auto& t : texVec) {
                loader.loadImage(t.path, w, h, inFmt, *vecptr.get());
                auto texPtr = std::make_unique<Texture>(std::move(vecptr), w, h, inFmt, t.fmt, t.path);
                unsigned int texId = manager.storeTexture(std::move(texPtr));
                texIdVec.push_back(texId);
                vecptr = std::make_unique<std::vector<unsigned char>>();
            }

            mat2->setTexture(manager.getTexture(texIdVec[0]), Texture::TextureType::ALBEDO_MAP);
            mat2->setTexture(manager.getTexture(texIdVec[1]), Texture::TextureType::METALLIC_MAP);
            mat2->setTexture(manager.getTexture(texIdVec[2]), Texture::TextureType::NORMAL_MAP);
            mat2->setTexture(manager.getTexture(texIdVec[3]), Texture::TextureType::ROUGHNESS_MAP);
            mat2->setTexture(manager.getTexture(texIdVec[4]), Texture::TextureType::AO_MAP);
        }
        // Material 4 textures
        {
            Tex t1;
            t1.path = "C:/dev/Meadow/data/images/red-scifi-metal-bl/red-scifi-metal-bl/red-scifi-metal_albedo.png";
            t1.fmt = Renderer::ImageFormat::sRGB;
            Tex t2;
            t2.path = "C:/dev/Meadow/data/images/red-scifi-metal-bl/red-scifi-metal-bl/red-scifi-metal_metallic.png";
            t2.fmt = Renderer::ImageFormat::R;
            Tex t3;
            t3.path = "C:/dev/Meadow/data/images/red-scifi-metal-bl/red-scifi-metal-bl/red-scifi-metal_normal-ogl.png";
            t3.fmt = Renderer::ImageFormat::RGB;
            Tex t4;
            t4.path = "C:/dev/Meadow/data/images/red-scifi-metal-bl/red-scifi-metal-bl/red-scifi-metal_roughness.png";
            t4.fmt = Renderer::ImageFormat::R;
            Tex t5;
            t5.path = "C:/dev/Meadow/data/images/red-scifi-metal-bl/red-scifi-metal-bl/red-scifi-metal_ao.png";
            t5.fmt = Renderer::ImageFormat::R;

            std::vector<Tex> texVec = { t1, t2, t3, t4, t5 };
            std::vector<unsigned int> texIdVec;
            int w, h;
            Renderer::ImageFormat inFmt;
            for (auto& t : texVec) {
                loader.loadImage(t.path, w, h, inFmt, *vecptr.get());
                auto texPtr = std::make_unique<Texture>(std::move(vecptr), w, h, inFmt, t.fmt, t.path);
                unsigned int texId = manager.storeTexture(std::move(texPtr));
                texIdVec.push_back(texId);
                vecptr = std::make_unique<std::vector<unsigned char>>();
            }

            mat4->setTexture(manager.getTexture(texIdVec[0]), Texture::TextureType::ALBEDO_MAP);
            mat4->setTexture(manager.getTexture(texIdVec[1]), Texture::TextureType::METALLIC_MAP);
            mat4->setTexture(manager.getTexture(texIdVec[2]), Texture::TextureType::NORMAL_MAP);
            mat4->setTexture(manager.getTexture(texIdVec[3]), Texture::TextureType::ROUGHNESS_MAP);
            mat4->setTexture(manager.getTexture(texIdVec[4]), Texture::TextureType::AO_MAP);
        }
        // Create a cube mesh
        Mesh* mesh;
        {
            std::unique_ptr<Mesh> tempMesh = std::make_unique<Mesh>("Cube");
            mesh = manager.getMesh(manager.storeMesh(std::move(tempMesh)));

            std::unique_ptr<SubMesh> tempSubMesh = PrimitiveCreation::createCubeMesh();
            SubMesh* submesh = manager.getSubmesh(manager.storeSubmesh(std::move(tempSubMesh)));

            mesh->addSubMesh(mat, submesh);
        }
        // Create a sphere mesh
        Mesh* mesh2;
        {
            std::unique_ptr<Mesh> tempMesh = std::make_unique<Mesh>("Sphere");
            mesh2 = manager.getMesh(manager.storeMesh(std::move(tempMesh)));

            std::unique_ptr<SubMesh> tempSubMesh = PrimitiveCreation::createSphere(15,30);
            SubMesh* submesh = manager.getSubmesh(manager.storeSubmesh(std::move(tempSubMesh)));

            mesh2->addSubMesh(mat2, submesh);
        }
        // Create a plane mesh
        Mesh* mesh3 = nullptr;
        {
            std::unique_ptr<Mesh> tempMesh = std::make_unique<Mesh>("Plane");
            mesh3 = manager.getMesh(manager.storeMesh(std::move(tempMesh)));

            std::unique_ptr<SubMesh> tempSubMesh = PrimitiveCreation::createPlane(50);
            SubMesh* submesh = manager.getSubmesh(manager.storeSubmesh(std::move(tempSubMesh)));

            mesh3->addSubMesh(mat4, submesh);
        }


        /*Entity entity = m_ecs.createEntity();
        Entity entity2 = m_ecs.createEntity();
        Entity entity3 = m_ecs.createEntity();*/
        Entity entity4 = m_ecs.createEntity();
        Entity cameraEntity = m_ecs.createEntity();

        // Add model components to entities
        {
            //{
            //    Model3D m;
            //    m.mesh = mesh;
            //    m_ecs.addComponent(entity, m);
            //    m_ecs.addComponent(entity3, m);
            //}
            //
            //{
            //    Model3D m2;
            //    m2.mesh = mesh2;
            //    m_ecs.addComponent(entity2, m2);
            //}
            
            {
                Model3D m;
                m.mesh = mesh3;
                m_ecs.addComponent(entity4, m);
            }

        }
        // Add light to entity2
       /* {
            Light l;
            m_ecs.addComponent(entity2, l);
        }*/
        // Add camera to camera entity
        {
            Camera c;
            m_ecs.addComponent(cameraEntity, c);
            InputEvents::SceneCameraMakeActiveEvent::notify(cameraEntity);
        }
        // Move camera entity
        {
            auto& t = m_ecs.getComponent<Transform>(cameraEntity);
            t.position = glm::vec3(0.f, 2.f, 10.f);
        }
        // Move entity 2 a bit
        //{
        //    auto& t = m_ecs.getComponent<Transform>(entity2);
        //    t.position = glm::vec3(1.f);
        //    //m_ecs.getComponent<Transform>(entity2).scale = glm::vec3(0.1f);
        //}

        //// Move entity 1 up
        //{
        //    auto& t = m_ecs.getComponent<Transform>(entity);
        //    t.position = glm::vec3(0.f, 10.f, 0.f);
        //}
        //
        //// Move entity 3 up
        //{
        //    auto& t = m_ecs.getComponent<Transform>(entity3);
        //    t.position = glm::vec3(0.f, 2.f, 0.f);
        //}

        // Scale entity 4
        /*{
            auto& t = m_ecs.getComponent<Transform>(entity4);
            t.scale = glm::vec3(5.f, 1.f, 5.f);
        }*/
        // Add physics
        {
            //RigidBody r;
            //r.type = (RigidBody::RigidBodyType::DBOX);
            //m_ecs.addComponent(entity, r);
            //m_ecs.addComponent(entity3, r);

            //RigidBody r2;
            //r2.type = (RigidBody::RigidBodyType::SSPHERE);
            //m_ecs.addComponent(entity2, r2);

            RigidBody r3;
            r3.type = RigidBody::RigidBodyType::TRIANGLEMESH;
            m_ecs.addComponent(entity4, r3);
        }

        // Benchmarking
        //Benchmark::addEntities(1000, &m_ecs);
        //Benchmark::addBenchmarkComponents(1000, &m_ecs);
        //Benchmark::addRigidBodySpheres(&m_ecs, mesh2, m_sceneGraphSystem.get());

    }
#if 0

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
    auto mat = std::make_unique<PBRMaterial>("First material");
    //mat->defaultPhong();
    auto matid = manager.storeMaterial(std::move(mat));
    auto mat2 = manager.getMaterial(matid);

    auto mat3 = std::make_unique<PBRMaterial>("Second material");
    //mat3->defaultPhong();
    matid = manager.storeMaterial(std::move(mat3));
    auto mat4 = manager.getMaterial(matid);

    auto m = PrimitiveCreation::createCubeMesh();
    auto smeshid = manager.storeSubmesh(std::move(m));
    SubMesh* smesh = manager.getSubmesh(smeshid);

    newMeshPtr->addSubMesh(mat2, smesh);
    node->setMesh(newMeshPtr);

    /*
    * Import some textures
    */
    ImageLoader loader;
    auto vecptr = std::make_unique<std::vector<unsigned char>>();
    struct Tex {
        std::string path;
        Renderer::ImageFormat fmt;
    };

    // Material 1 textures
    {
        Tex t1;
        t1.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_basecolor.png";
        t1.fmt = Renderer::ImageFormat::sRGB;
        Tex t2;
        t2.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_metallic.png";
        t2.fmt = Renderer::ImageFormat::R;
        Tex t3;
        t3.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_normal.png";
        t3.fmt = Renderer::ImageFormat::RGB;
        Tex t4;
        t4.path = "C:/dev/Meadow/data/images/rustediron1-alt2-bl/rustediron1-alt2-bl/rustediron2_roughness.png";
        t4.fmt = Renderer::ImageFormat::R;

        std::vector<Tex> texVec = { t1, t2, t3, t4 };
        std::vector<unsigned int> texIdVec;
        int w, h;
        Renderer::ImageFormat inFmt;
        for (auto& t : texVec) {
            loader.loadImage(t.path, w, h, inFmt, *vecptr.get());
            auto texPtr = std::make_unique<Texture>(std::move(vecptr), w, h, inFmt, t.fmt, t.path);
            unsigned int texId = manager.storeTexture(std::move(texPtr));
            texIdVec.push_back(texId);
            vecptr = std::make_unique<std::vector<unsigned char>>();
        }

        mat2->setTexture(manager.getTexture(texIdVec[0]), Texture::TextureType::ALBEDO_MAP);
        mat2->setTexture(manager.getTexture(texIdVec[1]), Texture::TextureType::METALLIC_MAP);
        mat2->setTexture(manager.getTexture(texIdVec[2]), Texture::TextureType::NORMAL_MAP);
        mat2->setTexture(manager.getTexture(texIdVec[3]), Texture::TextureType::ROUGHNESS_MAP);
    }
    // Material 2 textures
    {
        Tex t1;
        t1.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_albedo.png";
        t1.fmt = Renderer::ImageFormat::sRGB;
        Tex t2;
        t2.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_metallic.png";
        t2.fmt = Renderer::ImageFormat::R;
        Tex t3;
        t3.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_normal-ogl.png";
        t3.fmt = Renderer::ImageFormat::RGB;
        Tex t4;
        t4.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_roughness.png";
        t4.fmt = Renderer::ImageFormat::R;
        Tex t5;
        t5.path = "C:/dev/Meadow/data/images/ravine-rock1-ue/ravine-rock1-ue/ravine-rock1_ao.png";
        t5.fmt = Renderer::ImageFormat::R;

        std::vector<Tex> texVec = { t1, t2, t3, t4, t5 };
        std::vector<unsigned int> texIdVec;
        int w, h;
        Renderer::ImageFormat inFmt;
        for (auto& t : texVec) {
            loader.loadImage(t.path, w, h, inFmt, *vecptr.get());
            auto texPtr = std::make_unique<Texture>(std::move(vecptr), w, h, inFmt, t.fmt, t.path);
            unsigned int texId = manager.storeTexture(std::move(texPtr));
            texIdVec.push_back(texId);
            vecptr = std::make_unique<std::vector<unsigned char>>();
        }

        mat4->setTexture(manager.getTexture(texIdVec[0]), Texture::TextureType::ALBEDO_MAP);
        mat4->setTexture(manager.getTexture(texIdVec[1]), Texture::TextureType::METALLIC_MAP);
        mat4->setTexture(manager.getTexture(texIdVec[2]), Texture::TextureType::NORMAL_MAP);
        mat4->setTexture(manager.getTexture(texIdVec[3]), Texture::TextureType::ROUGHNESS_MAP);
        mat4->setTexture(manager.getTexture(texIdVec[4]), Texture::TextureType::AO_MAP);
    }

    /*
    * Add some transform to the node
    */
    /*node->scale = glm::vec3(4.0f, 4.0f, 1.0f);
    node->position = glm::vec3(0.0f, 2.0f, -1.0f);*/
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

    meshNode2->addSubMesh(mat4, mesh2);
    /*
    * Transform the second node
    */
    node2->position = glm::vec3(-3.0f, -0.1f, .0f);
    //node2->scale = glm::vec3(3.0f, 0.01f, 3.0f);

    /*
    * Add third node and mesh and mat
    */
    unsigned int thirdMaterialID = manager.storeMaterial(std::make_unique<ColorMaterial>("Color Material"));
    Material* thirdMaterial = manager.getMaterial(thirdMaterialID);

    unsigned int sphereSubmeshID = manager.storeSubmesh(std::move(PrimitiveCreation::createSphere(30, 50)));
    SubMesh* sphereSubmesh = manager.getSubmesh(sphereSubmeshID);
    unsigned int thirdNodeID = m_scene->addNode();
    SceneNode* thirdNode = m_scene->getNode(thirdNodeID);
    unsigned int thirdMeshID = manager.storeMesh(std::move(std::make_unique<Mesh>("Lamp")));
    Mesh* thirdMesh = manager.getMesh(thirdMeshID);
    thirdMesh->addSubMesh(thirdMaterial, sphereSubmesh);
    thirdNode->setMesh(thirdMesh);
    thirdNode->scale = glm::vec3(0.1f);
    thirdNode->position = glm::vec3(0.9f, 1.f, 1.5f);
    thirdNode->name = "LampNode";



    /*
    * Add some light
    */
    /*if (!DirectionalLight::maxInstanceCapacity()) {
        auto dirLight = std::make_unique<DirectionalLight>();
        m_scene->getNode(0)->setLightSource(std::move(dirLight));
    }*/
    if (!PointLight::maxInstanceCapacity()) {
        auto pointLight = std::make_unique<PointLight>();
        m_scene->getNode(thirdNodeID)->setLightSource(std::move(pointLight));
    }

    /*
    * import a model
    */
    //ModelImporting::objsFromFile("C:/dev/Meadow/data/3dmodels/old-office-window/source/office window.fbx", m_scene.get(), 0);
    ModelImporting::objsFromFile("C:/dev/Meadow/data/3dmodels/gooby/only_LP_FIXING_MESH_FOR_BETTER_BAKING.obj", m_scene.get(), 0);
    //ModelImporting::objsFromFile("C:/dev/Meadow/data/3dmodels/modular-lowpoly-medieval-environment/medieval_scene.fbx", m_scene.get(), 0);

    //m_scene->getNode(4)->scale = glm::vec3(0.1f);
#endif
}
