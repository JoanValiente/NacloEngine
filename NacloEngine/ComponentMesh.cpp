#include "ComponentMesh.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Config.h"

ComponentMesh::ComponentMesh(GameObject * container) : Component(container)
{
	type = COMPONENT_MESH;
	active = true;
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::ShowInspector()
{
	uint vertice = mesh->num_vertices;
	uint index = mesh->num_indices;
	uint uv = mesh->num_texture;
	uint triangles = mesh->num_indices / 3;

	if (ImGui::CollapsingHeader("Mesh information"))
	{
		ImGui::Text("File name: %s", mesh->filename.c_str());
		ImGui::Text("Path: %s", mesh->path.c_str());

		ImGui::Text("Vertices %i", vertice);
		ImGui::Text("Index %i", index);
		ImGui::Text("UV's %i", uv);
		ImGui::Text("triangles %i", triangles);
	}

}

void ComponentMesh::AssignMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SaveComponent(Config & conf)
{
	conf.SetString("Name", mesh->filename.c_str());
}
