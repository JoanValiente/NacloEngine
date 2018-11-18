#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "MeshImporter.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
#ifndef GAME_MODE
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			{
				mouse_z = e.wheel.y;
				break;
			}

			case SDL_MOUSEMOTION:
			{
				mouse_x = e.motion.x / SCREEN_SIZE;
				mouse_y = e.motion.y / SCREEN_SIZE;

				mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
				mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
				break;
			}

			case SDL_QUIT:
			{
				quit = true;
				break;
			}

			case SDL_WINDOWEVENT:
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED) {}
					//App->renderer3D->OnResize(e.window.data1, e.window.data2);
				break;
			}
			
#ifndef GAME_MODE
			case SDL_DROPFILE: 
			{     
				char* dropped_filedir = nullptr;
				dropped_filedir = e.drop.file;
				LoadDraggedFile(dropped_filedir);
				SDL_free(dropped_filedir);    // Free dropped_filedir memory

				break;
			}
#endif
		
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

#ifndef GAME_MODE
void const ModuleInput::ShowInputInfo()
{
	ImGui::Text("Mouse Position X %i", mouse_x);
	ImGui::SameLine();
	ImGui::Text("Mouse Position Y %i", mouse_y);
	ImGui::Separator();
	ImGui::Text("Mouse Motion X %i", mouse_x_motion);
	ImGui::SameLine();
	ImGui::Text("Mouse Motion Y %i", mouse_y_motion);
	ImGui::Separator();
}
#endif

void const ModuleInput::LoadDraggedFile(char * path)
{
	std::string file_path(path);
	size_t end_string = std::string::npos;

	if (file_path.find(".fbx") != end_string || file_path.find(".FBX") != end_string)
	{
		LOG("Loading FBX file");
		std::string output_file;
		App->meshes->Import(path, output_file);
	}

	else if (file_path.find(".ncl") != end_string)
	{
		LOG("Loading NCL file");
		App->meshes->ImportNCL(path);
	}

	else if (file_path.find(".png") != end_string || file_path.find(".PNG") != end_string)
	{
		LOG("Loading PNG file");
		App->renderer3D->AddTexture(path);
	}
	else if (file_path.find(".jpg") != end_string || file_path.find(".JPG") != end_string)
	{
		LOG("Loading JPG file");
		App->renderer3D->AddTexture(path);
	}
	else if (file_path.find(".dds") != end_string || file_path.find(".DDS") != end_string)
	{
		LOG("Loading DDS file");
		App->renderer3D->AddTexture(path);
	}
	else
	{
		LOG("ERROR, UNKNOWN FILE FORMAT")
	}
}
