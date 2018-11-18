#include "Application.h"
#include "ModuleFileSystem.h"
#include "physfs/include/physfs.h"

#include <time.h>
#include <list>


#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

#include "mmgr/mmgr.h"

ModuleFileSystem::ModuleFileSystem(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "File System";

	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	PHYSFS_init(nullptr);
	if (PHYSFS_setWriteDir(".") == 0) // create a directory (if it's point it take game as base directory)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	if (PHYSFS_mount("./Assets/", "Assets", 1) == 0) { //Add paths to physfs to search throught

		LOG("Physfs could not fin the path %s", PHYSFS_getLastError());

	}

	if (PHYSFS_mount("./Library/", "Library", 1) == 0) { //Add paths to physfs to search throught

		LOG("Physfs could not fin the path %s", PHYSFS_getLastError());

	}

}

ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}

bool ModuleFileSystem::Start(Config* conf)
{
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

bool ModuleFileSystem::Save(std::string & output, const void * buffer, uint size, const char * path, const char * prefix, const char * extension)
{
	char result[250];

	CreateNewDirectory(path);

	sprintf_s(result, 250, "%s%s_%lld.%s", path, prefix, App->fs->GenerateUID(), extension);

	if (SaveFile(result, buffer, size) > 0)
	{
		output = result;
		return true;
	}
	return false;
}

uint ModuleFileSystem::Load(const char* file, char** buffer)
{
	uint ret = 0;
	std::string name = file;
	NormalizePath(name);

	PHYSFS_file* fs_file = PHYSFS_openRead(name.c_str());
	
	if (fs_file == nullptr)
	{
		LOG("Creating a copy of the file on Assets folder");
		CopyFileToAssets(file, name);
		NormalizePath(name);
		fs_file = PHYSFS_openRead(name.c_str());
	}

	if (fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[size];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(*buffer);
			}
			else
				ret = readed;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
	{
		LOG("ERROR LOADING FILE %s", file);
	}

	return ret;
}
void ModuleFileSystem::NormalizePath(std::string &file)
{
	for (string::iterator iterator = file.begin(); iterator != file.end(); iterator++)
	{
		if (*iterator == '\\')
			*iterator = '/';
	}

	int erase = file.find("Game");
	if (erase >= 0)
	{
		file = file.erase(0, erase + 5);
	}
}

void ModuleFileSystem::CreateNewDirectory(const char * path) const
{
	PHYSFS_mkdir(path);
}

void ModuleFileSystem::CopyFileToAssets(const char * path, std::string &output_file)
{
	output_file = CreateNewFilePath(path);
	CopyFile(path, output_file.c_str(), 0);
}

std::string ModuleFileSystem::CreateNewFilePath(const char* path)
{
	std::string new_path = path;
	char* name = (char*)new_path.erase(0, new_path.find_last_of("\\") + 1).c_str();
	std::string extension_path = path;
	extension_path.erase(0, extension_path.find_last_of(".")).c_str();
	char* directory = nullptr;

	if (extension_path == ".fbx")
	{
		directory = "Assets\\Models";
		CreateNewDirectory(ASSETS_MESH_FOLDER);
	}
	else
	{
		directory = "Assets\\Textures";
		CreateNewDirectory(ASSETS_TEXTURES_FOLDER);

	}

	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	char final_path[250];

	sprintf_s(final_path, 250, "%s\\%s\\%s", NPath, directory, name);

	return final_path;
}

UID ModuleFileSystem::GenerateUID()
{
	return pcg32_random_r(&rng);;
}

char** ModuleFileSystem::GetFiles(const char * path)
{
	char** tmp = PHYSFS_enumerateFiles("Assets");

	if (tmp == NULL)
	{
		LOG("CAN'T GET FILES IN DIRECTORY");
	}

	return tmp;
	PHYSFS_freeList(tmp);
}

int ModuleFileSystem::Phys_DeleteFile(const char * filename)
{
	int ret = PHYSFS_delete(filename);

	if (ret == 0)
	{
		LOG("ERROR DELETING FILE OR DIRECTORY %s", PHYSFS_getLastError());
	}
	return ret;
}


uint ModuleFileSystem::SaveFile(const char* file, const void* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);


	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);
			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}


