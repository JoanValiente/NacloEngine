#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__
#include "Module.h"

struct grid;
struct GameObject;
struct ComponentCamera;
struct Quadtree;

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

	void UpdateQuadtree();

	void SetSelected(GameObject* obj_to_select);
	GameObject* GetSelected();

	void UpdateTransforms(GameObject* go);
	void DeleteAllGameObject();

public:
	GameObject* root = nullptr;
	std::vector<GameObject*> gameObjects;
	GameObject* main_camera = nullptr;
	ComponentCamera* mainCamera = nullptr;

	Quadtree* quadtree = nullptr;

private:
	GameObject * selected = nullptr;
	grid * Grid;
};
#endif
