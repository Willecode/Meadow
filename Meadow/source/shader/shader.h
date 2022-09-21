#pragma once
#include <glm/glm.hpp>
#include <string>
class Shader
{
public:
	Shader(const unsigned int &id, const char* vertexPath, const char* fragmentPath);
	unsigned int getId();
	void hotReload();
private:
	unsigned int m_id;
	// paths to shader source files
	const char* m_vertexPath, *m_fragmentPath;
private:
	// Fetches source code from paths and copies it to strings
	void sourceFromFile(const char* vertexPath, const char* fragmentPath,
						std::string &vertexCode, std::string &fragmentCode);
};

