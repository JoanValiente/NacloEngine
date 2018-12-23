#include "ComponentInputBox.h"
#include "Application.h"
#include "ComponentImage.h"
#include "ComponentLabel.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
#include "ModuleFonts.h"
#include "GameObject.h"
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
	text->GenerateText();
	text->UpdateText();
	text->SetString(App->input->composition);
}

void ComponentInputBox::ShowInspector()
{
	if (ImGui::CollapsingHeader("InputBox"))
	{
	}

	text->ShowInspector();
}

void ComponentInputBox::Hover()
{
	LOG("Hover");
}

void ComponentInputBox::Enter()
{
}

void ComponentInputBox::Exit()
{
}

void ComponentInputBox::OnClick()
{
}

void ComponentInputBox::SaveComponent(Config & conf)
{
}

void ComponentInputBox::LoadComponent(Config & conf)
{
}
