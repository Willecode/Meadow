#include "postprocessing.h"
#include "assets/texture.h"
#include "service_locator/locator.h"
#include "resource_management/resourcemanager.h"
#include "primitivecreation.h"
#include "input/inputevents.h"

PostProcessing::PostProcessing() :
    m_sdrMan(nullptr),
    m_grayscale(false),
    m_sharpen(false),
    m_negate(false)
{
    /*
    * Subscribe to events
    */
    std::function<void(bool)> sharpnessFunc = std::bind(&PostProcessing::sharpen, this, std::placeholders::_1);
    InputEvents::PostprocSharpnessEvent::subscribe(sharpnessFunc);
    
    std::function<void(bool)> grayscaleFunc = std::bind(&PostProcessing::grayscale, this, std::placeholders::_1);
    InputEvents::PostprocGrayscaleEvent::subscribe(grayscaleFunc);
    
    std::function<void(bool)> negateFunc = std::bind(&PostProcessing::negate, this, std::placeholders::_1);
    InputEvents::PostprocNegativeEvent::subscribe(negateFunc);
}

void PostProcessing::init(ShaderManager* sdrMan)
{
    m_sdrMan = sdrMan;
    sharpen(false);
    grayscale(false);
    negate(false);
}

void PostProcessing::sharpen(bool f)
{
    m_sdrMan->setCurrentShader("postprocess");
    m_sdrMan->setFrameUniform("sharpen", f);
    m_sharpen = f;
}

void PostProcessing::grayscale(bool f)
{
    m_sdrMan->setCurrentShader("postprocess");
    m_sdrMan->setFrameUniform("grayscale", f);
    m_grayscale = f;
}

void PostProcessing::negate(bool f)
{
    m_sdrMan->setCurrentShader("postprocess");
    m_sdrMan->setFrameUniform("negate", f);
    m_negate = f;
}

bool PostProcessing::getSharpen() const
{
    return m_sharpen;
}

bool PostProcessing::getGrayscale() const
{
    return m_grayscale;
}

bool PostProcessing::getNegate() const
{
    return m_negate;
}
