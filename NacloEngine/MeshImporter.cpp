#include "MeshImporter.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "PanelInspector.h"

#include "Application.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "TextureImporter.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


#include <iostream>

void myCallback(const char *msg, char *userData) {

	LOG("%s", msg);
}

MeshImporter::MeshImporter() : Importer()
{

}

MeshImporter ::~MeshImporter()
{

}

bool MeshImporter::Import(const char * path, std::string & output_file)
{
	bool ret = false;
	char* buffer = nullptr;

	uint size = App->fs->Load(path, &buffer);


	if (size > 0)
	{
		ret = Import(buffer, size, output_file, path);
		App->scene->quadtreeUpdate = true;
	}
	else
	{
		LOG("ERROR LOADING MESH %s", path);
		return false;
	}
	RELEASE_ARRAY(buffer);
}

bool MeshImporter::Import(const void * buffer, uint size, std::string & output_file, const char* path)
{
	bool ret = false; 

	const aiScene* scene = aiImportFileFromMemory((const char*)buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* main_node = scene->mRootNode;

		std::string tmp = path;

		GameObject* go = new GameObject(App->scene->root, tmp.c_str());
		aiVector3D scale;
		aiQuaternion rotation;
		aiVector3D position;

		main_node->mTransformation.Decompose(scale, rotation, position);

		//Component Transform

		ComponentTransform* transformComponent = (ComponentTransform*)go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
		transformComponent->SetPosition(math::float3(position.x, position.y, position.z));
		transformComponent->SetQuaternion({ rotation.x, rotation.y, rotation.z, rotation.w });
		transformComponent->SetSize(math::float3(scale.x, scale.y, scale.z));


		LoadMeshData(scene, main_node, path, go);


		App->scene->SetSelected(go);

		ret = true;
	}

	return ret;
}


void MeshImporter::LoadMeshData(const aiScene * scene, aiNode * node, const char * path, GameObject * obj)
{
	GameObject* final_obj;

	static int invalid_position = std::string::npos;
	 
	std::string name = node->mName.C_Str();
	if (name.length() == 0)
		name = "No Name";

	static const char* invalid_node_names[5] = { "$_PreRotation", "$_Rotation", "$_PostRotation",
		"$_Scaling", "$_Translation" };

	bool invalid_node = false;
	if (name.find(invalid_node_names[0]) != invalid_position || name.find(invalid_node_names[1]) != invalid_position || name.find(invalid_node_names[2]) != invalid_position
		|| name.find(invalid_node_names[3]) != invalid_position || name.find(invalid_node_names[4]) != invalid_position)
		invalid_node = true;

	if (!invalid_node && node->mNumMeshes > 0)
	{
			Mesh* mesh = new Mesh();

			mesh->path = path;
			std::string path_to_name = mesh->path;
			mesh->filename = path_to_name.erase(0, path_to_name.find_last_of("\\") + 1);
			


			GameObject* children = new GameObject(obj, name.c_str());

			if (scene->mRootNode != nullptr) {
				aiVector3D scale;
				aiQuaternion rotation;
				aiVector3D position;

				node->mTransformation.Decompose(scale, rotation, position);

				//Component Transform
				ComponentTransform* transformComponent = (ComponentTransform*)children->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
				transformComponent->SetPosition(math::float3(position.x, position.y, position.z));
				transformComponent->SetQuaternion({rotation.x, rotation.y, rotation.z, rotation.w});
				transformComponent->SetSize(math::float3(scale.x, scale.y, scale.z));
			}

			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[0]];

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
						invalid_node = true;
						LOG("Geometry face %i whit %i faces", num_faces, new_mesh->mFaces[num_faces].mNumIndices);
					}
					else {
						memcpy(&mesh->indices[num_faces * 3], new_mesh->mFaces[num_faces].mIndices, 3 * sizeof(uint));
					}
				}
			}

			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &mesh->color) == aiReturn_FAILURE || mesh->color == aiColor4D(0, 0, 0, 1))
			{
				mesh->color = { 255.0f,255.0f,255.0f,255.0f };
			}
			else
			{
				mesh->num_color = 4;
			}


			aiMaterial* tex = scene->mMaterials[new_mesh->mMaterialIndex];
			Texture* texture;

			//TODO REVISE THIS PART OF THE CODE
			if (tex != nullptr)
			{
				texture = GetTexture(tex, path);
			}

			App->renderer3D->AddTexture(texture);
			

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
			if (!invalid_node)
			{
				ComponentMesh* meshComponent = (ComponentMesh*)children->NewComponent(Component::COMPONENT_TYPE::COMPONENT_MESH);
				meshComponent->AssignMesh(mesh);

				//Component Material
				ComponentMaterial* materialComponent = (ComponentMaterial*)children->NewComponent(Component::COMPONENT_TYPE::COMPONENT_MATERIAL);
				materialComponent->AssignTexture(texture);
			}


			SetBuffers(mesh);

			LOG("Exporting mesh %s", node->mName.C_Str());
			char* buffer;
			App->fs->Load(path, &buffer);
			ExportNCL(buffer, mesh, mesh->ncl_path);
			RELEASE_ARRAY(buffer);

			final_obj = children;
	}

	else if(!invalid_node)
	{
		final_obj = new GameObject(obj, node->mName.C_Str());
		final_obj->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
	}
	else
	{
		final_obj = obj;
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		LoadMeshData(scene, node->mChildren[i], path, final_obj);
	}
}

Texture* MeshImporter::GetTexture(aiMaterial* tex, const char* path)
{
	Texture* ret = nullptr;

	std::string new_path = path;
	new_path.erase(new_path.find_last_of("\\") + 1, new_path.back());
	aiString file_name;
	tex->GetTexture(aiTextureType_DIFFUSE, 0, &file_name);
	if (file_name.length > 0)
	{
		std::string path_location = file_name.data;
		path_location.erase(0, path_location.find_last_of("\\") + 1);
		new_path.append(path_location.c_str());

		ret = App->texture->LoadTexture(path_location.c_str());

		if (ret == nullptr)
		{
			ret = App->texture->LoadTexture(new_path.c_str());
		}

		new_path.clear();
		path_location.clear();
	}
	file_name.Clear();




	return ret;
}


void MeshImporter::LoadMeshNCL(const char * path, Mesh * mesh)
{
	std::string new_path = path;

	mesh->path = path;
	std::string path_to_name = mesh->path;
	mesh->filename = path_to_name.erase(0, path_to_name.find_last_of("\\") + 1);


	aiVector3D scale = { 1.0f, 1.0f , 1.0f };
	aiQuaternion rotation = { 0.0f,0.0f,0.0f,0.0f };
	aiVector3D position = { 0.0f, 0.0f , 0.0f };;

	mesh->scale = { scale.x, scale.y, scale.z };
	mesh->rotation = { rotation.x, rotation.y, rotation.z, rotation.w };
	mesh->position = { position.x,position.y, position.z };

	mesh->color = { 255.0f,255.0f,255.0f,255.0f };

	SetBuffers(mesh);
	App->renderer3D->AddMesh(mesh);
}


void MeshImporter::SetBuffers(Mesh * mesh)
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

void MeshImporter::ExportNCL(const void * buffer, Mesh* mesh, std::string& output)
{
	if (buffer != nullptr)
	{
		uint ranges[3] = { mesh->num_vertices, mesh->num_indices, mesh->num_texture};

		uint size = sizeof(ranges) + sizeof(float3) * mesh->num_vertices * 3 + sizeof(uint) * mesh->num_indices + sizeof(float)* mesh->num_texture * 2;

		char* data = new char[size]; // Allocate
		char* cursor = data;

		// First store ranges
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);
		cursor += bytes;

		LOG("Stored ranges");

		//Store vertices
		bytes = sizeof(float) * mesh->num_vertices * 3;
		memcpy(cursor, mesh->vertices, bytes);

		cursor += bytes;

		LOG("Stored vertices");

		//Store indices
		bytes = sizeof(uint) * mesh->num_indices;
		memcpy(cursor, mesh->indices, bytes);

		cursor += bytes;

		LOG("Stored indices");

		//Store Uv
		bytes = sizeof(float)* mesh->num_texture * 2;
		memcpy(cursor, mesh->texture, bytes);


		LOG("Stored UV");

		App->fs->Save(output, data, size, LIBRARY_MESH_FOLDER, "mesh", "ncl");

		RELEASE_ARRAY(data);
	}
	else
	{
		LOG("ERROR LOADING MESH");
	}

}

//--------------------IMPORT OWN FORMAT--------------------

Mesh * MeshImporter::ImportNCL(const char * path)
{
	Mesh* ret = nullptr;
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

	LoadMeshNCL("patata1", ret);
	RELEASE_ARRAY(buffer);
	return ret;
}

Mesh * MeshImporter::LoadNCL(const void * buffer, uint size)
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
	bytes = sizeof(float) * ret->num_vertices * 3;
	ret->vertices = new float[ret->num_vertices * 3];
	memcpy(ret->vertices, cursor, bytes);

	// Load indices
	cursor += bytes;
	bytes = sizeof(uint) * ret->num_indices;
	ret->indices = new uint[ret->num_indices];
	memcpy(ret->indices, cursor, bytes);

	//Load UV
	cursor += bytes;
	bytes = sizeof(float) * ret->num_texture * 3;
	ret->texture = new float[ret->num_texture * 3];
	memcpy(ret->texture, cursor, bytes);

	App->renderer3D->AddMesh(ret);

	return ret;
}
