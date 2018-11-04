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
	uint SaveFile(const char* file, const void* buffer, unsigned int size) const;
	uint Load(const char* file, char** buffer);
	void NormalizePath(std::string &file);
	void CreateNewDirectory(const char* path) const;
	void CopyFileToAssets(const char* path, std::string &output_file);
	std::string CreateNewFile(const char* path);

private:

};

#endif