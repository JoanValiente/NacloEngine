#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetBrightnes(SDL_Window* window, float brightness);
	void SetWindowSize(SDL_Window* window, int width, int height);
	void SetWindowFullscreen(SDL_Window* window);
	void SetWindowResizable(SDL_Window* window);
	void SetWindowFullDesktop(SDL_Window* window);
	void SetWindowBorderless(SDL_Window* window);
	void const ShowWindowInfo();



public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface; 

	float brightness = 1.0f;

	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;

	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	bool fullscreen = false;
	bool borderless = false;
	bool resizable = false;
	bool fullscreen_desktop = false;
};

#endif // __ModuleWindow_H__