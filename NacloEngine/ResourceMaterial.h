#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__

#include "Resource.h"

class ResourceMaterial : public Resource
{
public:

	ResourceMaterial(uint uid, Resource_Type type);
	~ResourceMaterial();

	bool LoadInMemory();

public:

	uint id_texture = 0;
	uint widht = 0;
	uint height = 0;
};

#endif
