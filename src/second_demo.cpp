#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
#include "shader_s.h"
 
#include <iostream>
#include <map>
#include <array>
#include <vector>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void getUniformLocations(std::map<std::string, int> *m, std::vector<std::string> names, int shaderID);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
float mixValue = 1.;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

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
    Shader objectShader("D:/OpenGL_programming/projects/openGL_playground/shaders/second_demo/object.vs", "D:/OpenGL_programming/projects/openGL_playground/shaders/second_demo/object.fs"); 
    Shader lightShader("D:/OpenGL_programming/projects/openGL_playground/shaders/second_demo/lightsource.vs", "D:/OpenGL_programming/projects/openGL_playground/shaders/second_demo/lightsource.fs"); 
    
    glEnable(GL_DEPTH_TEST);

    // Cube vertices, texture coordinates
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  //0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  //1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  //0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  //0.0f, 0.0f,
//
        -0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  //1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  //1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  //1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  //0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,
//
        -0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
//
         0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
//
        -0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  //1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  //1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  //1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
//
        -0.5f,  0.5f, -0.5f,  //0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  //0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  //0.0f, 1.0f
    };

    // Buffers
    // ---------------
    unsigned int VBO, objectVAO, lightsourceVAO;
    glGenVertexArrays(1, &objectVAO);
    glGenVertexArrays(1, &lightsourceVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glBindVertexArray(objectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(lightsourceVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //-----------------

    // unsigned int VBO, VAO, EBO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // // position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // texturecoordinate attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // Draw loop variables
    // --------------------
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view;
    glm::mat4 projection;
    
    // Create maps with shader uniform locations for easy access
    // ---------------------------------------------------------
    std::map<std::string, int> objUniformLocs;
    std::vector<std::string> uniforms = {"model", "view", "projection", "lightCol", "objectCol"};
    getUniformLocations(&objUniformLocs, uniforms, objectShader.ID);

    std::map<std::string, int> lightUniformLocs;
    uniforms = {"model", "view", "projection", "lightCol"};
    getUniformLocations(&lightUniformLocs, uniforms, lightShader.ID);
    // ---------------------------------------------------------

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        // render object
        objectShader.use();
        glUniform3f(objUniformLocs.at("objectCol"), 0.0f, 1.0f, 0.0f);
        glUniform3f(objUniformLocs.at("lightCol"), 1.0f, 1.0f, 1.0f);

        model = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraUp);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(objUniformLocs.at("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(objUniformLocs.at("projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(objUniformLocs.at("model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render lightsource
        lightShader.use();
        glUniform3f(lightUniformLocs.at("lightCol"), 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(lightUniformLocs.at("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lightUniformLocs.at("projection"), 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(lightUniformLocs.at("model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(lightsourceVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &objectVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        mixValue = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        mixValue = 0;
    }
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

void getUniformLocations(std::map<std::string, int> *m, std::vector<std::string> names, int shaderID){
    // Populates m with argument name:location
    for (int i = 0; i < names.size(); i++){
        m->insert(std::pair<std::string, int>(names.at(i), glGetUniformLocation(shaderID, names.at(i).c_str())));
    }
}