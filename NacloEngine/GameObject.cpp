#include "GameObject.h"

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
