#include "postprocessing.h"
#include "assets/texture.h"
#include "service_locator/rendererlocator.h"
#include "resource_management/resourcemanager.h"
#include "primitivecreation.h"
#include "input/inputevents.h"

PostProcessing::PostProcessing() :
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

    InputEvents::MSAAToggleEvent::subscribe(std::bind(&PostProcessing::MSAA, this, std::placeholders::_1));
}

void PostProcessing::init()
{
    sharpen(false);
    grayscale(false);
    negate(false);
    MSAA(true);
}

void PostProcessing::sharpen(bool f)
{
    ShaderManager& sdrMan = ShaderManager::getInstance();
    sdrMan.bindShader(ShaderManager::ShaderType::POSTPROCESS);
    sdrMan.setFrameUniform("sharpen", f);
    m_sharpen = f;
}

void PostProcessing::grayscale(bool f)
{
    ShaderManager& sdrMan = ShaderManager::getInstance();
    sdrMan.bindShader(ShaderManager::ShaderType::POSTPROCESS);
    sdrMan.setFrameUniform("grayscale", f);
    m_grayscale = f;
}

void PostProcessing::negate(bool f)
{
    ShaderManager& sdrMan = ShaderManager::getInstance();
    sdrMan.bindShader(ShaderManager::ShaderType::POSTPROCESS);
    sdrMan.setFrameUniform("negate", f);
    m_negate = f;
}

void PostProcessing::MSAA(bool f)
{
    m_MSAA = f;
    RendererLocator::getRenderer()->multisampling(f);
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

bool PostProcessing::getMSAA() const
{
    return m_MSAA;
}
