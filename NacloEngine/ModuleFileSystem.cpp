#include "Application.h"
#include "ModuleFileSystem.h"
#include "physfs/include/physfs.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

ModuleFileSystem::ModuleFileSystem(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	PHYSFS_init(nullptr);

	if (PHYSFS_setWriteDir(".") == 0) // create a directory (if it's point it take game as base directory)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	if (PHYSFS_mount("./Assets/", "Assets", 1) == 0) { //Add paths to physfs to search throught

		LOG("Physfs could not fin the path %s", PHYSFS_getLastError());

	}

	if (PHYSFS_mount("./Assets/Textures/", "Textures", 1) == 0) { //Add paths to physfs to search throught

		LOG("Physfs could not fin the path %s", PHYSFS_getLastError());

	}

}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Start()
{
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

bool ModuleFileSystem::SavePath(std::string & output, const void * buffer, uint size, const char * path, const char * prefix, const char * extension)
{
	char result[250];

	sprintf_s(result, 250, "%s%s.%s", path, prefix, extension);

	if (SaveFile(result, buffer, size, false) > 0)
	{
		output = result;
		return true;
	}
	return false;
}

uint ModuleFileSystem::Load(const char* file, char** buffer) const
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

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
				RELEASE(buffer);
			}
			else
				ret = readed;
		}
		
		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

void ModuleFileSystem::NormalizePath(std::string & path) const
{
	for (string::iterator iterator = path.begin(); iterator != path.end(); iterator++)
	{
		if (*iterator == '\\')
			*iterator = '/';
	}

	path = path.erase(0, path.find("Game") + 5);

}


uint ModuleFileSystem::SaveFile(const char* file, const void* buffer, unsigned int size, bool append) const
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;

	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true)
			{
				LOG("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			}
			else if (overwrite == true)
			{
				LOG("File [%s%s] overwritten with %u bytes", PHYSFS_getWriteDir(), file, size);
			}
			else
			{
				LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);
			}

			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}


