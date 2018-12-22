#include "ComponentInputBox.h"
#include "ComponentImage.h"
#include "ComponentLabel.h"
#include "ComponentRectTransform.h"
#include "ModuleFonts.h"
#include "GameObject.h"
#include "SDL/include/SDL.h"

ComponentInputBox::ComponentInputBox(GameObject * container) : Component(container)
{
	this->type = COMPONENT_IMAGE;

	if (container->rectTransform != nullptr)
	{
		container->rectTransform->SetHeight(10.0f);
		container->rectTransform->SetWidth(20.0f);

		container->rectTransform->UpdateMatrix();
	}
	else
	{
		LOG("Error creating Image Rect, no rect transform component created");
	}

	//image = new ComponentImage(container);
	text = new ComponentLabel(container);
}

ComponentInputBox::~ComponentInputBox()
{
}

void ComponentInputBox::Update(float dt)
{
	SDL_StartTextInput();
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN)
		{
			//Handle backspace
			if (e.key.keysym.sym == SDLK_BACKSPACE && composition.length() > 0)
			{
				//lop off character
				composition.pop_back();
				//renderText = true;
			}
			//Handle copy
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
			{
				SDL_SetClipboardText(composition.c_str());
			}
			//Handle paste
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				composition = SDL_GetClipboardText();
				//renderText = true;
			}

			//Special text input event
			else if (e.type == SDL_TEXTINPUT)
			{
				//Not copy or pasting
				if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
				{
					//Append character
					composition += e.text.text;
					//renderText = true;
				}
			}
		}
	}
	text->SetString(composition);
}

void ComponentInputBox::ShowInspector()
{
	if (ImGui::CollapsingHeader("InputBox"))
	{
	}

	text->ShowInspector();
}

void ComponentInputBox::SaveComponent(Config & conf)
{
}

void ComponentInputBox::LoadComponent(Config & conf)
{
}
