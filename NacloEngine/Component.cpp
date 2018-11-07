#include "Component.h"
#include "GameObject.h"

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

void Component::CleanUp()
{
	//delete container;
	//container = nullptr;
}
