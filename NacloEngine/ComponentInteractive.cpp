#include "OpenGL.h"
#include "Component.h"
#include "ComponentInteractive.h"
#include "ComponentRectTransform.h"
#include "GameObject.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "Application.h"


ComponentInteractive::ComponentInteractive(GameObject * container) : Component(container)
{
}

ComponentInteractive::~ComponentInteractive()
{
}

void ComponentInteractive::Update(float dt)
{
}

void ComponentInteractive::UpdateInteractive()
{
	float width = container->rectTransform->width;
	float height = container->rectTransform->height;
	float x = container->rectTransform->localmatrix.TranslatePart().x;
	float y = container->rectTransform->localmatrix.TranslatePart().y;

	float left, right, top, bottom;

	left = x - width / 2;
	right = x + width / 2;
	top = y - height / 2;
	bottom = y + height / 2;

	SetValueOneToZero(left, top);
	SetValueOneToZero(right, bottom);


	float mouse_x = App->input->GetMouseX();
	float mouse_y = App->input->GetMouseY();

	SetMouseValueOneToZero(mouse_x, mouse_y);

	if (mouse_x >= left && mouse_y >= top && mouse_x <= right && mouse_y <= bottom)
	{
		LOG("HOVERED LMAO!!");
	}
}

void ComponentInteractive::SetValueOneToZero(float & x, float & y)
{
	x = (x / 50);
	y = (y / 50);

	x = (x + 1) / 2;
	y = (y + 1) / 2;

}

void ComponentInteractive::SetMouseValueOneToZero(float & x, float & y)
{
	x = x / App->window->width;
	y = y / App->window->height;
}


void ComponentInteractive::isHovered()
{

}

void ComponentInteractive::SaveComponent(Config & conf)
{
}

void ComponentInteractive::LoadComponent(Config & conf)
{
}
