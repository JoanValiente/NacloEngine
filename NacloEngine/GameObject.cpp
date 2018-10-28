#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

GameObject::GameObject(GameObject * parent, const char* name)
{
	this->parent = parent;
	this->name = name;

	if (parent != nullptr) {
		parent->children.push_back(this);
	}
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

Component* GameObject::NewComponent(Component::COMPONENT_TYPE type)
{
	Component* component = nullptr;

	switch (type)
	{
	case Component::COMPONENT_TYPE::COMPONENT_TRANSFORM:
		component = new ComponentTransform(this);
		break;

	case Component::COMPONENT_TYPE::COMPONENT_MESH:
		component = new ComponentMesh(this);
		break;

	case Component::COMPONENT_TYPE::COMPONENT_MATERIAL:
		component = new ComponentMaterial(this);
		break;

	case Component::COMPONENT_TYPE::COMPONENT_NONE:
		break;

	default:
		break;
	}

	if (component != nullptr)
	{
		component->container = this;
		this->components.push_back(component);
	}

	return component;
}

void GameObject::DeleteComponent(Component * component)
{
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->CleanUp();
		components.erase(it);
	}
}
