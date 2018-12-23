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
#include "Config.h"

ComponentInputBox::ComponentInputBox(GameObject * container) : ComponentInteractive(container)
{
	this->type = COMPONENT_INPUTBOX;

	if (container->rectTransform != nullptr)
	{
		if (!container->rectTransform->loading)
		{
			container->rectTransform->SetHeight(10.0f);
			container->rectTransform->SetWidth(20.0f);
		}

		container->rectTransform->UpdateMatrix();
	}
	else
	{
		LOG("Error creating Image Rect, no rect transform component created");
	}

	if (container->parent != nullptr)
	{
		interactive = true;
		AddToTheList();
	}

	//image = new ComponentImage(container);
	text = new ComponentLabel(container);
	text->text_str = "Enter text";
	App->input->composition = "Enter text";
	text->text = App->fonts->Load(DEFAULT_FONT, 48);
}

ComponentInputBox::~ComponentInputBox()
{
}

void ComponentInputBox::Update(float dt)
{
	if (!added)
	{
		AddToTheList();
	}

	if (text == nullptr)
	{
		if (container->label != nullptr)
		{
			text = container->label;
		}
	}
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
				(*it)->rectTransform->width = this->container->rectTransform->width + horizontalMargin;
				(*it)->rectTransform->height = this->container->rectTransform->height + verticalMargin;
			}
		}
	}
}

void ComponentInputBox::ShowInspector()
{
	float newHorizontalMargin = horizontalMargin;
	float newVerticalMargin = verticalMargin;

	if (ImGui::CollapsingHeader("InputBox"))
	{
		ImGui::Text("Horizontal Margin");
		ImGui::SameLine();
		if (ImGui::DragFloat("##horizontalMargin", &newHorizontalMargin, 0.1f)) {
			horizontalMargin = newHorizontalMargin;
		}

		ImGui::Text("Vertical Margin");
		ImGui::SameLine();
		if (ImGui::DragFloat("##verticalMargin", &newVerticalMargin, 0.1f)) {
			verticalMargin = newVerticalMargin;
		}

		text->ShowInspector();
	}
}

void ComponentInputBox::Hover()
{
	LOG("Patata");
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
		App->input->composition = "";
	}
}

void ComponentInputBox::SaveComponent(Config & conf)
{
	conf.SetBool("Interactive", interactive);
}

void ComponentInputBox::LoadComponent(Config & conf)
{
	interactive = conf.GetBool("Interactive");
}
