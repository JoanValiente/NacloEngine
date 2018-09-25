#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	imgui = new ModuleImgui(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	
	// ImGui
	AddModule(imgui);

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
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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
	imgui->ConsoleLog(text);
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}