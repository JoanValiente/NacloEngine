#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleWindow.h"
#include "MathGeoLib/MathGeoLib.h"

#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelRandomNumberGenerator.h"
#include "PanelInspector.h"


ModuleImgui::ModuleImgui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImgui::~ModuleImgui()
{}

bool ModuleImgui::Init()
{
	bool ret = true;

	panels.push_back(console = new PanelConsole());
	panels.push_back(configuration = new PanelConfiguration());
	panels.push_back(randomNumberGenerator = new PanelRandomNumberGenerator());
	panels.push_back(about = new PanelAbout());
	panels.push_back(inspector = new PanelInspector());
	return ret;
}

// Load assets
bool ModuleImgui::Start()
{	
	LOG("Loading Intro assets");

	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	configuration->LoadHardwareInfo();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	return ret;
}


update_status ModuleImgui::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return ret; 
}

// Update
update_status ModuleImgui::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	close_engine = Show_Main_Menu_Bar();
	
	for (vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);

		if (panel->IsActive())
		{
			panel->Draw();
		}
	}

	ImGui::ShowDemoWindow(&show_demo_window);

	return ret;
}

update_status ModuleImgui::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	if (!close_engine)
	{
		ret = UPDATE_STOP;
	}

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
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Main Menu"))
		{
			if (ImGui::MenuItem("Open", NULL, false, true))
			{
			}
			if (ImGui::MenuItem("Console", NULL, false, true))
			{
				console->active = true;
			}
			if (ImGui::MenuItem("Close", NULL, false, true))
			{
				ret = false;
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
			if (ImGui::Checkbox("Wire Mode", &wire_mode))
			{
				App->sceneintro->WireMode();
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

	return ret;
}

