#include "ModuleImporter.h"

ModuleImporter::ModuleImporter(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	return true;
}

bool ModuleImporter::CleanUp()
{
	return true;
}
