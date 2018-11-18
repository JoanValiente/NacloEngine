#ifndef __Application_H__
#define __Application_H__


#include <list>
#include <vector>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "PerfTimer.h"

struct ModuleWindow;
struct ModuleInput;
struct ModuleImgui;
struct ModuleScene;
struct ModuleResources;
struct ModuleTimer;
struct ModuleRenderer3D;
struct ModuleCamera3D;
struct ModuleFileSystem;

struct SceneSerialization;
struct MeshImporter;
struct TextureImporter;

using namespace std;

enum ENGINE_STATE
{
	PLAY,
	PAUSE,
	STOP,
	TICK,
	EDITOR
};

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleImgui* imgui;
	ModuleScene* scene;
	ModuleResources* resources;
	ModuleTimer* timer;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleFileSystem* fs;

	SceneSerialization* sceneser;
	MeshImporter* meshes;
	TextureImporter* texture;


	int		FPS_cap = 60;
	bool	vsync = false;

	ENGINE_STATE engineState = ENGINE_STATE::EDITOR;
	PerfTimer	ms_timer;

private:	
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

	void Play();
	void Pause();
	void Stop();
	void Tick();

	void Log(const char* text);

	void const ShowApplicationInfo();
	float GetDt() const;

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