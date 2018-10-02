#include "OpenGL.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Globals.h"
#include "PanelOpenGL.h"

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

	grid = new plane(float3(0.0f, 0.0f, 0.0f), float3(100.0f, 0, 100.0f));
	grid->axis = true;
	grid->grid = true;

	cube = new Cube(float3(0.0f, 3.0f, 0.0f), float3(2.0f,2.0f,2.0f));
	cube->axis = true;

	Ray = new ray(float3(0.0f, 2.0f, 0.0f), float3(5.0f, 2.0f, 0.0f));

	return true;

}

update_status ModuleSceneIntro::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;


	return ret;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	grid->Render();
	cube->Render();
	Ray->Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

void ModuleSceneIntro::WireMode()
{
	if (App->imgui->wire_mode) {
		cube->wire = true;
	}
	else {
		cube->wire = false;
	}
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro Scene");
	
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return false;
}
