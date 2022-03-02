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

#include <iostream>
#include <map>
#include <array>
#include <vector>


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void getUniformLocations(std::map<std::string, int>* m, std::vector<std::string> names, int shaderID);
void mouseCallback(GLFWwindow* window, double xPosArg, double yPosArg);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const float SCR_WIDTH = 1000.0;
const float SCR_HEIGHT = 800.0f;
const float ASPECT_RATIO = SCR_WIDTH / SCR_HEIGHT;
const float ZNEAR = 0.1f;
const float ZFAR = 100.0f;

Camera camera(ASPECT_RATIO, ZNEAR, ZFAR);

// Initialize global non-const variables
bool firstMouse = true;
float lastMouseX = SCR_WIDTH / 2.0f;
float lastMouseY = SCR_HEIGHT / 2.0f;
float lastFrameTime = 0.f;
float currentFrameTime = 0.f;
float deltaTime = 0.f;
bool wireframeMode = false;
bool altPressLastFrame = false;

int main()
{
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
    // Create primitive meshes
    const Mesh MESH_CUBE = PrimitiveCreation::createCubeMesh();
    const Mesh MESH_SPHERE = PrimitiveCreation::createSphere(30,15);
    // Compile shaders
    Shader phongTexShader("shaders/object.vs", "shaders/phongtex.fs");
    Shader phongSolidColShader("shaders/object.vs", "shaders/phongsolidcol.fs");
    Shader colorOnlyShader("shaders/object.vs", "shaders/coloronly.fs");
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);

    // Set up textures
    ImageCache cache;
    Texture diffuseMap("images/Wood066_1K_Color.jpg", GL_TEXTURE_2D, cache);
    Texture specularMap("images/Fingerprints009_1K_Color.jpg", GL_TEXTURE_2D, cache);
    
    Scene scene(&camera);
    
    glm::vec3 colorPink = glm::vec3(232.0f / 255, 100.0f / 255, 190.0f / 255);
    //PhongMaterial cubeMat;
    
    // Create objects
    PhongMaterial cubeMat(&diffuseMap, &specularMap);
    ColorOnlyMaterial cubeMat2;
    Object3D cube;
    cube.addMesh(&MESH_SPHERE);
    cube.setMaterial(&cubeMat);
    cube.setShader(&phongTexShader);

    glm::vec3 lampCol = glm::vec3(0.f, 153.f / 255.f, 0.f);
    PointLight light(lampCol * 0.0f, lampCol *0.5f, lampCol);

    Object3D dirLightObj;
    DirectionalLight dirLight(glm::vec3(-0.5f, -0.5f, 0.0f));
    dirLightObj.addLightSource(&dirLight);

    Object3D dirLightObj2;
    DirectionalLight dirLight2(glm::vec3(0.5f, -0.5f, 0.0f));
    dirLightObj2.addLightSource(&dirLight2);

    Object3D lamp;
    ColorOnlyMaterial lampMat(lampCol);
    lamp.addMesh(&MESH_CUBE);
    lamp.setMaterial(&lampMat);
    lamp.setShader(&colorOnlyShader);
    glm::mat4 lampModelMat = glm::mat4(1.0f);
    lampModelMat = glm::translate(lampModelMat, glm::vec3(2.0f, 0.0f, 0.0f));
    lamp.setModelMatrix(lampModelMat);
    lamp.addLightSource(&light);

    Object3D lamp2 = lamp;
    glm::mat4 lamp2ModelMat = glm::mat4(1.0f);
    lamp2ModelMat = glm::translate(lamp2ModelMat, glm::vec3(-5.0f, .0f, 0.0f));
    lamp2.setModelMatrix(lamp2ModelMat);

    // Add objects to scene
    //scene.addObject(&lamp);
    scene.addObject(&lamp2);
    scene.addObject(&cube);
    scene.addObject(&dirLightObj);
    scene.addObject(&dirLightObj2);

    scene.updateLighting();
    scene.updateShaders();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate deltatime ---
        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        //---

        processInput(window);

        lamp2ModelMat = glm::mat4(1.0f);
        lamp2ModelMat = glm::translate(lamp2ModelMat, glm::vec3(sin((float)glfwGetTime()) * 2, 0.0f, cos((float)glfwGetTime()) * 2));
        lamp2ModelMat = glm::scale(lamp2ModelMat, glm::vec3(0.2f, 0.2f, 0.2f));
        lamp2.setModelMatrix(lamp2ModelMat);

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
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    const float cameraSpeed = 0.05f;
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

void getUniformLocations(std::map<std::string, int>* m, std::vector<std::string> names, int shaderID) {
    // Populates m with argument name:location
    for (int i = 0; i < names.size(); i++) {
        m->insert(std::pair<std::string, int>(names.at(i), glGetUniformLocation(shaderID, names.at(i).c_str())));
    }
}