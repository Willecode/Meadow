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
#include "material.h"
#include "cube.h"

#include <iostream>
#include <map>
#include <array>
#include <vector>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void getUniformLocations(std::map<std::string, int>* m, std::vector<std::string> names, int shaderID);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
float mixValue = 1.;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Demo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Compile shaders
    /*Shader objectShader("shaders/object.vs", "shaders/object.fs");
    Shader lightShader("shaders/lightsource.vs", "shaders/lightsource.fs");*/
    Shader simpleShader("shaders/simple.vs", "shaders/simple.fs");
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);

    // Set up textures
    ImageCache cache;
    Texture diffuseMap("images/jellyfish.png", GL_TEXTURE_2D, cache);
    Texture specularMap("images/Fingerprints009_1K_Color.jpg", GL_TEXTURE_2D, cache);
    
    // Material
    glm::vec3 color = glm::vec3(232.0f / 255, 100.0f / 255, 190.0f / 255);
    Material cubeMat(color, color, color, &diffuseMap, &specularMap);
    Scene scene;
    Cube cube;
    cube.setMaterial(&cubeMat);
    cube.setShader(&simpleShader);
    scene.addObject(&cube);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // input
        // -----
        processInput(window);

        // Camera settings 
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime()/2, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraUp);
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1000.0f / 800.0f, 0.1f, 100.0f);

        // Set shader
        simpleShader.use();

        glUniformMatrix4fv(glGetUniformLocation(simpleShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(simpleShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        //glUniformMatrix4fv(glGetUniformLocation(simpleShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        cube.setModelMatrix(&modelMatrix);
        simpleShader.setFloat3("lightCol", 1.0f, 1.0f, 1.0f);
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
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixValue = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue = 0;
    }
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos += cameraUp * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraPos += -cameraUp * cameraSpeed;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void getUniformLocations(std::map<std::string, int>* m, std::vector<std::string> names, int shaderID) {
    // Populates m with argument name:location
    for (int i = 0; i < names.size(); i++) {
        m->insert(std::pair<std::string, int>(names.at(i), glGetUniformLocation(shaderID, names.at(i).c_str())));
    }
}