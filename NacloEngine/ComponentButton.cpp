#include "Glew/include/glew.h"
#include "GameObject.h"
#include "ComponentButton.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "Component.h"
#include "ComponentImage.h"
#include "GameObject.h"

ComponentButton::ComponentButton(GameObject * container) : ComponentInteractive(container)
{
	this->type = COMPONENT_BUTTON;
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

ComponentButton::~ComponentButton()
{
}

void ComponentButton::Update(float dt)
{
	DebugDraw();
}

void ComponentButton::ShowInspector()
{
	if (ImGui::CollapsingHeader("Button"))
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

		OpenPopUps();
	}
}

void ComponentButton::DebugDraw()
{
	if (container->rectTransform != nullptr) {

		glBegin(GL_LINES);
		glLineWidth(8.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		float4x4 matrix = container->rectTransform->globalMatrix;

		float3 pos = { matrix[0][3], matrix[1][3], matrix[2][3] };
		float3 v1 = float3(pos.x - size.x, pos.y - size.y, pos.z);
		float3 v2 = float3(pos.x + size.x, pos.y - size.y, pos.z);
		float3 v3 = float3(pos.x + size.x, pos.y + size.y, pos.z);
		float3 v4 = float3(pos.x - size.x, pos.y + size.y, pos.z);
												   
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);
		glVertex3f(v4.x, v4.y, v4.z);
		glVertex3f(v1.x, v1.y, v1.z);

		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void ComponentButton::SaveComponent(Config & conf)
{
}

void ComponentButton::LoadComponent(Config & conf)
{
}

void ComponentButton::Idle()
{
	if (target_graphic != nullptr)
	{
		target_graphic->color.x = normal_color.x;
		target_graphic->color.y = normal_color.y;
		target_graphic->color.z = normal_color.z;
		target_graphic->color.w = normal_color.w;
	}
}

void ComponentButton::Enter()
{
}

void ComponentButton::Hover()
{
	if (target_graphic != nullptr)
	{
		target_graphic->color.x = highlighted_color.x;
		target_graphic->color.y = highlighted_color.y;
		target_graphic->color.z = highlighted_color.z;
		target_graphic->color.w = highlighted_color.w;
	}
}

void ComponentButton::Down()
{
	if (target_graphic != nullptr)
	{
		target_graphic->color.x = pressed_color.x;
		target_graphic->color.y = pressed_color.y;
		target_graphic->color.z = pressed_color.z;
		target_graphic->color.w = pressed_color.w;
	}
}

void ComponentButton::OnClick()
{
}

void ComponentButton::Exit()
{
}


void ComponentButton::OpenPopUps()
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

