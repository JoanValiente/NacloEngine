#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "Globals.h"
#include "MeshImporter.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Quadtree.h"

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
	ComponentCamera* camera_component = (ComponentCamera*)main_camera->NewComponent(Component::COMPONENT_TYPE::COMPONENT_CAMERA);
	main_camera->staticGO = false;

	quadtree = new Quadtree();

	std::string otuput_file;
	App->meshes->Import("Assets/Models/BakerHouse.fbx", otuput_file);
	App->renderer3D->AddTexture("E:\\GitHub\\NacloEngine\\NacloEngine\\Game\\Assets\\Textures\\Baker_house.png");

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	App->camera->CullingGameObjects(root);

	UpdateQuadtree();

	return ret;
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

GameObject* ModuleScene::CreateGameObject(GameObject * parent, const char* name)
{
	GameObject* go = new GameObject(parent, name);

	gameObjects.push_back(go);

	return go;
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

bool ModuleScene::CleanUp()
{
	LOG("Unloading Scene");

	RELEASE(Grid);
	RELEASE(quadtree);

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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