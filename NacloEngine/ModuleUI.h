#ifndef _ModuleUI_H__
#define _ModuleUI_H__

#include "Module.h"
#include <list>

class UIElement;
class Label;
class Image;
class Button;
class Slider;

enum COLOURS
{
	VERY_BLACK,
	BLACK,
	WHITE,
	RED,
	BLUE,
	GREEN,
	YELLOW
};

enum UI_STATE {
	NO_STATE,
	MOUSE_ENTER,
	MOUSE_LEAVE,
	L_MOUSE_PRESSED,
	R_MOUSE_PRESSED,
	FOCUSED,
};

enum FONTS
{
	NO_FONT,
};

class ModuleUI : public Module
{
public:

	ModuleUI(Application* app, bool start_enabled = true);

	virtual ~ModuleUI();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

	/*
	Label* AddLabel(int x, int y, char* text, uint color, uint font, int size, UIElement* parent = nullptr, Uint32 wrap = 100);
	Image* AddImage(int x, int y, SDL_Texture* texture, UIElement* parent = nullptr);
	Button* AddButton(int x, int y, SDL_Texture* texture, Module* callback = nullptr, UIElement* parent = nullptr);
	Slider* AddSlider(int x, int y, SDL_Texture* texture, Module* callback = nullptr, UIElement* parent = nullptr);
	*/

	SDL_Color GetColor(int color);
	char* GetFont(uint font);

public:
	std::vector<UIElement*> elements;
};

#endif