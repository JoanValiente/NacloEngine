#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Start();
	bool CleanUp();

	bool SavePath(std::string& output, const void* buffer, uint size, const char* path, const char* prefix, const char* extension);
	uint SaveFile(const char* file, const void* buffer, unsigned int size, bool append) const;
	uint Load(const char* file, char** buffer) const;
	void NormalizePath(std::string &path) const;

private:

};

#endif