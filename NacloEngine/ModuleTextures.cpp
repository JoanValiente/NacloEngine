#include "ModuleTextures.h"

ModuleTextures::ModuleTextures(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Start()
{
	bool ret = true;
	return ret;
}

bool ModuleTextures::CleanUp()
{
	bool ret = true;
	return ret;
}

uint ModuleTextures::LoadTexture()
{
	return uint();
}
