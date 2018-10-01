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

	update_status PostUpdate(float dt);

private:
	plane * grid;

};

#endif // __ModuleAudio_H__