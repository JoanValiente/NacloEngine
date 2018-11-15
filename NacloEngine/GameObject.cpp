#include "GameObject.h"
#include "Glew/include/glew.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ModuleScene.h"
#include "Config.h"


GameObject::GameObject(GameObject * parent, const char* name)
{
	this->parent = parent;
	this->name = name;
	this->active = true;
	this->staticGO = true;

	if (parent != nullptr)
	{
		parent->children.push_back(this);
		UID = App->fs->GenerateUID();
	}
	else
	{
		UID = 0;
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

	UpdateBoundingBox();
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
		this->transform = (ComponentTransform*)component;
		break;

	case Component::COMPONENT_TYPE::COMPONENT_MESH:
		component = new ComponentMesh(this);
		this->mesh = (ComponentMesh*)component;
		break;

	case Component::COMPONENT_TYPE::COMPONENT_MATERIAL:
		component = new ComponentMaterial(this);
		this->material = (ComponentMaterial*)component;
		break;

	case Component::COMPONENT_TYPE::COMPONENT_CAMERA:
		component = new ComponentCamera(this);
		this->camera = (ComponentCamera*)component;
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

/*
void GameObject::CreateBoundingBox(Mesh * mesh)
{
	boundingBox.SetNegativeInfinity();
	boundingBox.Enclose((const float3*)mesh->vertices, mesh->num_vertices);
}
*/

void GameObject::UpdateBoundingBox()
{
	boundingBox.SetNegativeInfinity();

	ComponentTransform* transform = (ComponentTransform*)GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
	ComponentMesh* mesh = (ComponentMesh*)GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_MESH);

	if (mesh != nullptr)
		boundingBox.Enclose((const float3*)mesh->mesh->vertices, mesh->mesh->num_vertices);

	if (transform != nullptr)
	{
		math::OBB obb = boundingBox.Transform(transform->globalMatrix);

		if (obb.IsFinite())
			boundingBox = obb.MinimalEnclosingAABB();
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

void GameObject::ChangeHierarchy(GameObject* &obj)
{
	if (!SearchForParent(obj, this))
	{
		GameObject* aux = new GameObject(this, obj->name.c_str());
		EqualGameObjects(obj, aux);

		std::vector<GameObject*>::const_iterator test;
		for (std::vector<GameObject*>::const_iterator it = obj->parent->children.begin(); it != obj->parent->children.end(); ++it)
		{
			if ((*it)->name == obj->name)
			{
				test = it;
			}
		}

		obj->parent->children.erase(test);

		
	}
}

uint GameObject::GetNumChildren()
{
	return children.size();
}


void GameObject::Inspector()
{
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->ShowInspector();
	}
}

bool GameObject::SaveGO(Config* & conf)
{
	Config go;
	go.SetString("Name", name.c_str());
	go.SetUID("UID", UID);
	go.SetUID("Parent UID", parent->UID);
	go.SetArray("COMPONENTS");

	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		Config component;
		component.SetInt("Type", (*it)->type);
		(*it)->SaveComponent(component);
		go.NewArrayEntry(component);
	}

	conf->NewArrayEntry(go);

	return true;
}

bool GameObject::SearchForParent(GameObject * parent, GameObject* child)
{
	bool ret = false;
	GameObject* iterator = child;

	while(iterator->name != App->scene->root->name)
	{
		if(iterator->parent->name == parent->name)
		{
			ret = true;
			iterator = iterator->parent;
		}
		else
		{
			iterator = iterator->parent;
		}
	}

	return ret;
}

void GameObject::EqualGameObjects(GameObject* copy, GameObject* &paste)
{
	paste->active		= copy->active;
	paste->boundingBox	= copy->boundingBox;
	paste->camera		= copy->camera;
	paste->children		= copy->children;
	paste->components	= copy->components;
	paste->transform	= copy->transform;
	paste->mesh			= copy->mesh;
	paste->camera		= copy->camera;
	paste->material		= copy->material;
	paste->staticGO		= copy->staticGO;


	for (int it = 0; it < components.size(); ++it)
	{
		paste->components.at(it)->container->parent = this;
	}
}