#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__
#include "Module.h"

struct grid;
struct GameObject;
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

	GameObject* CreateGameObject(GameObject* parent, const char* name);
	void DeleteGameObject(GameObject* object);

	void UpdateQuadtree();

	void UpdateTransforms(GameObject* go);

public:
	GameObject* root = nullptr;
	std::vector<GameObject*> gameObjects;
	GameObject* main_camera = nullptr;
	GameObject* selected = nullptr;

	Quadtree* quadtree = nullptr;

private:
	grid * Grid;
};
#endif
