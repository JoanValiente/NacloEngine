#include "Resource.h"

Resource::Resource(UID uid, Resource_Type type)
{
	this->uid = uid;
	this->type = type;
}

Resource::~Resource()
{
}

Resource_Type Resource::GetType() const
{
	return type;
}

UID Resource::GetUID() const
{
	return uid;
}

const char * Resource::GetFile() const
{
	return file.c_str();
}

const char * Resource::GetExportedFile() const
{
	return exported_file.c_str();
}

bool Resource::IsLoadedToMemory() const
{
	return loaded == 0;
}

bool Resource::LoadToMemory()
{
	if (IsLoadedToMemory())
		LoadInMemory();

	return loaded++;
}

uint Resource::CountReferences() const
{
	return loaded;
}
