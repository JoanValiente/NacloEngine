#include "Component.h"
#include "GameObject.h"
#include "Config.h"

Component::Component(GameObject * container)
{
	this->container = container;
}

Component::~Component()
{
}

void Component::Update(float dt)
{
}

void Component::ShowInspector()
{
}

void Component::SaveComponent(Config& conf)
{
	conf.SetInt("TYPE", type);
}

void Component::LoadComponent(Config & conf)
{
	type = (COMPONENT_TYPE)conf.GetInt("Type");
}

void Component::CleanUp()
{
	//delete container;
	//container = nullptr;
}
