#ifndef GAME_MODE
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "SceneSerialization.h"
#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ComponentRectTransform.h"
#include "ComponentImage.h"
#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelRandomNumberGenerator.h"
#include "PanelInspector.h"
#include "PanelHierarchy.h"
#include "PanelTime.h"
#endif


#ifndef GAME_MODE
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
	//panels.push_back(inspector = new PanelInspector());
	panels.push_back(hierarchy = new PanelHierarchy());
	panels.push_back(time = new PanelTime());

	return ret;
}

// Load assets
bool ModuleImgui::Start()
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
			if (ImGui::MenuItem("Save"))
			{
				save_type = SAVE;
				save = true;
			}
			if (ImGui::MenuItem("Save As"))
			{
				save_type = SAVE_AS;
				save = true;
			}
			if (ImGui::MenuItem("Load"))
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
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::MenuItem("Create Empty", NULL, false, true))
				{
					App->scene->CreateEmptyGO();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("UI"))
			{
				if (ImGui::MenuItem("Canvas", NULL, false, true))
				{
					App->scene->CreateCanvas();
				}
				if (ImGui::MenuItem("Image", NULL, false, true))
				{
					if (App->scene->canvas.empty()) {
						App->scene->CreateCanvas();
					}
					vector<GameObject*>::iterator it = App->scene->canvas.begin();
					GameObject* go = new GameObject((*it), "Image");
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_IMAGE);
				}
				if (ImGui::MenuItem("Button", NULL, false, true))
				{
					if (App->scene->canvas.empty()) {
						App->scene->CreateCanvas();
					}
					vector<GameObject*>::iterator it = App->scene->canvas.begin();
					GameObject* go = new GameObject((*it), "Button");
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_IMAGE);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_BUTTON);
				}
				if (ImGui::MenuItem("CheckBox", NULL, false, true))
				{
					if (App->scene->canvas.empty()) {
						App->scene->CreateCanvas();
					}
					vector<GameObject*>::iterator it = App->scene->canvas.begin();
					GameObject* go = new GameObject((*it), "Checkbox");
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_IMAGE);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_CHECKBOX);
				}
				if (ImGui::MenuItem("Label", NULL, false, true))
				{
					if (App->scene->canvas.empty()) {
						App->scene->CreateCanvas();
					}
					vector<GameObject*>::iterator it = App->scene->canvas.begin();
					GameObject* go = new GameObject((*it), "Label");
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_LABEL);
				}
				if (ImGui::MenuItem("InputBox", NULL, false, true))
				{
					if (App->scene->canvas.empty()) {
						App->scene->CreateCanvas();
					}
					vector<GameObject*>::iterator it = App->scene->canvas.begin();
					GameObject* go = new GameObject((*it), "InputBox");
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_LABEL);
					go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_INPUTBOX);
					GameObject* go2 = new GameObject(go, "Image");					
					go2->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
					go2->NewComponent(Component::COMPONENT_TYPE::COMPONENT_IMAGE);
					go2->rectTransform->width = go->rectTransform->width;
					go2->rectTransform->height = go->rectTransform->height;
					go2->image->color = float4(255, 255, 255, 255);
				}
				ImGui::EndMenu();
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
		console->active = !console->active;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
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

#endif
