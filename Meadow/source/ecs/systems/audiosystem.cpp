#include "audiosystem.h"
#include <fmod/fmod_errors.h>
#include "service_locator/loggerlocator.h"
#include "input/inputevents.h"
#include "resource_management/resourcemanager.h"

AudioSystem::AudioSystem()
{
    InputEvents::PlaySoundEvent::subscribe(std::bind(&AudioSystem::playSound, this, std::placeholders::_1));
}

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

    if (result != FMOD_OK) {
        LoggerLocator::getLogger()->getLogger()->error("AudioSystem init failed: {}", FMOD_ErrorString(result));
        return;
    }
}

void AudioSystem::update(float deltaT)
{
    for (auto& ent : m_entities) {

    }
    if (m_system != NULL)
        m_system->update();
}

void AudioSystem::playSound(unsigned int s)
{
    LoggerLocator::getLogger()->getLogger()->info("Playing sound {}", s);
    auto& resMan = ResourceManager::getInstance();
    Meadow::Sound* sound = resMan.getSound(s);
    if (sound != nullptr) {
        auto it = m_sounds.find(s);
        if (it == m_sounds.end()) {
            LoggerLocator::getLogger()->getLogger()->error("Failed to play sound {}: sound has not been created", s);
            return;
        }
        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = m_system->playSound(it->second, nullptr, false, &channel);
        if (result != FMOD_OK) {
            LoggerLocator::getLogger()->getLogger()->error("Failed to play sound {}", s);
        }
        channel->setVolume(0.5f);
    }
    else
    {
        LoggerLocator::getLogger()->getLogger()->info("AudioSystem: there is no sound {}", s);
    }
}

bool AudioSystem::createSound(unsigned int s, std::string path)
{
    FMOD::Sound* sound;
    FMOD_RESULT result = m_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound);
    if (result != FMOD_OK) {
        LoggerLocator::getLogger()->getLogger()->error("Failed to create sound {} at path {}", s, path);
        return false;
    }
    else {
        m_sounds[s] = sound;
        return true;
    }
}

AudioSystem::~AudioSystem()
{
    if (m_system != NULL)
        m_system->release();
}
