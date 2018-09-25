#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			fullscreen = true;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
			resizable = true; 
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
			borderless = true;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			fullscreen_desktop = true; 
		}

		window = SDL_CreateWindow(App->engine_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		SDL_SetWindowBrightness(window, brightness);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);

}

void ModuleWindow::SetBrightnes(SDL_Window* window, float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
	SDL_UpdateWindowSurface(window);
}

void ModuleWindow::SetWindowSize(SDL_Window * window, int width, int height)
{
	SDL_SetWindowSize(window, width, height);
	SDL_UpdateWindowSurface(window);
}

void ModuleWindow::SetWindowFullscreen(SDL_Window * window)
{
	if (fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else 
		SDL_SetWindowFullscreen(window, flags);

}

void ModuleWindow::SetWindowResizable(SDL_Window * window)
{
	if (resizable)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_RESIZABLE);
	else 
		SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::SetWindowFullDesktop(SDL_Window * window)
{
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void ModuleWindow::SetWindowBorderless(SDL_Window * window)
{
	if (fullscreen_desktop)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_BORDERLESS);
	else
		SDL_SetWindowFullscreen(window, flags);
}

