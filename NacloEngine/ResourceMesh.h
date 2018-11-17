#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__

#include "Resource.h"

#include "MeshImporter.h"


class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uid, Resource_Type type);
	~ResourceMesh();

	bool LoadInMemory();

public:

	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_texture = 0;
	uint num_texture = 0;
	float* texture = nullptr;

};

#endif
