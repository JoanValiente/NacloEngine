#include "ComponentMesh.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Config.h"
#include "Application.h"
#include "MeshImporter.h"

ComponentMesh::ComponentMesh(GameObject * container) : Component(container)
{
	type = COMPONENT_MESH;
	active = true;
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(mesh->vertices);
	RELEASE_ARRAY(mesh->indices);
	RELEASE_ARRAY(mesh->texture);
	delete mesh;
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
	conf.SetString("Path", mesh->path.c_str());
	conf.SetString("NCL Path", mesh->ncl_path.c_str());
}

void ComponentMesh::LoadComponent(Config & conf)
{
	mesh = App->meshes->ImportNCL(conf.GetString("NCL Path"));
	mesh->filename = conf.GetString("Name");
	mesh->path = conf.GetString("Path");
}
