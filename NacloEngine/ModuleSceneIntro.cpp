#include "Application.h"
#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

bool ModuleSceneIntro::Start() 
{
	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

	grid = new plane(0, 1, 0, 0);
	grid->axis = true;
	grid->grid = true;

	return true;

}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	grid->Render();

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::CleanUp()
{
	return false;
}
