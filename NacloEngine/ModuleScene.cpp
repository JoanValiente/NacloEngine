#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "Globals.h"
#include "MeshImporter.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Quadtree.h"
#include "PanelInspector.h"
#include "Config.h"

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

	root = new GameObject(nullptr, "root");
	ComponentTransform* root_transform = (ComponentTransform*)root->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
	
	main_camera = new GameObject(root, "Main Camera");
	ComponentTransform* camera_transform = (ComponentTransform*)main_camera->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);
	camera_transform->SetPosition(float3(5.0f, 5.0f, -25.0f));
	mainCamera = (ComponentCamera*)main_camera->NewComponent(Component::COMPONENT_TYPE::COMPONENT_CAMERA);
	main_camera->staticGO = false;

	quadtree = new Quadtree();
	
	char new_file[256];
	strcpy_s(new_file, 256, "test");
	strcat(new_file, ".json");

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	App->camera->CullingGameObjects(root);

	UpdateQuadtree();
	UpdateTransforms(root);

	return ret;
}

update_status ModuleScene::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	root->Update(dt);

	ImGui::ShowDemoWindow();

	App->imgui->inspector->DrawInspector();

	return ret;
}

update_status ModuleScene::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (App->engineState == ENGINE_STATE::EDITOR) {
		Grid->Render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

void ModuleScene::DeleteGameObject(GameObject * gameObject)
{
	for (std::vector<GameObject*>::const_iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if ((*it)->name == gameObject->name) {
			gameObjects.erase(it);
			(*it)->CleanUp();
		}
	}
}

void ModuleScene::UpdateQuadtree()
{
	quadtree->Clear();
	quadtree->Create(AABB(AABB(float3(-50, -10, -50), float3(50, 10, 50))));

	for (std::vector<GameObject*>::const_iterator it = gameObjects.begin(); it != gameObjects.end(); it++) {
		if ((*it)->staticGO) {
			quadtree->Insert((*it));
		}
	}
}


void ModuleScene::SetSelected(GameObject * obj_to_select)
{
	if (selected != nullptr)
	{
		this->selected->selected = false;
	}

	this->selected = obj_to_select;
	obj_to_select->selected = true;
}

GameObject * ModuleScene::GetSelected()
{
	return selected;
}

void ModuleScene::UpdateTransforms(GameObject* go)
{
	if (go->transform != nullptr) {
		go->transform->UpdateMatrix();
	}

	for (int i = 0; i < go->GetNumChildren(); i++)
	{
		UpdateTransforms(go->children[i]);
	}
}

void ModuleScene::DeleteAllGameObject()
{
	for (std::vector<GameObject*>::const_iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it)->CleanUp();
	}

	gameObjects.clear();
}

bool ModuleScene::CleanUp()
{
	LOG("Unloading Scene");

	RELEASE(Grid);
	RELEASE(quadtree);

#ifndef GAME_MODE
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif
	/*
	Config *file = new Config("test");

	file->SetArray("GAME OBJECTS");

	for (std::vector<GameObject*>::const_iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		if (*it != root)
			(*it)->SaveGO(file);
	}

	file->Save();*/

	for (std::vector<GameObject*>::const_iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it)->CleanUp();
	}

	gameObjects.clear();

	if (root != nullptr) {
		delete root;
	}



	return false;
}