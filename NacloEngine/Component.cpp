#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject * parent)
{
	this->parent = parent;
}

Component::~Component()
{
}

void Component::Update(float dt)
{
}

void Component::CleanUp()
{
	delete parent;
	parent = nullptr;
}
