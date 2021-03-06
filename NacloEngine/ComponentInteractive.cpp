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
	float3 pos = float3::zero;
	Quat rot = Quat::identity;
	float3 size = float3::zero;
	container->rectTransform->GetLocalCanvasMatrix().Decompose(pos, rot, size);

	float width = container->rectTransform->width;
	float height = container->rectTransform->height;
	float size_x = size.x;
	float size_y = size.y;
	float x = pos.x;
	float y = pos.y;

	float left, right, top, bottom;

	right = x + width * size_x / 2;
	left = x - width * size_x / 2;
	bottom = y - height * size_y / 2;
	top = y + height * size_y / 2;

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
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					Down();
				}
				else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
				{ 
					Down();
					if (dragable)
					{
						Move();
					}
				}
				else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
				{
					if (!dragable)
					{
						OnClick();
					}
				}
				else
				{
					Hover();
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
				Idle();
			}
		}
	}
}

void ComponentInteractive::AddToTheList()
{
	incanvas = GetCanvas();
	if (incanvas != nullptr)
	{
		added = true;
		incanvas->interactive_components.push_back(this);
	}
	else
	{
		LOG("CANVAS NOT FOUND");
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


void ComponentInteractive::Move()
{
	float mouse_motion_x = App->input->GetMouseXMotion();
	float mouse_motion_y = App->input->GetMouseYMotion();

	container->rectTransform->position.x -= mouse_motion_x / 20;
	container->rectTransform->position.y -= mouse_motion_y / 10;
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
