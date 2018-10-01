#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imgui = new ModuleImgui(this);
	sceneintro = new ModuleSceneIntro(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

		// ImGui
	AddModule(imgui);

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(sceneintro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	list<Module*>::reverse_iterator item;
	item = modules.rbegin();

	while (item != modules.rend())
	{
		delete *item;
		item++;
	}
	modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	engine_name = "Naclo Engine";
	organization_name = "UPC CITM";
	author_1_name = "David Lozano Sanchez";
	author_2_name = "Joan Valiente Lorite";

	list<Module*>::const_iterator item;
	item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		if ((*item)->active)
			ret = (*item)->Init();

		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		if ((*item)->active)
			ret = (*item)->Start();

		item++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	last_ms = ms_timer.ReadMs();

	if (!vsync) {

		double ms_cap = 0;

		if (FPS_cap > 0) 
		{
			ms_cap = 1000 / FPS_cap;
		}

		else 
		{
			ms_cap = 1000 / 60;
		}

		if (last_ms < ms_cap)
		{
			SDL_Delay(ms_cap - last_ms);
		}

	}

	last_ms = ms_timer.ReadMs();
	last_FPS = 1000.0 / last_ms;
	dt = 1 / last_FPS;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list<Module*>::const_iterator item;
	item = modules.begin();
	
	while(item != modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = modules.begin();

	while(item != modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->active)
			ret = (*item)->Update(dt);

		item++;
	}

	item = modules.begin();

	while(item != modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->active)
			ret = (*item)->PostUpdate(dt);

		item++;
	}

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list<Module*>::reverse_iterator item;
	item = modules.rbegin();

	while(item != modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::Log(const char * text)
{
	imgui->Log(text);
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}