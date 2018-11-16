#include "Resource.h"
#include "Config.h"

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

// ---------------------------------------------------------
void Resource::Save(Config & config) const
{
	config.SetUID("UID", uid);
	config.SetInt("Type", type);
	config.SetString("File", file.c_str());
	config.SetString("Exported", exported_file.c_str());
}

// ---------------------------------------------------------
void Resource::Load(const Config & config)
{
	uid = config.GetUID("UID");
	type = (Resource_Type)config.GetInt("Type");
	file = config.GetString("File");
	exported_file = config.GetString("Exported");
}

