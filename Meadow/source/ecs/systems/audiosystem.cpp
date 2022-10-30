#include "audiosystem.h"
#include <fmod/fmod_errors.h>
#include "service_locator/loggerlocator.h"

void AudioSystem::init(ECSCoordinator* ecs)
{
    m_ecs = ecs;
    FMOD_RESULT result;
    m_system = NULL;

    result = FMOD::System_Create(&m_system);      // Create the main system object.
    if (result != FMOD_OK)
    {
        LoggerLocator::getLogger()->getLogger()->error("AudioSystem init failed: {}", FMOD_ErrorString(result));
        return;
    }

    result = m_system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (result != FMOD_OK)
    {
        LoggerLocator::getLogger()->getLogger()->error("AudioSystem init failed: {}", FMOD_ErrorString(result));
        return;
    }

    m_system->createSound("C:/dev/Meadow/data/sounds/ps2startup.mp3", FMOD_DEFAULT, nullptr, &m_sound);

    FMOD::Channel* channel = nullptr;
    result = m_system->playSound(m_sound, nullptr, false, &channel);
    if (result != FMOD_OK) {
        LoggerLocator::getLogger()->getLogger()->error("AudioSystem init failed: {}", FMOD_ErrorString(result));
        return;
    }
}

void AudioSystem::update(float deltaT)
{
    if (m_system != NULL)
        m_system->update();
}
