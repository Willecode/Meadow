#include "camerasystem.h"
#include "ecs/components/camera.h"
#include "ecs/components/transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "shader/shadermanager.h"
#include "input/inputevents.h"
#include "service_locator/loggerlocator.h"


CameraSystem::CameraSystem():
    m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_editorCameraMode(true), m_editorCamera(EditorCamera(1080.f/1920.f, 0.1f,100.f)),
    m_activeCam(NullEntity)
{
    InputEvents::WindowDimensionsChangedEvent::subscribe(std::bind(&CameraSystem::setAspect, this, std::placeholders::_1, std::placeholders::_2));
    InputEvents::MouseLockEvent::subscribe(std::bind(&CameraSystem::cameraUnlock, this));
    InputEvents::MouseUnlockEvent::subscribe(std::bind(&CameraSystem::cameraLock, this));
    InputEvents::SceneCameraMakeActiveEvent::subscribe(std::bind(&CameraSystem::setActiveCamera, this, std::placeholders::_1));
    InputEvents::PlayGameEvent::subscribe(std::bind(&CameraSystem::useSceneCamera, this, std::placeholders::_1));

}

void CameraSystem::init(ECSCoordinator* ecs)
{
    m_ecs = ecs;
}

void CameraSystem::update(float deltaT, const InputGather& input)
{

    glm::mat4 viewMat(1.0f);
    glm::mat4 projMat(1.0f);
    glm::vec3 position(0.f);

    // Use editor cam
    if (m_editorCameraMode)
    {
        m_editorCamera.update(deltaT, input, m_cameraLock);
        viewMat = m_editorCamera.getViewMatrix();
        projMat = m_editorCamera.getProjectionMatrix();
        position = m_editorCamera.position;
    }
    // Use active camera component, right now just one cam possible
    else
    {
        if (m_activeCam != NullEntity) {
            Transform& trans = m_ecs->getComponent<Transform>(m_activeCam);
            Camera& cam = m_ecs->getComponent<Camera>(m_activeCam);

            glm::vec3 lookatPos;
            if (cam.inheritOrientation)
                lookatPos = glm::mat3(trans.modelMatrix) * glm::vec3(0.f, 0.f, -1.f);
            else
                lookatPos = rotateVecByQuat(glm::vec3(0.f, 0.f, -1.f), trans.orientation);


            position = trans.worldPos;
            viewMat = getViewMatrix(position, lookatPos);
            projMat = getProjectionMatrix(cam.fov, m_aspectRatio, cam.zNear, cam.zFar);
        }
        else
        {
            LoggerLocator::getLogger()->getLogger()->info("No active scene camera!");
        }

    }

    auto& sdrMan = ShaderManager::getInstance();

    // Set uniforms
    sdrMan.setFrameUniform("viewSkybox", glm::mat4(glm::mat3(viewMat)));
    sdrMan.setFrameUniform("view",       viewMat);
    sdrMan.setFrameUniform("projection", projMat);
    sdrMan.setFrameUniform("viewPos",    position);

    // Pass uniforms
    auto cameraDependantSdrs = sdrMan.getCameraDependant();
    for (int i = 0; i < cameraDependantSdrs->size(); i++) {
        sdrMan.bindShader((*cameraDependantSdrs)[i]);
        sdrMan.forwardFrameUniforms();
    }

    

}

void CameraSystem::setAspect(const int width, const int height)
{
    if (height != 0) // Prevent division by zero
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    m_editorCamera.setAspect(m_aspectRatio);
}

glm::mat4 CameraSystem::getViewMatrix(const glm::vec3& pos, const glm::vec3& dir)
{
    return glm::lookAt(pos, pos + dir, m_worldUp);
}

glm::mat4 CameraSystem::getProjectionMatrix(const float& fov, const float& aspect, const float& zNear, const float& zFar)
{
    return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}

void CameraSystem::cameraLock()
{
    m_cameraLock = true;
}

void CameraSystem::cameraUnlock()
{
    m_cameraLock = false;

}

void CameraSystem::setActiveCamera(Entity ent)
{
    m_activeCam = ent;
}

void CameraSystem::useSceneCamera(bool f)
{
    m_editorCameraMode = !f;
}

glm::vec3 CameraSystem::rotateVecByQuat(const glm::vec3& v, const glm::quat& q)
{
    // Extract the vector part of the quaternion
    glm::vec3 u(q.x, q.y, q.z);

    // Extract the scalar part of the quaternion
    float s = q.w;

    // Do the math
    return 2.0f * glm::dot(u, v) * u
        + (s * s - glm::dot(u, u)) * v
        + 2.0f * s * glm::cross(u, v);
}