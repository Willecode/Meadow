#pragma once
class Shader
{
public:
	Shader(const unsigned int &id, const char* vertexPath, const char* fragmentPath);
	unsigned int getId();
private:
	unsigned int m_id;
};

