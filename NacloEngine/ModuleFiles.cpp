#include "Application.h"
#include "ModuleFiles.h"

ModuleFiles::ModuleFiles(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFiles::~ModuleFiles()
{
}

bool ModuleFiles::Start()
{
	bool ret = true;
	return ret;
}

bool ModuleFiles::CleanUp()
{
	bool ret = true;
	return ret;
}

bool ModuleFiles::LoadFBX()
{
	bool ret = true;
	return ret;
}
