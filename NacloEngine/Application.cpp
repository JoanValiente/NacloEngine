#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleImgui.h"
#include "MeshImporter.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleFonts.h"
#include "ModuleResources.h"
#include "ModuleTimer.h"
#include "TextureImporter.h"
#include "ModuleFileSystem.h"
#include "SceneSerialization.h"

Application::Application()
{

#ifndef GAME_MODE
	imgui = new ModuleImgui(this);
	timer = new ModuleTimer(this);
#endif
	resources = new ModuleResources(this);
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene = new ModuleScene(this);
	fonts = new ModuleFonts(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	fs = new ModuleFileSystem(this);

	texture = new TextureImporter();
	meshes = new MeshImporter();
	sceneser = new SceneSerialization();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

		// ImGui
	// Main Modules
	AddModule(window);
	AddModule(fs);
	AddModule(resources);
	AddModule(input);
	AddModule(scene);
	AddModule(fonts);
	AddModule(camera);

#ifndef GAME_MODE
	AddModule(timer);
	AddModule(imgui);
#endif

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

	switch (engineState)
	{
	case EDITOR:

		switch (gameState)
		{
		case PLAY:
			engineState = ENGINE_STATE::GAME;
			gameState = GAME_STATE::NONE;
			break;
		case PAUSE:
			break;
		case STOP:
			break;
		default:
			break;
		}

		break;

	case GAME:

		switch (gameState)
		{
		case PLAY:
			gameState = GAME_STATE::NONE;
			break;
		case STOP:
			engineState = ENGINE_STATE::EDITOR;
			camera->activeCamera = camera->camera;
			break;
		case PAUSE:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
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

	timer->EndUpdate();

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

	delete texture;
	delete meshes;
	delete sceneser;

	return ret;
}

void Application::Log(const char * text)
{
	imgui->Log(text);
}

void const Application::ShowApplicationInfo()
{
	ImGui::InputText("Engine Name", App->engine_name, 20);
	ImGui::InputText("Organization", App->organization_name, 20);

	if (vector_fps.size() != 100)
	{
		vector_fps.push_back(ImGui::GetIO().Framerate);
		vector_ms.push_back(1000 / ImGui::GetIO().Framerate);
	}

	else
	{
		vector_fps.erase(vector_fps.begin());
		vector_fps.push_back(ImGui::GetIO().Framerate);

		vector_ms.erase(vector_ms.begin());
		vector_ms.push_back(1000 / ImGui::GetIO().Framerate);
	}

	char title[25];
	ImGui::Spacing();

	ImGui::SliderInt("FPS Cap", &App->FPS_cap, 0, 60);
	ImGui::Spacing();


	sprintf_s(title, 25, "Framerate %.1f", vector_fps[vector_fps.size() - 1]);
	ImGui::PlotHistogram("##framerate", &vector_fps[0], vector_fps.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	ImGui::Spacing();


	sprintf_s(title, 25, "Milliseconds %.1f", vector_ms[vector_ms.size() - 1]);
	ImGui::PlotHistogram("##milliseconds", &vector_ms[0], vector_ms.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
}

float Application::GetDt() const
{
	return dt;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}