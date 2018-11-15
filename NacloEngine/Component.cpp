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

void Component::CleanUp()
{
	//delete container;
	//container = nullptr;
}
