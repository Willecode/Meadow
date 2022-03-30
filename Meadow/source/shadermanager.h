#pragma once
#include "shader_s.h"
#include <memory>
class ShaderManager
{
public:
	ShaderManager();
	static void initShaderManager();
	static Shader* getPhongShader();
	static Shader* getColorShader();
private:
	static bool initialized;
	static std::unique_ptr<Shader> m_PhongSdr;
	static std::unique_ptr<Shader> m_ColorSdr;
};

