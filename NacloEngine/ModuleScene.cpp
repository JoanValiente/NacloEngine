#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "Globals.h"
#include "ModuleLoadMeshes.h"
#include "GameObject.h"

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

	root = new GameObject(nullptr, "root");

	App->meshes->LoadFBX("Assets/Models/BakerHouse.fbx");
	App->renderer3D->AddTexture("Assets/Textures/Baker_house.png");

	return true;
}

update_status ModuleScene::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	root->Update(dt);

	return ret;
}

update_status ModuleScene::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	Grid->Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

void ModuleScene::CreateGameObject(GameObject * gameObject)
{
	gameObjects.push_back(gameObject);
}

void ModuleScene::DeleteGameObject(GameObject * gameObject)
{
	for (std::vector<GameObject*>::const_iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if ((*it) == gameObject) {
			(*it)->CleanUp();
			gameObjects.erase(it);
		}
	}
}

bool ModuleScene::CleanUp()
{
	LOG("Unloading Scene");

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	for (std::vector<GameObject*>::const_iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it)->CleanUp();
	}
	gameObjects.clear();

	return false;
}