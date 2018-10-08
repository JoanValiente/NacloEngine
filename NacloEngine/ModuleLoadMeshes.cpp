#include "ModuleLoadMeshes.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

ModuleLoadMeshes::ModuleLoadMeshes(Application*app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleLoadMeshes::~ModuleLoadMeshes()
{

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
					else
						memcpy(&mesh.indices[num_faces * 3], new_mesh->mFaces[num_faces].mIndices, 3 * sizeof(uint));

				}
			}

			if (new_mesh->HasTextureCoords(mesh.id_texture))
			{
				mesh.num_texture = new_mesh->mNumVertices;
				mesh.texture = new float[mesh.num_texture * 2];
				LOG("New mesh with %d textures", mesh.num_texture);
				for (uint num_textures = 0; num_textures < new_mesh->mNumVertices; ++num_textures)
				{
					memcpy(&mesh.texture[num_textures * 2], &new_mesh->mTextureCoords[num_textures], 2 * sizeof(float));
				}

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
		}

		glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh.id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);
	}// Use scene->mNumMeshes to iterate on scene->mMeshes array   aiReleaseImport(scene); } else   LOG(“Error loading scene %s”, full_path);
}
