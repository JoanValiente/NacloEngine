#include "ModuleLoadMeshes.h"

#include "Assimp/include/cimport.h"
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

	return true;
}

void ModuleLoadMeshes::LoadFBX(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes()) {
		for (int num_meshes = 0; num_meshes < scene->mNumMeshes; ++num_meshes)
		{
			aiMesh* new_mesh = scene->mMeshes[num_meshes];
			num_vertices = new_mesh->mNumVertices;
			vertices = new float[num_vertices * 3];
			memcpy(vertices, new_mesh->mVertices, sizeof(float)*num_vertices * 3);
			LOG("New mesh with %d verices", num_vertices);

			if (new_mesh->HasFaces())
			{
				num_indices = new_mesh->mNumFaces * 3;
				indices = new uint[num_indices];

				for (uint num_faces = 0; num_faces < new_mesh->mNumFaces; ++num_faces)
				{
					if (new_mesh->mFaces[num_faces].mNumIndices != 3)
					{
						LOG("Geometry face %i whit %i faces", num_faces, new_mesh->mFaces[num_faces].mNumIndices);
					}
					else
						memcpy(&indices[num_faces * 3], new_mesh->mFaces[num_faces].mIndices, 3 * sizeof(uint));

				}
			}
		}

		glGenBuffers(1, (GLuint*) &(id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertices, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
	}// Use scene->mNumMeshes to iterate on scene->mMeshes array   aiReleaseImport(scene); } else   LOG(“Error loading scene %s”, full_path);
}
