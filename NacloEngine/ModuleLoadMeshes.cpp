#include "ModuleLoadMeshes.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "PanelInspector.h"

#include "Application.h"
#include "Globals.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include <iostream>

void myCallback(const char *msg, char *userData) {

	LOG("%s", msg);
}

ModuleLoadMeshes::ModuleLoadMeshes(Application*app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleLoadMeshes::~ModuleLoadMeshes()
{

}

bool ModuleLoadMeshes::Init()
{
	bool ret = true;

	ilInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleLoadMeshes::Start()
{
	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	return true;
}

bool ModuleLoadMeshes::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleLoadMeshes::LoadFBX(const char * path)
{

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes()) 
	{
		const aiNode* main_node = scene->mRootNode;

		std::string tmp = path;		

		GameObject* go = App->scene->CreateGameObject(App->scene->root, tmp.c_str());

		if (main_node != nullptr)
		{
			for (int num_meshes = 0; num_meshes < scene->mNumMeshes; ++num_meshes)
			{	
				Mesh* mesh = new Mesh();

				mesh->path = path;
				std::string path_to_name = mesh->path;
				mesh->filename = path_to_name.erase(0, path_to_name.find_last_of("\\") + 1);

				GameObject* children = App->scene->CreateGameObject(go, mesh->filename.c_str());

				if (scene->mRootNode != nullptr) {
					aiVector3D scale;
					aiQuaternion rotation;
					aiVector3D position;

					main_node->mTransformation.Decompose(scale, rotation, position);

					//Component Transform
					ComponentTransform* transformComponent = (ComponentTransform*)children->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
					transformComponent->SetPosition(math::float3(position.x, position.y, position.z));
					transformComponent->SetRotation(math::float3(rotation.GetEuler().x, rotation.GetEuler().y, rotation.GetEuler().z));
					transformComponent->SetSize(math::float3(scale.x, scale.y, scale.z));

					mesh->scale = { scale.x, scale.y, scale.z };
					mesh->rotation = { rotation.x, rotation.y, rotation.z, rotation.w };
					mesh->position = { position.x,position.y, position.z };
				}	

				aiMesh* new_mesh = scene->mMeshes[num_meshes];

				mesh->num_vertices = new_mesh->mNumVertices;
				mesh->vertices = new float[mesh->num_vertices * 3];
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

				aiMaterial* color_material = scene->mMaterials[new_mesh->mMaterialIndex];
				if (aiGetMaterialColor(color_material, AI_MATKEY_COLOR_AMBIENT, &mesh->color) == aiReturn_FAILURE || mesh->color == aiColor4D(0, 0, 0, 1))
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

				aiMaterial* tex = scene->mMaterials[0];

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

				App->renderer3D->AddMesh(mesh);

			}

		}
		App->scene->selected = go;
	}
}

void ModuleLoadMeshes::ShowMeshInformation()
{
	Mesh* mesh_info = *App->renderer3D->meshes.begin();
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
			float3 position = GetFbxPosition(mesh_info);
			Quat rotation = GetFbxRotation(mesh_info);
			float3 scale = GetFbxScale(mesh_info);

			uint vertice = mesh_info->num_vertices;
			uint index = mesh_info->num_indices;
			uint uv = mesh_info->num_texture;
			uint triangles = mesh_info->num_indices / 3;

			ImTextureID texture_id = (ImTextureID)mesh_info->texture_path;

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
				ImGui::InputFloat("", &position.x);
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat("", &position.y);
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat("", &position.z);
				//ROTATION------------
				ImGui::Text("Rotation");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat("", &rotation.x);
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat("", &rotation.y);
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat("", &rotation.z);
				//SCALE------------
				ImGui::Text("Scale");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat("", &scale.x);
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat("", &scale.y);
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat("", &scale.z);

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


const float3 ModuleLoadMeshes::GetFbxPosition(const Mesh* mesh)
{
	return mesh->position;
}

const float3 ModuleLoadMeshes::GetFbxScale(const Mesh* mesh)
{
	return mesh->scale;
}

const Quat ModuleLoadMeshes::GetFbxRotation(const Mesh* mesh)
{
	return mesh->rotation;
}
