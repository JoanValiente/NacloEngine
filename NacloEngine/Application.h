#ifndef __Application_H__
#define __Application_H__


#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleImgui.h"
#include "MeshImporter.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleResources.h"
#include "TextureImporter.h"
#include "ModuleFileSystem.h"
#include "PerfTimer.h"


using namespace std;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleImgui* imgui;
	ModuleScene* scene;
	ModuleResources* resources;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleFileSystem* fs;

	MeshImporter* meshes;
	TextureImporter* texture;


	int		FPS_cap = 60;
	bool	vsync = false;

private:

	PerfTimer	ms_timer;
	double		last_FPS = 0.0f;
	double		last_ms = 0.0f;
	float		dt = 0.0f;

	list<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	char* engine_name;
	char* author_1_name;
	char* author_2_name;
	char* organization_name;

	void Log(const char* text);

	void const ShowApplicationInfo();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

private: 	
	std::vector<float> vector_ms;
	std::vector<float> vector_fps;
	std::vector<float> vector_memory_usage;
};

extern Application* App;

#endif