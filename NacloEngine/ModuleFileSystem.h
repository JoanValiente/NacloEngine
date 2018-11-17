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


	bool Save(std::string& output, const void* buffer, uint size, const char* path, const char* prefix, const char* extension);
	uint SaveFile(const char* file, const void* buffer, unsigned int size) const;
	uint Load(const char* file, char** buffer);
	void NormalizePath(std::string &file);
	void CreateNewDirectory(const char* path) const;
	UID GenerateUID();

	void GetFiles(const char* path);

	int Phys_DeleteFile(const char* filename);

private:
	std::string CreateNewFilePath(const char* path);
	void CopyFileToAssets(const char* path, std::string &output_file);

private:
	pcg32_random_t rng;
};

#endif