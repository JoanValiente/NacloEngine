#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "MathGeoLib/MathGeoLib.h"
//#include "PhysBody3D.h"

#include <time.h>

ModuleImgui::ModuleImgui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImgui::~ModuleImgui()
{}

// Load assets
bool ModuleImgui::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

	grid = new plane(0, 1, 0, 0);
	grid->axis = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	return ret;
}

// Load assets
bool ModuleImgui::CleanUp()
{
	LOG("Unloading Intro scene");

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}

// Update
update_status ModuleImgui::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//Close Engine

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Main Menu"))
			{
				if (ImGui::MenuItem("Open", NULL, false, true))
				{
				}
				if (ImGui::MenuItem("Save", NULL, false, true))
				{
				}
				if (ImGui::MenuItem("Close", NULL, false, true))
				{
					ret = UPDATE_STOP;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

	
	ImGui::ShowDemoWindow(&show_demo_window);

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	grid->Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

void ModuleImgui::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

