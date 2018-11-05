#include "ModuleImportMeshes.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "PanelInspector.h"

#include "Application.h"
#include "Globals.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include <iostream>

void myCallback(const char *msg, char *userData) {

	LOG("%s", msg);
}

ModuleImportMeshes::ModuleImportMeshes (Application*app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleImportMeshes ::~ModuleImportMeshes ()
{

}

bool ModuleImportMeshes ::Init()
{
	bool ret = true;

	ilInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleImportMeshes ::Start()
{
	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	return true;
}

bool ModuleImportMeshes ::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleImportMeshes::Import(const char * path, std::string & output_file)
{
	bool ret = false;
	char* buffer = nullptr;

	uint size = App->fs->Load(path, &buffer);


	if (size > 0)
	{
		ret = Import(buffer, size, output_file, path);
	}
	else
	{
		LOG("ERROR LOADING MESH %s", path);
		return false;
	}
}

bool ModuleImportMeshes::Import(const void * buffer, uint size, std::string & output_file, const char* path)
{
	bool ret = false; 

	const aiScene* scene = aiImportFileFromMemory((const char*)buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* main_node = scene->mRootNode;

		std::string tmp = path;

		GameObject* go = App->scene->CreateGameObject(App->scene->root, tmp.c_str());

		LoadChildren(scene, main_node, path, go);

		App->scene->selected = go;

		ret = true;
	}

	return ret;
}


void ModuleImportMeshes ::LoadChildren(const aiScene * scene, aiNode * node, const char * path, GameObject * obj)
{
	if (node != nullptr)
	{
		std::string new_path = path;
		for (int num_meshes = 0; num_meshes < node->mNumMeshes; ++num_meshes)
		{
			Mesh* mesh = new Mesh();

			mesh->path = path;
			std::string path_to_name = mesh->path;
			mesh->filename = path_to_name.erase(0, path_to_name.find_last_of("\\") + 1);

			GameObject* children = App->scene->CreateGameObject(obj, node->mName.C_Str());

			if (scene->mRootNode != nullptr) {
				aiVector3D scale;
				aiQuaternion rotation;
				aiVector3D position;

				node->mTransformation.Decompose(scale, rotation, position);

				//Component Transform
				ComponentTransform* transformComponent = (ComponentTransform*)children->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
				transformComponent->SetPosition(math::float3(position.x, position.y, position.z));
				transformComponent->SetRotation(math::float3(rotation.GetEuler().x, rotation.GetEuler().y, rotation.GetEuler().z));
				transformComponent->SetSize(math::float3(scale.x, scale.y, scale.z));

				mesh->scale = { scale.x, scale.y, scale.z };
				mesh->rotation = { rotation.x, rotation.y, rotation.z, rotation.w };
				mesh->position = { position.x,position.y, position.z };
			}

			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[num_meshes]];

			mesh->num_vertices = new_mesh->mNumVertices;
			mesh->vertices = new float3[mesh->num_vertices];
			memcpy(mesh->vertices, new_mesh->mVertices, sizeof(float)*mesh->num_vertices * 3);

			LOG("Added new mesh. Vertices = %d", mesh->num_vertices);

			if (new_mesh->HasFaces())
			{
				mesh->num_indices = new_mesh->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices];

				for (uint num_faces = 0; num_faces < new_mesh->mNumFaces; ++num_faces)
				{
					if (new_mesh->mFaces[num_faces].mNumIndices != 3)
					{
						LOG("Geometry face %i whit %i faces", num_faces, new_mesh->mFaces[num_faces].mNumIndices);
					}
					else {
						memcpy(&mesh->indices[num_faces * 3], new_mesh->mFaces[num_faces].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//Component Mesh
			ComponentMesh* meshComponent = (ComponentMesh*)children->NewComponent(Component::COMPONENT_TYPE::COMPONENT_MESH);
			meshComponent->AssignMesh(mesh);

			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &mesh->color) == aiReturn_FAILURE || mesh->color == aiColor4D(0, 0, 0, 1))
			{
				mesh->color = { 255.0f,255.0f,255.0f,255.0f };
			}
			aiColor4D* colors_mesh = *new_mesh->mColors;

			if (colors_mesh != nullptr)
			{
				mesh->colors = new float[mesh->num_vertices * 3];
				for (int num_color = 0; num_color < mesh->num_vertices; ++num_color)
				{
					memcpy(mesh->colors, &colors_mesh[num_color], sizeof(float)*mesh->num_vertices * 3);
				}
			}

			aiMaterial* tex = scene->mMaterials[new_mesh->mMaterialIndex];
			Texture* texture = new Texture();

			if (tex != nullptr)
			{
				aiString path;
				tex->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				if (path.length > 0)
				{
					std::string path_location = path.data;
					path_location.erase(0, path_location.find_last_of("\\") + 1);
					std::string folder = "Textures/";
					folder += path_location;
					ILuint id;
					ilGenImages(1, &id);
					ilBindImage(id);
					ilLoadImage(folder.c_str());

					mesh->id_texture = ilutGLBindTexImage();

					folder.clear();
					path_location.clear();
					path.Clear();
				}
			}

			if (new_mesh->HasTextureCoords(0))
			{
				mesh->num_texture = new_mesh->mNumVertices;
				mesh->texture = new float[mesh->num_texture * 2];
				LOG("New mesh with %d textures", mesh->num_texture);
				for (uint texCoord = 0; texCoord < new_mesh->mNumVertices; ++texCoord)
				{
					memcpy(&mesh->texture[texCoord * 2], &new_mesh->mTextureCoords[0][texCoord].x, sizeof(float));
					memcpy(&mesh->texture[(texCoord * 2) + 1], &new_mesh->mTextureCoords[0][texCoord].y, sizeof(float));
				}

			}	
			//Component Mesh
			ComponentMaterial* materialComponent = (ComponentMaterial*)children->NewComponent(Component::COMPONENT_TYPE::COMPONENT_MATERIAL);
			materialComponent->AssignTexture(texture);

			SetBuffers(mesh);

			char* buffer;
			App->fs->Load(path, &buffer);
			ExportNCL(buffer, mesh);

			App->renderer3D->AddMesh(mesh);
			App->renderer3D->AddTexture(texture);

			children->CreateBoundingBox(mesh);
		}

	}
	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		LoadChildren(scene, node->mChildren[i], path, obj);
	}
}

void ModuleImportMeshes::SetBuffers(Mesh * mesh)
{
	glGenBuffers(1, (GLuint*) &(mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(mesh->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(mesh->id_texture));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->num_texture, mesh->texture, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(mesh->id_color));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_color);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_color, mesh->colors, GL_STATIC_DRAW);

}

void ModuleImportMeshes::ExportNCL(const void * buffer, Mesh* mesh)
{
	if (buffer != nullptr)
	{
		uint ranges[3] = { mesh->num_vertices, mesh->num_indices, mesh->num_texture };

		uint size = sizeof(ranges) + sizeof(float) * mesh->num_vertices + sizeof(uint) * mesh->num_indices + sizeof(float)* mesh->num_texture;;

		char* data = new char[size]; // Allocate
		char* cursor = data;

		// First store ranges
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);

		cursor += bytes;

		//Store vertices
		bytes = sizeof(float) * mesh->num_vertices;
		memcpy(cursor, mesh->vertices, bytes);

		cursor += bytes;

		//Store indices
		bytes = sizeof(uint) * mesh->num_indices;
		memcpy(cursor, mesh->indices, bytes);

		cursor += bytes;

		bytes = sizeof(float)* mesh->num_texture;
		memcpy(cursor, mesh->texture, bytes);

		std::string output;
		App->fs->SavePath(output, data, size, LIBRARY_MESH_FOLDER, "mesh", "ncl");
	}
	else
	{
		LOG("ERROR LOADING MESH");
	}

}

Mesh * ModuleImportMeshes::ImportNCL(const char * path)
{
	Mesh* ret = nullptr;
	GameObject* go = App->scene->CreateGameObject(App->scene->root, "patata");
	char* buffer;
	uint size = App->fs->Load(path, &buffer);

	if (size > 0)
	{
		LOG("LOADING OWN MESH %s", path);
		ret = LoadNCL(buffer, size);
	}
	else
	{
		LOG("ERROR LOADING OWN MESH %s", path);
	}

	ComponentTransform* transform = (ComponentTransform*)go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
	ComponentMesh* meshComponent = (ComponentMesh*)go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_MESH);
	meshComponent->AssignMesh(ret);

	App->scene->selected = go;

	return ret;
}

Mesh * ModuleImportMeshes::LoadNCL(const void * buffer, uint size)
{
	Mesh* ret = new Mesh;
	char* cursor = (char*)buffer;

	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[3];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	cursor += bytes;

	ret->num_vertices	= ranges[0];
	ret->num_indices	= ranges[1];
	ret->num_texture	= ranges[2];

	//Load Vertices
	bytes = sizeof(float) * ret->num_vertices;
	ret->vertices = new float3[ret->num_vertices * 3];
	memcpy(ret->vertices, cursor, bytes);

	// Load indices
	cursor += bytes;
	bytes = sizeof(uint) * ret->num_indices;
	ret->indices = new uint[ret->num_indices];
	memcpy(ret->indices, cursor, bytes);

	//Load UV
	cursor += bytes;
	bytes = sizeof(uint) * ret->num_texture;
	ret->texture = new float[ret->num_texture];
	memcpy(ret->texture, cursor, bytes);

	App->renderer3D->AddMesh(ret);

	return ret;
}

void ModuleImportMeshes ::ShowMeshInformation()
{
	ComponentMesh* m = nullptr;
	ComponentMaterial* t = nullptr;
	ComponentTransform* transf = nullptr;

	Mesh* mesh_info = nullptr;
	Texture* tex_info = nullptr;

	for (std::vector<GameObject*>::const_iterator iterator = App->scene->selected->children.begin(); iterator != App->scene->selected->children.end(); ++iterator)
	{
		for (std::vector<Component*>::const_iterator it = (*iterator)->components.begin(); it != (*iterator)->components.end(); ++it)
		{
			if ((*it)->type == Component::COMPONENT_TYPE::COMPONENT_MESH) {
				m = (ComponentMesh*)(*it);
			}
			if ((*it)->type == Component::COMPONENT_TYPE::COMPONENT_MATERIAL) {
				t = (ComponentMaterial*)(*it);
			}
			if ((*it)->type == Component::COMPONENT_TYPE::COMPONENT_TRANSFORM) {
				transf = (ComponentTransform*)(*it);
			}
			if (m != nullptr && t != nullptr) {
				mesh_info = m->mesh;
				tex_info = t->texture;
			}
		}
	}

	if (mesh_info != nullptr)
	{
		ImGuiTreeNodeFlags flags = 0;

		flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (!ImGui::Begin("Inspector", &active))
		{
			ImGui::End();
		}
		else
		{
			uint vertice = mesh_info->num_vertices;
			uint index = mesh_info->num_indices;
			uint uv = mesh_info->num_texture;
			uint triangles = mesh_info->num_indices / 3;

			ImTextureID texture_id = (ImTextureID)tex_info->texture_path;

			if (ImGui::CollapsingHeader("Information"), ImGuiTreeNodeFlags_DefaultOpen)
			{
				ImGui::Text("File name: %s", mesh_info->filename.c_str());
				ImGui::Text("Path: %s", mesh_info->path.c_str());
			}
			if (ImGui::CollapsingHeader("Transformation"), flags)
			{
				//POSITION------------
				ImGui::Text("Position");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->position.x, 0.01);
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->position.y, 0.01);
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->position.z, 0.01);

				transf->SetPosition(transf->position);

				//ROTATION------------
				ImGui::Text("Rotation");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->rotation.x, 0.01);
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->rotation.y, 0.01);
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->rotation.z, 0.01);

				transf->SetRotation(transf->rotation);

				//SCALE------------
				ImGui::Text("Scale");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->size.x, 0.01);
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->size.y, 0.01);
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat("", &transf->size.z, 0.01);

				transf->SetSize(transf->size);

			}
			if (ImGui::CollapsingHeader("Mesh information"), flags)
			{
				ImGui::Text("Vertices %i", vertice);
				ImGui::Text("Index %i", index);
				ImGui::Text("UV's %i", uv);
				ImGui::Text("triangles %i", triangles);
			}
			if (ImGui::CollapsingHeader("Texture"), flags)
			{
				ImGui::Text("SIZE");
				ImGui::Text("Width: %i", App->texture->texture_width); ImGui::SameLine();
				ImGui::Text("Height: %i", App->texture->texture_height);
				ImGui::Image(texture_id, { 256,256 });
			}

			ImGui::End();
		}	
	}
}


const float3 ModuleImportMeshes ::GetFbxPosition(const Mesh* mesh)
{
	return mesh->position;
}

const float3 ModuleImportMeshes ::GetFbxScale(const Mesh* mesh)
{
	return mesh->scale;
}

const Quat ModuleImportMeshes ::GetFbxRotation(const Mesh* mesh)
{
	return mesh->rotation;
}
