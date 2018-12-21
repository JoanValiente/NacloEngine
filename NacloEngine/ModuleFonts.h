#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "Module.h"
#include <vector>

#define DEFAULT_FONT "Assets/Fonts/Arial.ttf"
#define DEFAULT_FONT_SIZE 12

typedef struct _TTF_Font TTF_Font;
struct Font
{
	TTF_Font* font = NULL;
	int size = 12;
};

class ModuleFonts : public Module
{
public:

	ModuleFonts(Application* app, bool start_enabled = true);
	virtual ~ModuleFonts();

	bool Init();
	bool CleanUp();

	// Load Font
	Font* const Load(const char* path, int size = 12);
	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	std::vector<Font*>	fonts;
	Font*			default = nullptr;
};


#endif // __j1FONTS_H__