#include "Application.h"
#include "ModuleFonts.h"

#include "SDL\include\SDL.h"
#include "SDL2_ttf/include/SDL_ttf.h"
#pragma comment( lib, "SDL2_ttf/libx86/SDL2_ttf.lib" )

ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleFonts::~ModuleFonts()
{}

// Called before render is available
bool ModuleFonts::Init()
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = DEFAULT_FONT;
		int size = DEFAULT_FONT_SIZE;
		default = Load(path, size);
	}

	return ret;
}

// Called before quitting
bool ModuleFonts::CleanUp()
{
	LOG("Freeing True Type library");
	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
Font* const ModuleFonts::Load(const char* path, int size)
{
	Font* font = new Font;
	font->font = TTF_OpenFont(path, size);
	font->size = size;

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

// calculate size of a text
bool ModuleFonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default->font, text, &width, &height) != 0) {
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		ret = true;
	}

	return ret;
}