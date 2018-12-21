#include "OpenGL.h"
#include "Component.h"
#include "ComponentInteractive.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
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

void ComponentInteractive::UpdateInteractive()
{
	float width = container->rectTransform->width;
	float height = container->rectTransform->height;
	float x = container->rectTransform->localmatrix.TranslatePart().x;
	float y = container->rectTransform->localmatrix.TranslatePart().y;

	float left, right, top, bottom;

	right = x - width / 2;
	left = x + width / 2;
	bottom = y - height / 2;
	top = y + height / 2;

	SetValueOneToZero(left, top);
	SetValueOneToZero(right, bottom);

	float mouse_x = App->input->GetMouseX();
	float mouse_y = App->input->GetMouseY();

	SetMouseValueOneToZero(mouse_x, mouse_y);

	if (interactive == true)
	{
		if (mouse_x >= left && mouse_y >= top && mouse_x <= right && mouse_y <= bottom)
		{

			if (state == NO_STATE)
			{
				state = ENTER;
				Enter();
			}
			if (state == ENTER)
			{
				state = HOVER;
			}

			if (state == HOVER)
			{
				Hover();
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
				{
					state = DOWN;
					OnClick();
				}
				if (App->input->GetMouseButton(SDL_MOUSEBUTTONUP))
				{
					state = HOVER;
				}
			}
		}
		else
		{
			if (state == HOVER)
			{
				state = EXIT;
				Exit();
			}
			else
			{
				state = NO_STATE;
			}
		}
	}
}

void ComponentInteractive::SetValueOneToZero(float & x, float & y)
{
	x = (x / 50);
	y = (y / 50);

	x = fabs((x - 1) / 2);
	y = fabs((y - 1) / 2);
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

ComponentCanvas * ComponentInteractive::GetCanvas()
{
	ComponentCanvas* ret = nullptr;
	GameObject* tmp = container;
	while (ret == nullptr)
	{
		if (tmp->parent->canvas != nullptr)
		{
			ret = tmp->parent->canvas;
		}
		else
		{
			if (tmp->parent != nullptr)
				tmp = tmp->parent;
			else
			{
				return nullptr;
				LOG("COULDN'T FIND GAME OBJECT CANVAS");
			}
		}
	}

	return ret;
}