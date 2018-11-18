#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "SceneSerialization.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ModuleInput.h"
#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelRandomNumberGenerator.h"
#include "PanelInspector.h"
#include "PanelHierarchy.h"
#include "PanelResources.h"
#include "PanelTime.h"
#include "mmgr/mmgr.h"


ModuleImgui::ModuleImgui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ImGui";
}

ModuleImgui::~ModuleImgui()
{}

bool ModuleImgui::Init(Config* conf)
{
	bool ret = true;

	panels.push_back(console = new PanelConsole());
	panels.push_back(configuration = new PanelConfiguration());
	panels.push_back(randomNumberGenerator = new PanelRandomNumberGenerator());
	panels.push_back(about = new PanelAbout());
	//panels.push_back(inspector = new PanelInspector());
	panels.push_back(hierarchy = new PanelHierarchy());
	panels.push_back(resources = new PanelResources());
	panels.push_back(time = new PanelTime());

	return ret;
}

// Load assets
bool ModuleImgui::Start(Config* conf)
{	
	LOG("Loading Intro assets");

	bool ret = true;

	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	explorer_path = NPath;
	explorer_path.erase(explorer_path.find_last_of("\\"), explorer_path.back());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	configuration->LoadHardwareInfo();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsLight();

	return ret;
}


update_status ModuleImgui::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	ImGuiShortCuts();

	return ret; 
}

// Update
update_status ModuleImgui::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	Show_Main_Menu_Bar();
	
	for (vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);

		if (panel->IsActive())
		{
			panel->Draw();
		}
	}

	return ret;
}

update_status ModuleImgui::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;


	return ret;
}

bool ModuleImgui::CleanUp()
{
	LOG("Unloading Intro scene");

	for (vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
		if ((*it) != nullptr) {
			delete (*it);
			(*it) = nullptr;
		}
	}
	panels.clear();

	console = nullptr;

	return true;
}

void ModuleImgui::Log(const char * text)
{
	if (console != nullptr)
		console->ConsoleLog(text);
}


bool ModuleImgui::Show_Main_Menu_Bar()
{
	bool ret = true;
	static bool save = false;
	static bool load = false;
	static TypeSave save_type = SAVE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Main Menu"))
		{
			if (ImGui::MenuItem("Open Explorer", NULL, false, true))
			{
				ShellExecuteA(NULL, "open", explorer_path.c_str() , NULL, NULL, SW_SHOWDEFAULT);
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				save_type = SAVE;
				save = true;
			}
			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
			{
				save_type = SAVE_AS;
				save = true;
			}
			if (ImGui::MenuItem("Load", "Ctrl+O"))
			{
				load = true;
			}
			if (ImGui::MenuItem("Console", NULL, false, true))
			{
				console->active = true;
			}
			if (ImGui::MenuItem("Close", NULL, false, true))
			{
				close_engine = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Random Number Generator", NULL, false, true))
			{
				randomNumberGenerator->active = true;
			}
			if (ImGui::MenuItem("Configuration", NULL, false, true))
			{
				configuration->active = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", NULL, false, true))
			{
				about->active = true; 
			}

			if (ImGui::BeginMenu("Help"))
			{

				if (ImGui::MenuItem("Documentation", NULL, false, true))
				{
					ShellExecuteA(NULL, NULL, "https://github.com/JoanValiente/NacloEngine", NULL, NULL, NULL);
				}

				if (ImGui::MenuItem("Download Last Version", NULL, false, true))
				{
					ShellExecuteA(NULL, NULL, "https://github.com/JoanValiente/NacloEngine/releases", NULL, NULL, NULL);
				}


				if (ImGui::MenuItem("Report Bug", NULL, false, true))
				{
					ShellExecuteA(NULL, NULL, "https://github.com/JoanValiente/NacloEngine/issues", NULL, NULL, NULL);
				}

				ImGui::EndMenu(); 
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (save)
	{
		save = App->sceneser->ShowSavingOption(save_type);
	}

	if (load)
	{
		load = App->sceneser->ShowLoadingOption();
	}

	return ret;
}

void const ModuleImgui::ImGuiShortCuts()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		configuration->active = !configuration->active;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		inspector->active = !inspector->active;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		console->active = !console->active;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		about->active = !about->active;
	}
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
			App->renderer3D->wire_mode = !App->renderer3D->wire_mode;
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
			App->renderer3D->ischecked = !App->renderer3D->ischecked;
	}
}

void ModuleImgui::ShowImGuiInfo()
{
	ImGuiStyle& style = ImGui::GetStyle();
	
	ImGui::ShowStyleSelector("ImGui Style");
	
	if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f))
	{
		style.GrabRounding = style.FrameRounding;
	}

	bool window_border = style.WindowBorderSize; 
	if (ImGui::Checkbox("WindowBorder", &window_border)) 
		style.WindowBorderSize = window_border; 
	ImGui::SameLine();

	bool frame_border = style.FrameBorderSize;
	if (ImGui::Checkbox("FrameBorder", &frame_border)) 
		style.FrameBorderSize = frame_border; 
}

