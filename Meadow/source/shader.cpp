#include "shader.h"
#include <fstream>
#include <sstream>
#include <fmt/format.h>
#include "service_locator/locator.h"

Shader::Shader(const unsigned int& id, const char* vertexPath, const char* fragmentPath):m_id(id)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
         Locator::getLogger()->getLogger()->info("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
         vertexCode = "";
         fragmentCode = "";
    }
    
    Locator::getRenderer()->createShaderProgram(m_id, vertexCode.c_str(), fragmentCode.c_str());

}

unsigned int Shader::getId()
{
    return m_id;
}
