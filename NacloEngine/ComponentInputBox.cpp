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
			container->rectTransform->SetHeight(size.y);
			container->rectTransform->SetWidth(size.x);
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
	container->label->color = float4(0, 0, 0, 255);
	text = container->label;
	text->text_str = "Enter text";
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

	if (App->scene->uiGoSelected != nullptr && App->scene->uiGoSelected->goUID == this->container->goUID)
	{
		text->SetString(App->input->composition);
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			App->scene->uiGoSelected = nullptr;
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
		text->SetString("");
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
