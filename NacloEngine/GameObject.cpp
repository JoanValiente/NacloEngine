#include "GameObject.h"

GameObject::GameObject(GameObject * parent, const char* name)
{
	this->parent = parent;
	this->name = name;
}

GameObject::~GameObject()
{
}

void GameObject::Update(float dt)
{
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->Update(dt);
	}
}

void GameObject::CleanUp()
{
	for (std::vector<GameObject*>::const_iterator go_it = children.begin(); go_it != children.end(); ++go_it)
	{
		(*go_it)->CleanUp();
	}

	for (std::vector<Component*>::const_iterator component_it = components.begin(); component_it != components.end(); ++component_it)
	{
		(*component_it)->CleanUp();
	}

	children.clear();
}

void GameObject::NewComponent(Component* component)
{
	if (component != nullptr)
	{
		component->container = this;
		components.push_back(component);
	}
}

void GameObject::DeleteComponent(Component * component)
{
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->CleanUp();
		components.erase(it);
	}
}
