#include "lighting.h"
#include "input/inputevents.h"
Lighting::Lighting()
{
	/*
	* Subscribe to events
	*/
	std::function<void(bool)> blinnFunc = std::bind(&Lighting::lightingEventHandler, this, std::placeholders::_1);
	InputEvents::LightingBlinnEvent::subscribe(blinnFunc);
}

void Lighting::init(ShaderManager* sdrMan)
{
	m_sdrMan = sdrMan;
}
void Lighting::setPhongShading()
{
	m_sdrMan->setCurrentShader("phong");
	m_sdrMan->setFrameUniform("blinn", false);
}

void Lighting::setBlinnShading()
{
	m_sdrMan->setCurrentShader("phong");
	m_sdrMan->setFrameUniform("blinn", true);
}

void Lighting::lightingEventHandler(bool blinn)
{
	if (blinn)
		setBlinnShading();
	if (!blinn)
		setPhongShading();
}
