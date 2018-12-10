#include "Glew/include/glew.h"
#include "Component.h"
#include "GameObject.h"
#include "ComponentButton.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"

ComponentButton::ComponentButton(GameObject * container) : Component(container)
{
	this->type = COMPONENT_BUTTON;
}

void ComponentButton::Update(float dt)
{
	canvas_size = SearchCanvas(container);
	DebugDraw();
}

void ComponentButton::ShowInspector()
{
}

void ComponentButton::DebugDraw()
{
	if (container->rectTransform != nullptr) {

		glBegin(GL_LINES);
		glLineWidth(8.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		float3 pos = container->rectTransform->position;
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

float2 ComponentButton::SearchCanvas(GameObject * go)
{
	float2 ret = float2::zero;

	if(go->parent->canvas != nullptr)
	{
		ret.x = go->parent->canvas->w;
		ret.y = go->parent->canvas->h;
	}
	else
	{
		ret = SearchCanvas(go->parent);
	}

	return ret;
}

void ComponentButton::isHovered()
{
	//	if ((App->input->GetMouseX > this->pos.x && App->input->GetMouseX < this->pos.x + button_rect.w) && (App->input->GetMouseY > this->pos.y && mouse_y < this->pos.y + button_rect.h))
}
