#include "Application.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "GameObject.h"
//#include "Button.h"
#include "ComponentUI.h"
#include "ComponentImage.h"
//#include "Label.h"
//#include "Slider.h"

ModuleUI::ModuleUI(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Init()
{
	bool ret = true;

	return ret;
}

bool ModuleUI::Start()
{
	return true;
}

update_status ModuleUI::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{

	for (std::vector<ComponentUI*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->Draw(dt);
			(*it)->Update(dt);
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}


bool ModuleUI::CleanUp()
{
	LOG("Freeing GUI");

	for (std::vector<ComponentUI*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			//App->tex->UnLoad((SDL_Texture*)(*it)->texture);
			RELEASE((*it));
		}
	}
	elements.clear();

	return true;
}



/*
Label* ModuleUI::AddLabel(int x, int y, char* text, uint colors, uint fonts, int size, UIElement* parent, Uint32 wrap)
{
	SDL_Color color;

	color = GetColor(colors);
	char* path = GetFont(fonts);
	_TTF_Font* font = App->font->Load(path, size);


	const SDL_Texture* tex = App->font->Print(text, color, font, wrap);

	Label* label = new Label(x, y, LABEL, tex, parent);
	elements.add((UIElement*)label);
	label->font = font;

	return label;
}


Button* ModuleUI::AddButton(int x, int y, SDL_Texture* texture, Module* callback, UIElement* parent)
{
	Button* button = new Button(x, y, BUTTON, texture, callback, parent);
	elements.add((UIElement*)button);

	return button;
}

Slider* ModuleUI::AddSlider(int x, int y, SDL_Texture* texture, Module* callback, UIElement* parent)
{
	Slider* slider = new Slider(x, y, SLIDER, texture, callback, parent);
	elements.add((UIElement*)slider);

	return slider;
}

void ModuleUI::DeleteUI(UIElement * element)
{
	int index = elements.find(element);
	if (index > -1)
		elements.del(UIElement.At(index));
}
*/


ComponentImage * ModuleUI::CreateImage(float x, float y, SDL_Texture * texture, ComponentUI * parent)
{
	GameObject* go = new GameObject(App->scene->root, "Image");
	//ComponentImage* image = new ComponentImage(go, x, y, UI_TYPE::IMAGE, texture, parent);
	//elements.push_back((ComponentUI*)image);

	return nullptr;
}

SDL_Color ModuleUI::GetColor(int color)
{
	SDL_Color ret;
	ret.a = 255;

	switch (color)
	{
	case VERY_BLACK:
		ret.r = 0;
		ret.g = 0;
		ret.b = 0;
		break;
	case BLACK:
		ret.r = 30;
		ret.g = 30;
		ret.b = 30;
		break;
	case WHITE:
		ret.r = 255;
		ret.g = 255;
		ret.b = 255;
		break;
	case RED:
		ret.r = 255;
		ret.g = 0;
		ret.b = 0;
		break;
	case BLUE:
		ret.r = 0;
		ret.g = 0;
		ret.b = 255;
		break;
	case GREEN:
		ret.r = 0;
		ret.g = 255;
		ret.b = 0;
		break;
	case YELLOW:
		ret.r = 218;
		ret.g = 202;
		ret.b = 124;
		break;
	default:
		break;
	}
	return ret;
}

char * ModuleUI::GetFont(uint font)
{
	char* path = nullptr;
	switch (font)
	{
	default:
		break;
	}
	return path;
}