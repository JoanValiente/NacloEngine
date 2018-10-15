#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "Globals.h"
#include "ModuleLoadMeshes.h"

ModuleScene::ModuleScene(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Start()
{
	Grid = new grid(0, 1, 0, 0);
	Grid->axis = true;

	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));	

	App->meshes->LoadFBX("Assets/Models/BakerHouse.fbx");
	App->renderer3D->AddTexture("Assets/Textures/Baker_house.png");
	return true;
}

update_status ModuleScene::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	Grid->Render();

	return ret;
}

update_status ModuleScene::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro Scene");

	return false;
}