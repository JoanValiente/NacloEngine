#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include <list>

struct Cube;
struct plane;
struct ray;
struct Arrow;
struct sphere;
struct cylinder; 


class ModuleSceneIntro : public Module
{
public:

	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool CleanUp();
	bool Start();

	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void WireMode();
	
public:
	plane * grid;
	Cube * cube;
	ray * Ray;
	Arrow * arrow;
	sphere * test; 
	cylinder * Cylinder;

private:
	float vertexs[200];
	uint indices[8] = { 0,1,2 };
	uint my_id = 0;
	uint my_indices = 0;

	uint ImageName;

};

#endif // __ModuleAudio_H__