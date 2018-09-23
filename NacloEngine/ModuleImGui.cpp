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
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	//Random Number Generator
	
	if (show_random_window)
	{
		static int random1 = 0;
		static int random2 = 0;

		ImGui::Begin("Random Number Generator");
		ImGui::Text("Generate a random number between 0 - 1");
		if (ImGui::Button("Generate!"))
		{
			random1 = (int)pcg32_boundedrand_r(&rng, 2);
		}
		ImGui::SameLine();
		ImGui::Text("number = %i", random1);

		ImGui::Text("Generate a random number between 0 - 100");
		if (ImGui::Button("Generate"))
		{
			random2 = (int)pcg32_boundedrand_r(&rng, 101);
		}
		ImGui::SameLine();
		ImGui::Text("number = %i", random2);

		ImGui::End();
	}

	// Intersects

	if (show_intersection_window)
	{
		static bool intersect_test = false; 
		static char* intersection = "No intersection";
		static char* intersection_result = "NO";

		ImGui::Begin("Intersection Test");
		if (ImGui::Button("Plane - Sphere"))
		{
			intersection = "Plane - Sphere";
			intersect_test = mPlane->Intersects(*mSphere);
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
			intersection_result = "Yes";


		ImGui::Text(intersection_result);


		

		ImGui::End();
	}


	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

void ModuleImgui::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

