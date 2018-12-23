#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "Component.h"
#include "ComponentImage.h"
#include "Config.h"
#include "Application.h"


ComponentCheckbox::ComponentCheckbox(GameObject * container) : ComponentInteractive(container)
{
	this->type = COMPONENT_CHECKBOX;

	if (container->parent != nullptr)
	{
		interactive = true;
		AddToTheList();
	}

	if (container->rectTransform != nullptr)
	{
		container->rectTransform->width = size.x;
		container->rectTransform->height = size.y;
	}

	if (container->image != nullptr)
	{
		target_graphic = container->image;
	}
	if (container->children.size() != 0)
	{
		if (container->children[0]->image != nullptr)
		{
			graphic = container->children[0]->image;
			graphic->container->rectTransform->width = size.x;
			graphic->container->rectTransform->height = size.y;
			graphic->active = false;
		}
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
	if (!added)
	{
		AddToTheList();
	}
	if (isOn)
	{
		ExecuteFunction();
	}

	if (graphic == nullptr)
	{
		if (container->children.size() != 0)
		{
			if (container->children[0]->image != nullptr)
			{
				graphic = container->children[0]->image;
				graphic->container->rectTransform->width = size.x;
				graphic->container->rectTransform->height = size.y;
				graphic->active = false;
			}
		}
	}
}

void ComponentCheckbox::DebugDraw()
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
	target_graphic->active = !target_graphic->active;
	graphic->active = !graphic->active;
	isOn = !isOn;
	ExecuteFunction();
}

void ComponentCheckbox::ExecuteFunction()
{
	App->FPS_cap = 60;
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


void ComponentCheckbox::SaveComponent(Config & conf)
{
	conf.SetBool("Interactable", interactive);
	conf.SetBool("Is On", isOn);
	conf.SetFloat4("Normal Color", float4(normal_color.x, normal_color.y, normal_color.z, normal_color.w));
	conf.SetFloat4("Highlighted Color", float4(highlighted_color.x, highlighted_color.y, highlighted_color.z, highlighted_color.w));
	conf.SetFloat4("Pressed Color", float4(pressed_color.x, pressed_color.y, pressed_color.z, pressed_color.w));
}

void ComponentCheckbox::LoadComponent(Config & conf)
{
	float4 aux;
	aux = conf.GetFloat4("Normal Color");
	FloatToImVec(aux, normal_color);
	aux = conf.GetFloat4("Highlighted Color");
	FloatToImVec(aux, highlighted_color);
	aux = conf.GetFloat4("Pressed Color");
	FloatToImVec(aux, pressed_color);

	interactive = conf.GetBool("Interactable");
	isOn = conf.GetBool("Is On");

}

void ComponentCheckbox::FloatToImVec(float4 aux, ImVec4 & vec)
{
	vec.x = aux.x;
	vec.y = aux.y;
	vec.z = aux.z;
	vec.w = aux.w;
}

