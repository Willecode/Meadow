#include <glad/glad.h>
#include <GLFW/glfw3.h>
//--------------------------------------
// IMAGE LOADER
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
//--------------------------------------
// MATH FUNCTIONS
#define GLM_ENABLE_EXPERIMENTAL // for glm/ext.hpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp> // For printing glm vectors
//--------------------------------------
// STRING FORMATTING
#define FMT_HEADER_ONLY
#include <fmt/core.h>
//--------------------------------------
#include "shader_s.h"
#include "scene.h"
#include "object3d.h"
#include "texture.h"
#include "materials/phongmaterial.h"
#include "materials/coloronlymaterial.h"
#include "primitivecreation.h"
#include "camera.h"
#include "modelimporting.h"

#include <iostream>
#include <map>
#include <array>
#include <vector>


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Scene* scene);
void getUniformLocations(std::map<std::string, int>* m, std::vector<std::string> names, int shaderID);
void mouseCallback(GLFWwindow* window, double xPosArg, double yPosArg);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const float SCR_WIDTH = 1920.0;
const float SCR_HEIGHT = 1080.0f;
const float ASPECT_RATIO = SCR_WIDTH / SCR_HEIGHT;
const float ZNEAR = 0.1f;
const float ZFAR = 100.0f;

Camera camera(ASPECT_RATIO, ZNEAR, ZFAR);

// Initialize global non-const variables
bool mouseLock = true;
bool firstMouse = true;
float lastMouseX = SCR_WIDTH / 2.0f;
float lastMouseY = SCR_HEIGHT / 2.0f;
float lastFrameTime = 0.f;
float currentFrameTime = 0.f;
float deltaTime = 0.f;
bool wireframeMode = false;
bool altPressLastFrame = false;
bool tabPressLastFrame = false;
bool fPressLastFrame = false;
bool selected = false;

int main()
{
    // Create a cache for loaded images
    ImageCache textureCache;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "´Wilkan Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Depth test config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Stencil test config
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Create primitive meshes
    std::shared_ptr<Mesh> MESH_CUBE = PrimitiveCreation::createCubeMesh();
    std::shared_ptr<Mesh> MESH_SPHERE = PrimitiveCreation::createSphere(30, 25);
    // Compile shaders
    Shader phongTexShader("shaders/object.vs", "shaders/phongtex.fs");
    Shader colorOnlyShader("shaders/object.vs", "shaders/coloronly.fs");
    
    
    //stbi_set_flip_vertically_on_load(true);

    // Set up textures
    auto diffuseMap = std::make_shared<Texture>("images/Wood066_1K_Color.jpg", GL_TEXTURE_2D, textureCache);
    auto specularMap = std::make_shared<Texture>("images/squares.jpg", GL_TEXTURE_2D, textureCache);
    
    // Create scene
    Scene scene(&camera);
    camera.speed *= 2;
    // Create objects
    auto sphereMat = std::make_shared<PhongMaterial>(diffuseMap);
    auto sphere = std::make_shared<Object3D>();
    sphere->addMesh(MESH_SPHERE, 0);
    sphere->setMaterial(sphereMat, 0);
    sphere->setShader(&phongTexShader);
    glm::mat4 sphereModelMat = glm::mat4(1.0f);
    //sphereModelMat = glm::scale(sphereModelMat, glm::vec3(.1f));
    
    sphere->setModelMatrix(sphereModelMat);

    // Create objects
    auto sphere2 = std::make_shared<Object3D>();
    sphere2->addMesh(MESH_SPHERE, 0);
    sphere2->setMaterial(sphereMat, 0);
    sphere2->setShader(&phongTexShader);
    sphere2->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f)));
    sphere->addChild(sphere2);


    auto cube = std::make_shared<Object3D>();
    cube->addMesh(MESH_CUBE, 0);
    cube->setMaterial(sphereMat, 0);
    cube->setShader(&phongTexShader);
    cube->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
    // From obj file
    //auto importObj = ModelImporting::importWavefront("./3dmodels/Stone_church_of_kakskerta_isle_Turku_Finland/Stone_church_of_kakskerta_isle_Turku_Finland.obj", textureCache);
    //importObj->setShader(&phongTexShader);
    //glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -10.0f, -5.0f));
    //modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //importObj->setModelMatrix(modelMat);
    //PhongMaterial* impMat = dynamic_cast<PhongMaterial*>(importObj->getMaterial(0));
    //impMat->shininess = 10.0f;
    //impMat->specular *= 0.1;

    //auto importObj2 = ModelImporting::importWavefront("./3dmodels/gooby/only_LP_FIXING_MESH_FOR_BETTER_BAKING.obj", textureCache);
    //importObj2->setShader(&phongTexShader);
    //glm::mat4 modelMat2 = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -10.0f, -5.0f));
    //modelMat2 = glm::rotate(modelMat2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //importObj2->setModelMatrix(modelMat2);
    //PhongMaterial* impMat2 = dynamic_cast<PhongMaterial*>(importObj2->getMaterial(0));
    //impMat2->shininess = 10.0f;
    //impMat2->specular *= 0.1;

    auto importObj = ModelImporting::objsFromFile("./3dmodels/medieval-market/market.fbx", textureCache);
    importObj->setShader(&phongTexShader);
    importObj->setModelMatrix(glm::scale(importObj->getModelMatrix(), glm::vec3(0.1)));
    /*PhongMaterial* impMat = dynamic_cast<PhongMaterial*>(importObj->getMaterial(0));
    impMat->shininess = 10.0f;
    impMat->specular *= 0.1;*/

    // Create some lightsource objects
    auto dirLightObj = std::make_shared<Object3D>();
    dirLightObj->addLightSource(std::make_shared<DirectionalLight>(glm::vec3(-0.5f, -0.5f, 0.0f)));

    glm::vec3 lampCol = glm::vec3(0.f, 153.f / 255.f, 0.f);
    auto pointLightObj = std::make_shared<Object3D>();
    auto pointlight = std::make_shared<PointLight>();
    pointlight->linear = 0.045f;
    pointlight->quadratic = 0.0075f;
    auto mat = std::make_shared<ColorOnlyMaterial>();
    pointLightObj->addLightSource(pointlight);
    pointLightObj->addMesh(MESH_CUBE, 0);
    pointLightObj->setShader(&colorOnlyShader);
    pointLightObj->setMaterial(mat, 0);

    // Add objects to scene
    //scene.addObject(sphere);
    scene.addObject(dirLightObj);
    scene.addObject(pointLightObj);
    //scene.addObject(cube);
    scene.addObject(importObj);
    

    scene.updateLighting();
    scene.updateShaders();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Calculate deltatime ---
        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        //---

        processInput(window, &scene);

        glm::mat4 modelMat = glm::mat4(1.0f);
        glm::vec3 lightStartPos = glm::vec3(10.f, 12.0f, 0.0f);
        modelMat = glm::translate(modelMat, lightStartPos + glm::vec3(sin((float)glfwGetTime()) * 2, 0.0f, cos((float)glfwGetTime()) * 2)* 6);
        modelMat = glm::scale(modelMat, glm::vec3(0.2f));
        pointLightObj->setModelMatrix(modelMat);
        dirLightObj->getLightSource()->rotate((float)glfwGetTime() / 100, glm::vec3(0.f, -1.f, 0.f));

        //sphereModelMat = glm::rotate(sphereModelMat, .1f, glm::vec3(0.f, 0.f, 1.0f));
        //sphere->setModelMatrix(sphereModelMat);
        
        scene.drawScene();
       
        // glfw: swap buffers and poll IO events
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Scene* scene)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.inputMoveForward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.inputMoveBackward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.inputMoveLeft(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.inputMoveRight(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.inputMoveUp(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.inputMoveDown(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (!altPressLastFrame) {
            if (wireframeMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireframeMode = false;
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireframeMode = true;
            }
        }
        altPressLastFrame = true;
    }
    else {
        altPressLastFrame = false;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!tabPressLastFrame) {
            if (mouseLock) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                mouseLock = false;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                mouseLock = true;
            }
        }
        tabPressLastFrame = true;
    }
    else
    {
        tabPressLastFrame = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

        if (!fPressLastFrame) {
            if (!selected) {
                scene->selectObject(0);
                selected = true;
            }
            else {
                scene->deselectAllObjects();
                selected = false;
            }
        }
        fPressLastFrame = true;
    }
    else
    {
        fPressLastFrame = false;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double mouseXIn, double mouseYIn) {
    float mouseX = static_cast<float>(mouseXIn);
    float mouseY = static_cast<float>(mouseYIn);

    if (firstMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
    }
    camera.processMouseMovement(mouseX - lastMouseX, mouseY - lastMouseY, deltaTime);
    lastMouseX = mouseX;
    lastMouseY = mouseY;

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float fov = camera.getFov();
    fov += static_cast<float>(-yoffset);
    camera.setFov(fov);
}
