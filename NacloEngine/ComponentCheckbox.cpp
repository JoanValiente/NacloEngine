#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "Component.h"
#include "ComponentImage.h"


ComponentCheckbox::ComponentCheckbox(GameObject * container) : ComponentInteractive(container)
{
	this->type = COMPONENT_CHECKBOX;
	incanvas = GetCanvas();

	if (incanvas != nullptr)
	{
		interactive = true;
		incanvas->interactive_components.push_back(this);
	}

	if (container->image != nullptr)
	{
		target_graphic = container->image;
	}
}

ComponentCheckbox::~ComponentCheckbox()
{
}

void ComponentCheckbox::ShowInspector()
{
	if (ImGui::CollapsingHeader("Checkbox"))
	{
		ImGui::Checkbox("Interactable", &interactive);

		ImGui::Text("Target Graphic"); ImGui::SameLine();
		if (target_graphic != nullptr)
		{
			std::string name = target_graphic->container->name;
			name.append("(Image)");
			ImGui::InputText("##Image name", (char*)name.c_str(), 64);
		}
		else
		{
			ImGui::Text("None (Graphic)");
		}
		ImGui::Text("Normal Color      "); ImGui::SameLine();
		if (ImGui::ColorButton("##normal_color", normal_color, 0, ImVec2(200, 15)))
		{
			ImGui::OpenPopup("Normal_Color");
		}

		ImGui::Text("Highlighted Color "); ImGui::SameLine();
		if (ImGui::ColorButton("##highlighted_color", highlighted_color, 0, ImVec2(200, 15)))
		{
			ImGui::OpenPopup("Highlighted_Color");
		}

		ImGui::Text("Pressed Color     "); ImGui::SameLine();
		if (ImGui::ColorButton("##pressed_color", pressed_color, 0, ImVec2(200, 15)))
		{
			ImGui::OpenPopup("Pressed_Color");
		}

		ImGui::Checkbox("Is On", &isOn);
		if (graphic != nullptr)
		{
			std::string name = graphic->container->name;
			name.append("(Image)");
			ImGui::InputText("##Image name", (char*)name.c_str(), 64);
		}

		OpenPopUps();
	}
}

void ComponentCheckbox::Update(float dt)
{
}

void ComponentCheckbox::DebugDraw()
{
}

void ComponentCheckbox::SaveComponent(Config & conf)
{
}

void ComponentCheckbox::LoadComponent(Config & conf)
{
}

void ComponentCheckbox::Hover()
{
}

void ComponentCheckbox::Enter()
{
}

void ComponentCheckbox::Exit()
{
}

void ComponentCheckbox::OnClick()
{
	isOn = !isOn;
	ExecuteFunction();
}

void ComponentCheckbox::ExecuteFunction()
{
}

void ComponentCheckbox::OpenPopUps()
{
		float normal[4];
		float high[4];
		float pressed[4];

		if (ImGui::BeginPopup("Normal_Color"))
		{
			normal[0] = normal_color.x;
			normal[1] = normal_color.y;
			normal[2] = normal_color.z;
			normal[3] = normal_color.w;

			ImGui::ColorPicker4("##normal", normal);

			normal_color.x = normal[0];
			normal_color.y = normal[1];
			normal_color.z = normal[2];
			normal_color.w = normal[3];

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Highlighted_Color"))
		{
			high[0] = highlighted_color.x;
			high[1] = highlighted_color.y;
			high[2] = highlighted_color.z;
			high[3] = highlighted_color.w;

			ImGui::ColorPicker4("##high", high);

			highlighted_color.x = high[0];
			highlighted_color.y = high[1];
			highlighted_color.z = high[2];
			highlighted_color.w = high[3];

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Pressed_Color"))
		{
			pressed[0] = pressed_color.x;
			pressed[1] = pressed_color.y;
			pressed[2] = pressed_color.z;
			pressed[3] = pressed_color.w;

			ImGui::ColorPicker4("##pressed", pressed);

			pressed_color.x = pressed[0];
			pressed_color.y = pressed[1];
			pressed_color.z = pressed[2];
			pressed_color.w = pressed[3];

			ImGui::EndPopup();
		}
}
