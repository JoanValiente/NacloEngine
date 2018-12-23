#include "ComponentInputBox.h"
#include "Application.h"
#include "ComponentImage.h"
#include "ComponentLabel.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
#include "ModuleFonts.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ComponentInputBox::ComponentInputBox(GameObject * container) : ComponentInteractive(container)
{
	this->type = COMPONENT_INPUTBOX;

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

	interactive = true;
	incanvas = GetCanvas();
	dragable = false;
	if (incanvas != nullptr)
	{
		interactive = true;
		incanvas->interactive_components.push_back(this);
	}
	//image = new ComponentImage(container);
	text = new ComponentLabel(container);
	text->text_str = "Input box";
	text->text = App->fonts->Load(DEFAULT_FONT, 48);
}

ComponentInputBox::~ComponentInputBox()
{
}

void ComponentInputBox::Update(float dt)
{
	//SDL_StartTextInput();
	/*
	SDL_PumpEvents();
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_TEXTINPUT:
		{
			//Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				//Append character
				composition += e.text.text;
				//renderText = true;
			}
			break;
		}
		}
		
	}
	*/
	if (App->scene->uiGoSelected != nullptr && App->scene->uiGoSelected->goUID == this->container->goUID) {
		text->SetString(App->input->composition);
		text->GenerateText();
		text->UpdateText();

		for (std::vector<GameObject*>::const_iterator it = this->container->children.begin(); it != this->container->children.end(); ++it) {
			if ((*it)->GetComponentByType(COMPONENT_TYPE::COMPONENT_IMAGE)) {
				(*it)->rectTransform->width = this->container->rectTransform->width;
				(*it)->rectTransform->height = this->container->rectTransform->height;
			}
		}
	}
}

void ComponentInputBox::ShowInspector()
{
	if (ImGui::CollapsingHeader("InputBox"))
	{
		text->ShowInspector();
	}
}

void ComponentInputBox::Hover()
{
}

void ComponentInputBox::Enter()
{
}

void ComponentInputBox::Exit()
{
}

void ComponentInputBox::OnClick()
{
	if (App->engineState == GAME) {
		LOG("Input Box Selected");
		App->scene->uiGoSelected = this->container;
	}
}

void ComponentInputBox::SaveComponent(Config & conf)
{
}

void ComponentInputBox::LoadComponent(Config & conf)
{
}
