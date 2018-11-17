#include "ResourceMesh.h"
#include "ModuleResources.h"

ResourceMesh::ResourceMesh(uint uid, Resource_Type type) : Resource(uid, type)
{
}

ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadInMemory()
{
	bool ret = false;

	glGenBuffers(1, (GLuint*) &(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertices, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) &(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) &(id_texture));
	glBindBuffer(GL_ARRAY_BUFFER, id_texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * num_texture, texture, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	return ret;
}
