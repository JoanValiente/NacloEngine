#include "GameObject.h"
#include "Glew/include/glew.h"

#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

GameObject::GameObject(GameObject * parent, const char* name)
{
	this->parent = parent;
	this->name = name;
	this->active = true;

	if (parent != nullptr) {
		parent->children.push_back(this);
	}
	
	boundingBox.SetNegativeInfinity();
}

GameObject::~GameObject()
{
	
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
		delete (*it);
	}

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		delete (*it);
	}
	
}

void GameObject::Update(float dt)
{
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->Update(dt);
	}

	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->Update(dt);
	}

	BoundingBoxDebugDraw();
}

void GameObject::CleanUp()
{
	for (std::vector<Component*>::const_iterator component_it = components.begin(); component_it != components.end(); ++component_it)
	{
		(*component_it)->CleanUp();
	}

	for (std::vector<GameObject*>::const_iterator go_it = children.begin(); go_it != children.end(); ++go_it)
	{
		(*go_it)->CleanUp();
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

	case Component::COMPONENT_TYPE::COMPONENT_CAMERA:
		component = new ComponentCamera(this);
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

Component * GameObject::GetComponentByType(Component::COMPONENT_TYPE type)
{
	Component* component = nullptr;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type)
			component = components[i];
	}

	return component;
}

void GameObject::CreateBoundingBox(Mesh * mesh)
{
	boundingBox.SetNegativeInfinity();
	boundingBox.Enclose((const float3*)mesh->vertices, mesh->num_vertices);
}

void GameObject::UpdateBoundingBox()
{
	boundingBox.SetNegativeInfinity();

	ComponentTransform* transform = (ComponentTransform*)GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
	ComponentMesh* mesh = (ComponentMesh*)GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_MESH);

	if (mesh != nullptr)
		boundingBox.Enclose((const float3*)mesh->mesh->vertices, mesh->mesh->num_vertices);

	if (transform != nullptr)
	{
		math::OBB obb = boundingBox.Transform(transform->matrix);

		if (obb.IsFinite())
			boundingBox = obb.MinimalEnclosingAABB();
	}

	for (uint i = 0; i < children.size(); ++i) {
		children[i]->UpdateBoundingBox();
	}
}

void GameObject::BoundingBoxDebugDraw()
{
	glBegin(GL_LINES);
	glLineWidth(2.0f);
	glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(boundingBox.Edge(i).a.x, boundingBox.Edge(i).a.y, boundingBox.Edge(i).a.z);
		glVertex3f(boundingBox.Edge(i).b.x, boundingBox.Edge(i).b.y, boundingBox.Edge(i).b.z);
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	   
}
