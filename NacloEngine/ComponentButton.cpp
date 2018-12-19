#include "Glew/include/glew.h"
#include "Component.h"
#include "GameObject.h"
#include "ComponentButton.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "ModuleWindow.h"

ComponentButton::ComponentButton(GameObject * container) : Component(container)
{
	this->type = COMPONENT_BUTTON;
}

void ComponentButton::Update(float dt)
{
	DebugDraw();
	isHovered();
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

void ComponentButton::isHovered()
{
	float x = container->rectTransform->globalMatrix.TranslatePart().x;
	float y = container->rectTransform->globalMatrix.TranslatePart().y;

	SetValueOneToZero(x, y);

	float mouse_x = App->input->GetMouseX();
	float mouse_y = App->input->GetMouseY();

	SetMouseValueOneToZero(mouse_x, mouse_y);

	LOG("%f, %f", mouse_x, mouse_y);
	LOG("%f, %f", x, y);


	if (mouse_x >= x && mouse_y >= y)
	{
		LOG("HOVERED LMAO!!");
	}

	//	if ((App->input->GetMouseX > this->pos.x && App->input->GetMouseX < this->pos.x + button_rect.w) && (App->input->GetMouseY > this->pos.y && mouse_y < this->pos.y + button_rect.h))
}

void ComponentButton::SetValueOneToZero(float & x, float & y)
{
	x = (x / 50);
	y = (y / 50);

	x = (x + 1) / 2;
	y = (y + 1) / 2;

}

void ComponentButton::SetMouseValueOneToZero(float & x, float & y)
{
	x = x / App->window->width;
	y = y / App->window->height;
}


