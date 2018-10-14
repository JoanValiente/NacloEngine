#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include <list>
#include <string>


class ModuleTextures : public Module
{
public:

	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Start();
	bool CleanUp();

	uint LoadTexture(const char* path);

	uint const LoadCheckersTexture();

	uint CreateCheckersTexture(const void* checkImage);

	void const ShowTextureInfo();


private:
	std::string texture_path;
	std::string texture_name;
	uint last_texture_loaded = 0;

};

#endif // __MODULETEXTURES_H__