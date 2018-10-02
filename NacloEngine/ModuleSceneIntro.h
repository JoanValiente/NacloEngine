#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include <list>


class ModuleSceneIntro : public Module
{
public:

	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool CleanUp();
	bool Start();

	update_status Update(float dt);
	update_status PostUpdate(float dt);
	

private:
	plane * grid;

	float vertexs[200];
	uint indices[8] = { 0,1,2 };
	uint my_id = 0;
	uint my_indices = 0;

};

#endif // __ModuleAudio_H__