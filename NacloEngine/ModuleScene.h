#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__
#include "Module.h"

struct grid;
struct GameObject;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();
	bool CleanUp();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	GameObject* CreateGameObject(GameObject* parent, const char* name);
	void DeleteGameObject(GameObject* object);

public:
	GameObject* root = nullptr;
private:
	grid * Grid;
	std::vector<GameObject*> gameObjects;
};
#endif
