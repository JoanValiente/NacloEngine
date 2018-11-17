#include "ResourceMaterial.h"
#include "ModuleResources.h"

ResourceMaterial::ResourceMaterial(uint uid, Resource_Type type) : Resource(uid, type)
{
}

ResourceMaterial::~ResourceMaterial()
{
}

bool ResourceMaterial::LoadInMemory()
{
	bool ret = true;

	return ret;
}

