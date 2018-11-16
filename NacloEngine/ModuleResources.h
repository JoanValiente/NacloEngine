#ifndef __MODULERESOURCES_H__
#define __MODULERESOURCES_H__

#include "Module.h"
#include "Globals.h"

#include <map>
#include <vector>

class Resource;
enum Resource_Type;

class ModuleResources : public Module
{
public:

	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	uint Find(const char* file_in_assets) const;
	UID ImportFile(const char* new_file_in_assets, bool force = false);
	UID GenerateNewUID();
	const Resource* Get(UID uid) const;
	Resource* Get(UID uid);
	Resource* CreateNewResource(Resource_Type type, UID force_uid = 0);
	void CollectResourcesByType(std::vector<const Resource*>& resources, Resource_Type type) const;

private:

	UID last_uid = 1;
	std::map<UID, Resource*> resources;
};

#endif
