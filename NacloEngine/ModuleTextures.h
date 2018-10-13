#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include <list>


class ModuleTextures : public Module
{
public:

	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Start();
	bool CleanUp();

	uint LoadTexture(const char* path);


private:

};

#endif // __MODULETEXTURES_H__