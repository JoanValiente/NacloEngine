#include "ComponentMesh.h"
#include "OpenGL.h"
#include "GameObject.h"


ComponentMesh::ComponentMesh(GameObject * container) : Component(container)
{
	type = COMPONENT_MESH;
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::AssignMesh(Mesh* mesh)
{
	this->mesh = mesh;
}
