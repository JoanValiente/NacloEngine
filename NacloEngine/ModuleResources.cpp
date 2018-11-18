#include "ModuleResources.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Resource.h"
#include "Config.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"

#include "mmgr/mmgr.h"

ModuleResources::ModuleResources(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Start(Config* conf)
{
	return true;
}

update_status ModuleResources::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	LOG("Unloading Resource Manager");

	for (map<UID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		RELEASE(it->second);

	resources.clear();

	return true;
}

void ModuleResources::LoadResources()
{
	char* buffer = nullptr;
	uint size = App->fs->Load(SETTINGS_FOLDER "resources.json", &buffer);

	if (buffer != nullptr && size > 0)
	{
		Config config(buffer);
		
		int resourcesSize = config.GetArraySize("Resources");
		for (int i = 0; i < resourcesSize; ++i)
		{
			Config resource(config.GetArray("Resources", i));
			Resource_Type type = (Resource_Type) resource.GetInt("Type");
			UID uid = resource.GetUID("UID");

			if (Get(uid) != nullptr)
			{
				LOG("Skipping duplicated resource id %llu", uid);
				continue;
			}

			Resource* res = CreateNewResource(type, uid);
			res->Load(config.GetArray("Resources", i));
		}
		RELEASE_ARRAY(buffer);
	}
}

void ModuleResources::SaveResources() const
{
	bool ret = true;

	Config save;
	save.SetArray("Resources");

	for (map<UID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		Config resource;
		it->second->Save(resource);
		save.NewArrayEntry(resource);		
	}

	//TODO David: The resource should save the UID of whatever it's referencing 
	save.Save();
}

uint ModuleResources::Find(const char * file_in_assets) const
{
	for (std::map<UID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
		if (strcmp(it->second->GetExportedFile(), file_in_assets) == 0)
			return it->first;

	return 0;
}

UID ModuleResources::ImportFile(const char * new_file_in_assets, bool force)
{
	UID ret = 0;

	if (force == true)
	{
		ret = Find(new_file_in_assets);

		if (ret != 0)
			return ret;
	}

	bool import_ok = false;
	string written_file;
	Resource_Type type = Resource_Type::RESOURCE_UNKNOWN;
	
	switch (type) {
		case Resource_Type::RESOURCE_MATERIAL: 
			//import_ok = App->texture->Import(new_file_in_assets, "", written_file); 
			break;
		case Resource_Type::RESOURCE_MESH:
			//import_ok = App->meshes->Import(new_file_in_assets, "", written_file);
			break;
	}
	if (import_ok == true) { // If export was successful, create a new resource
		Resource* res = CreateNewResource(type);
		res->file = new_file_in_assets;
		res->exported_file = written_file;
		ret = res->uid;
		LOG("Imported successful from [%s] to [%s]", res->GetFile(), res->GetExportedFile());
	}
	else {
		LOG("Importing of [%s] FAILED", new_file_in_assets);
	}
	

	return ret;

}

UID ModuleResources::GenerateNewUID()
{
	++last_uid;
	//SaveUID();
	return last_uid;
}

const Resource * ModuleResources::Get(UID uid) const
{
	Resource* ret = nullptr;

	if (resources.find(uid) != resources.end())
		ret = resources.at(uid);

	return ret;
}

Resource * ModuleResources::Get(UID uid)
{
	Resource* ret = nullptr;

	std::map<UID, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		ret = it->second;

	return ret;
}

Resource * ModuleResources::CreateNewResource(Resource_Type type, UID force_uid)
{
	Resource* ret = nullptr;
	UID uid;

	if (force_uid != 0 && Get(force_uid) == nullptr)
		uid = force_uid;
	else
		uid = GenerateNewUID();

	switch (type)
	{
	case Resource_Type::RESOURCE_MESH:
		ret = (Resource*) new ResourceMesh(uid, RESOURCE_MESH);
		break;
	case Resource_Type::RESOURCE_MATERIAL:
		ret = (Resource*) new ResourceMaterial(uid, RESOURCE_MATERIAL);
		break;
	default:
		break;
	}

	if (ret != nullptr)
		resources[uid] = ret;	

	return ret;
}

void ModuleResources::CollectResourcesByType(std::vector<const Resource*>& resources, Resource_Type type) const
{
	for (map<UID, Resource*>::const_iterator it = this->resources.begin(); it != this->resources.end(); ++it)
	{
		if (it->second->type == type)
			resources.push_back(it->second);
	}
}
