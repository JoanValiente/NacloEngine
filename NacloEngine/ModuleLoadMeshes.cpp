#include "ModuleLoadMeshes.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


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
	aiLogStream stream = aiLogStream();
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleLoadMeshes::CleanUp()
{
	aiDetachAllLogStreams();

	if (mesh.indices != nullptr)
		delete[]mesh.indices;

	if (mesh.vertices != nullptr)
		delete[]mesh.vertices;


	return true;
}

void ModuleLoadMeshes::LoadFBX(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes()) {
		for (int num_meshes = 0; num_meshes < scene->mNumMeshes; ++num_meshes)
		{
			aiMesh* new_mesh = scene->mMeshes[num_meshes];
			mesh.num_vertices = new_mesh->mNumVertices;
			mesh.vertices = new float[mesh.num_vertices * 3];
			memcpy(mesh.vertices, new_mesh->mVertices, sizeof(float)*mesh.num_vertices * 3);
			LOG("New mesh with %d vertices", mesh.num_vertices);

			if (new_mesh->HasFaces())
			{
				mesh.num_indices = new_mesh->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices];

				for (uint num_faces = 0; num_faces < new_mesh->mNumFaces; ++num_faces)
				{
					if (new_mesh->mFaces[num_faces].mNumIndices != 3)
					{
						LOG("Geometry face %i whit %i faces", num_faces, new_mesh->mFaces[num_faces].mNumIndices);
					}
					else {
						memcpy(&mesh.indices[num_faces * 3], new_mesh->mFaces[num_faces].mIndices, 3 * sizeof(uint));
					}
				}
			}

			aiMaterial* color_material = scene->mMaterials[new_mesh->mMaterialIndex];
			if (aiGetMaterialColor(color_material, AI_MATKEY_COLOR_AMBIENT, &mesh.color) == aiReturn_FAILURE || mesh.color == aiColor4D(0, 0, 0, 1))
			{
				mesh.color = { 255.0f,255.0f,255.0f,255.0f };
			}
			aiColor4D* colors_mesh = *new_mesh->mColors;

			if (colors_mesh != nullptr)
			{
				mesh.colors = new float[mesh.num_vertices * 3];
				for (int num_color = 0; num_color < mesh.num_vertices; ++num_color)
				{
					memcpy(mesh.colors, &colors_mesh[num_color], sizeof(float)*mesh.num_vertices * 3);
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

					mesh.id_texture = ilutGLBindTexImage();

					folder.clear();
					path_location.clear();
					path.Clear();
				}
			}

			if (new_mesh->HasTextureCoords(mesh.id_texture))
			{
				mesh.num_texture = new_mesh->mNumVertices;
				mesh.texture = new float[mesh.num_texture * 2];
				LOG("New mesh with %d textures", mesh.num_texture);
				for (uint texCoord = 0; texCoord < new_mesh->mNumVertices; ++texCoord)
				{
					memcpy(&mesh.texture[texCoord * 2], &new_mesh->mTextureCoords[0][texCoord].x, 2 * sizeof(float));
					memcpy(&mesh.texture[texCoord * 2], &new_mesh->mTextureCoords[0][texCoord].y, 2 * sizeof(float));
				}

			}
		}

		glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh.id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh.id_texture));
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_texture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh.num_texture, mesh.texture, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh.id_color));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_color);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_color, mesh.colors, GL_STATIC_DRAW);

	}
}
