#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

#include <string>

struct Config;

enum Resource_Type {
	RESOURCE_MATERIAL,
	RESOURCE_MESH,
	RESOURCE_UNKNOWN
};


class Resource
{
public:
	Resource(UID uid, Resource_Type type);
	virtual ~Resource();

	Resource_Type GetType() const;
	UID GetUID() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	bool IsLoadedToMemory() const;
	uint LoadToMemory();
	uint CountReferences() const;

	void Save(Config& config) const;
	void Load(const Config& config);
	virtual bool LoadInMemory() = 0;

public:
	UID uid = 0;
	std::string file;
	std::string exported_file;
	Resource_Type type = RESOURCE_UNKNOWN;
	uint loaded = 0;
};


#endif // __RESOURCE_H__
