#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(GameObject * parent)
{
	this->parent = parent;
}

GameObject::~GameObject()
{
}

bool GameObject::Update(float dt)
{
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->Update(dt);
	}
	
	return true;
}

bool GameObject::CleanUp()
{
	for (std::vector<GameObject*>::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->CleanUp();
	}
	children.clear();

	return true;
}

void GameObject::NewComponent(Component * component)
{
	components.push_back(component);
}

void GameObject::DeleteComponent(Component * component)
{
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->CleanUp();
		components.erase(it);
	}
}
