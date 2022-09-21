#pragma once
#include "shader/shadermanager.h"
class Lighting
{
public:
	Lighting();
	void init(ShaderManager* sdrMan);
	void setPhongShading();
	void setBlinnShading();
private:
	void lightingEventHandler(bool blinn);
	ShaderManager* m_sdrMan;
	
};

