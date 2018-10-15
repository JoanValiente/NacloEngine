#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__
#include "Module.h"

struct grid;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();
	bool CleanUp();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);

private:
	grid * Grid;
};
#endif
