#include "componenteventhandler.h"
#include "input/inputevents.h"
#include "resource_management/resourcemanager.h"

#include "ecs/components/light.h"
#include "ecs/components/model3d.h"
#include "ecs/components/camera.h"

void ComponentEventHandler::init(ECSCoordinator* ecs)
{
	m_ecs = ecs;

	InputEvents::AddLightComponentEvent::subscribe(std::bind(&ComponentEventHandler::addLightsourceEventHandler, this, std::placeholders::_1));
	InputEvents::RemoveLightComponentEvent::subscribe(std::bind(&ComponentEventHandler::removeLightsourceEventHandler, this, std::placeholders::_1));

	InputEvents::Add3DModelComponentEvent::subscribe(std::bind(&ComponentEventHandler::add3DModelEventHandler, this, std::placeholders::_1));
	InputEvents::Remove3DModelComponentEvent::subscribe(std::bind(&ComponentEventHandler::remove3DModelEventHandler, this, std::placeholders::_1));

	InputEvents::AddRigidBodyComponentEvent::subscribe(std::bind(&ComponentEventHandler::addRigidBodyEventHandler, this, std::placeholders::_1, std::placeholders::_2));
	InputEvents::removeRigidBodyComponentEvent::subscribe(std::bind(&ComponentEventHandler::removeRigidBodyEventHandler, this, std::placeholders::_1));

	InputEvents::AddCameraEvent::subscribe(std::bind(&ComponentEventHandler::addCameraEventHandler, this, std::placeholders::_1));
	InputEvents::RemoveCameraEvent::subscribe(std::bind(&ComponentEventHandler::removeCameraEventHandler, this, std::placeholders::_1));

	InputEvents::SetNodeMeshEvent::subscribe(std::bind(&ComponentEventHandler::setNodeMeshHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void ComponentEventHandler::addLightsourceEventHandler(Entity ent)
{
	Light l;
	m_ecs->addComponent<Light>(ent, l);
}

void ComponentEventHandler::add3DModelEventHandler(Entity ent)
{
	Model3D m;
	m_ecs->addComponent<Model3D>(ent, m);
}

void ComponentEventHandler::removeLightsourceEventHandler(Entity ent)
{
	m_ecs->removeComponent<Light>(ent);
}

void ComponentEventHandler::addRigidBodyEventHandler(Entity ent, RigidBody::RigidBodyType t)
{
	auto signature = m_ecs->getEntitySignature(ent);
	if (signature.test(m_ecs->getComponentType<RigidBody>())) {
		// remove existing rb
		m_ecs->removeComponent<RigidBody>(ent);
	}
	RigidBody r;
	r.type = t;
	m_ecs->addComponent(ent, r);
}

void ComponentEventHandler::removeRigidBodyEventHandler(Entity ent)
{
	m_ecs->removeComponent<RigidBody>(ent);
}

void ComponentEventHandler::addCameraEventHandler(Entity ent)
{
	Camera c;
	m_ecs->addComponent<Camera>(ent, c);
}

void ComponentEventHandler::removeCameraEventHandler(Entity ent)
{
	m_ecs->removeComponent<Camera>(ent);
}

void ComponentEventHandler::remove3DModelEventHandler(Entity ent)
{
	m_ecs->removeComponent<Model3D>(ent);

}

void ComponentEventHandler::setNodeMeshHandler(Entity ent, unsigned int meshid)
{
	auto& m = m_ecs->getComponent<Model3D>(ent);
	m.mesh = ResourceManager::getInstance().getMesh(meshid);
}
