#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__
#include "Module.h"

struct grid;
struct GameObject;
struct Quadtree;
struct ComponentCamera;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();
	bool CleanUp();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void DeleteGameObject(GameObject* object);
	void CreateEmptyGO();
	void CreateCanvas();

	void UpdateQuadtree();

	void SetSelected(GameObject* obj_to_select);
	GameObject* GetSelected();

	void UpdateTransforms(GameObject* go);
	void DeleteAllGameObject();

	void DestroyAllGameObjects();

public:
	GameObject* root = nullptr;
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> canvas;
	GameObject* main_camera = nullptr;
	ComponentCamera* mainCamera = nullptr;
	GameObject* uiGoSelected = nullptr;

	Quadtree* quadtree = nullptr;
	bool quadtreeUpdate = true;

	bool show_quadtree = true;

	//FOR DEMO
	GameObject* optionsMenu = nullptr;

private:
	GameObject * selected = nullptr;
	GameObject * test = nullptr;
	grid * Grid;
};
#endif
