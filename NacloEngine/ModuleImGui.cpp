#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "MathGeoLib/MathGeoLib.h"

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
	grid->grid = true;

	//--------------------------------------------------------------------------------------------------------
	mPlane = new Plane(float3(0.0f, 1.0f, 0.0f), 1.0f);
	mSphere = new Sphere(float3(0.0f, 1.0f, 0.0f), 1.0f);
	mCapsule = new Capsule(LineSegment(float3((0.0f, 1.0f, 0.0f)), float3(1.0f, 2.0f, 1.0f)), 3.0f);
	mAabb = new AABB(float3(0.0f, 1.0f, 0.0f), float3(3.0f, 4.0f, 3.0f));
	mFrustum = new Frustum();
	mRay = new Ray(float3(0.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f).Normalized());
	mTriangle = new Triangle(float3(0.0f, 0.0f, 0.0f), float3(3.0f, 3.0f, 3.0f), float3(5.0f, 5.0f, 5.0f));
	//---------------------------------------------------------------------------------------------------------

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	srand(time(NULL));
	uint64_t seeds[2];
	seeds[0] = rand();
	seeds[1] = rand();
	pcg32_srandom_r(&rng, seeds[0], seeds[1]);

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
	Tools();

	ImGui::ShowDemoWindow(&show_demo_window);

	return ret;
}

update_status ModuleImgui::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	grid->Render();
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
	show_console_window = false;
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}

void ModuleImgui::ConsoleLog(const char * text)
{	
	console_logs.appendf(text);
	ConsoleScroll = true;	
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
				show_console_window = true;
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
				show_random_window = true;
			}
			if (ImGui::MenuItem("Intersections", NULL, false, true))
			{
				show_intersection_window = true;
			}
			if (ImGui::MenuItem("Configuration", NULL, false, true))
			{
				show_configuration_window = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", NULL, false, true))
			{
				show_about_window = true; 
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

void ModuleImgui::Tools()
{
	if (show_random_window)
	{
		Random_Number_Generator_Window();
	}

	if (show_intersection_window)
	{
		Intersection_Window();
	}

	if (show_configuration_window)
	{
		Configuration_Window();
	}
	if (show_about_window)
	{
		About_Window();
	}
	if(show_console_window)
	{
		Console_Window();
	}
}

void ModuleImgui::Random_Number_Generator_Window()
{
	static double random1 = 0.0f;
	static int random2 = 0;

	if (!ImGui::Begin(("Random Number Generator"), &show_random_window))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Generate a random number between 0 - 1");
		if (ImGui::Button("Generate!"))
		{
			random1 = ldexp(pcg32_random_r(&rng), -32);
		}
		ImGui::SameLine();
		ImGui::Text("number = %f", random1);

		ImGui::Text("Generate a random number between 0 - 100");
		if (ImGui::Button("Generate"))
		{
			random2 = (int)pcg32_boundedrand_r(&rng, 101);
		}
		ImGui::SameLine();
		ImGui::Text("number = %i", random2);

		ImGui::End();
	}
}

void ModuleImgui::Intersection_Window()
{
	static bool intersect_test = false;
	static char* intersection = "No intersection";
	static char* intersection_result = "NO";


	if (!ImGui::Begin("Intersection Test", &show_intersection_window))
	{
		ImGui::End();
	}
	else
	{
		if (ImGui::Button("Plane - Sphere"))
		{
			intersection = "Plane - Sphere";
			intersect_test = mPlane->Intersects(*mSphere);
			LOG("Plane - Sphere Button pressed");
		}
		ImGui::SameLine();
		if (ImGui::Button("Plane - AABB"))
		{
			intersection = "Plane - AABB";
			intersect_test = mPlane->Intersects(*mAabb);
		}
		if (ImGui::Button("Plane - Triangle"))
		{
			intersection = "Plane - Triangle";
			intersect_test = mPlane->Intersects(*mTriangle);
		}
		ImGui::SameLine();
		if (ImGui::Button("Plane - Ray"))
		{
			intersection = "Plane - Ray";
			intersect_test = mPlane->Intersects(*mRay);
		}
		if (ImGui::Button("AABB - Sphere"))
		{
			intersection = "AABB - Sphere";
			intersect_test = mAabb->Intersects(*mSphere);
		}
		ImGui::SameLine();
		if (ImGui::Button("AABB - Ray"))
		{
			intersection = "AABB - Ray";
			intersect_test = mAabb->Intersects(*mRay);
		}
		if (ImGui::Button("Capsule - Sphere"))
		{
			intersection = "Capsule - Sphere";
			intersect_test = mCapsule->Intersects(*mSphere);
		}
		ImGui::SameLine();
		if (ImGui::Button("Capsule - Plane"))
		{
			intersection = "Capsule - Plane";
			intersect_test = mCapsule->Intersects(*mPlane);
		}
		if (ImGui::Button("Capsule - Triangle"))
		{
			intersection = "Capsule - Triangle";
			intersect_test = mCapsule->Intersects(*mTriangle);
		}
		ImGui::SameLine();
		if (ImGui::Button("Capsule - AABB"))
		{
			intersection = "Capsule - AABB";
			intersect_test = mCapsule->Intersects(*mAabb);
		}
		if (ImGui::Button("AABB - Triengle"))
		{
			intersection = "AABB - Triengle";
			intersect_test = mAabb->Intersects(*mTriangle);
		}

		ImGui::NewLine();
		ImGui::Text(intersection);

		if (intersect_test == 0)
		{
			intersection_result = "No";
		}
		else
		{
			intersection_result = "Yes";
		}

		ImGui::Text(intersection_result);
		ImGui::End();
	}
}

void ModuleImgui::Configuration_Window()
{
	if (!ImGui::Begin("Configuration", &show_configuration_window))
	{
		ImGui::End();
	}
	else
	{
		if (ImGui::CollapsingHeader("Application"))
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


		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
			{
				App->window->SetBrightnes(App->window->window, App->window->brightness);
			}

			if (ImGui::SliderInt("Width", &App->window->width, 1, 1920) || ImGui::SliderInt("Height", &App->window->height, 1, 1080))
			{
				App->window->SetWindowSize(App->window->window, App->window->width, App->window->height);
			}
			ImGui::Spacing();

			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			{
				App->window->SetWindowFullscreen(App->window->window);
			}

			if (ImGui::Checkbox("Borderless", &App->window->borderless))
			{
				App->window->SetWindowBorderless(App->window->window);
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Restart to apply");
			}

			if (ImGui::Checkbox("Resizable", &App->window->resizable))
			{
				App->window->SetWindowResizable(App->window->window);
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Restart to apply");
			}

			if (ImGui::Checkbox("Fullscreen Desktop", &App->window->fullscreen_desktop))
			{
				App->window->SetWindowFullDesktop(App->window->window);
			}

		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL Version: ");
			ImGui::SameLine();

			SDL_version curren_version;
			SDL_VERSION(&curren_version);
			char sdlVer[38];
			sprintf_s(sdlVer, sizeof(sdlVer), "%d.%d.%d", curren_version.major, curren_version.minor, curren_version.patch);
			ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), sdlVer);
			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
	
			int cpu = SDL_GetCPUCount();
			char cpuVer[38];
			sprintf_s(cpuVer, sizeof(sdlVer), "%i", cpu);
			ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), cpuVer);
		

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
		
			cpu = SDL_GetSystemRAM() / 1000;
			sprintf_s(cpuVer, sizeof(sdlVer), "%i Gb", cpu);
			ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), cpuVer);

			ImGui::Text("Caps: ");
			ImGui::SameLine();

			if (SDL_HasRDTSC())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "RDTSC");
				ImGui::SameLine();
			}
			if (SDL_HasMMX())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "MMX");
				ImGui::SameLine();		 
			}							 
			if (SDL_HasAVX())			 
			{							 
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "AVX");
				ImGui::SameLine();		
			}							
			if (SDL_HasAVX2())			
			{							
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "AVX2");
				ImGui::SameLine();		
			}							
			if (SDL_HasSSE())			
			{							
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE");
				ImGui::SameLine();		
			}							
			if (SDL_HasSSE2())			
			{							
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE2");
				ImGui::SameLine();
			}
			if (SDL_HasSSE3())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE3");
				ImGui::SameLine();
			}
			if (SDL_HasSSE41())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE41");
				ImGui::SameLine();
			}
			if (SDL_HasSSE42())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE42");

			}
		}
		ImGui::End();
	}
}

void ModuleImgui::About_Window()
{
	if (ImGui::Begin("About"), &show_about_window)
	{
		ImGui::InputText("Engine Name", App->engine_name, 20);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Videogame Engine developed by 2 students from UPC CITM");


		ImGui::Text("AUTHORS");
		ImGui::Spacing();
		ImGui::Spacing();


		ImGui::Text("David Lozano Sanchez");
		ImGui::Text("Joan Valiente Lorite");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("LIBRARIES");
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("SDL2");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "https://www.libsdl.org/download-2.0.php", NULL, NULL, NULL);
		}
		ImGui::Text("ImGui");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "https://github.com/ocornut/imgui", NULL, NULL, NULL);
		}
		ImGui::Text("PCG");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "http://www.pcg-random.org/download.html", NULL, NULL, NULL);
		}
		ImGui::Text("MathGeoLib");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "https://github.com/juj/MathGeoLib", NULL, NULL, NULL);
		}
		ImGui::Text("MMGR");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", NULL, NULL, NULL);
		}

		}




		ImGui::End();
	
}

void ModuleImgui::Console_Window()
{
	ImGui::Begin("Console", &show_console_window);

	ImGui::TextUnformatted(console_logs.begin());
	if (ConsoleScroll)
		ImGui::SetScrollHere(1.0f);
	ConsoleScroll = false;
	ImGui::End();
}

void ModuleImgui::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}


