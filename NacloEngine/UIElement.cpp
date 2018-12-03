#include "OpenGL.h"
#include "UIElement.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleUI.h"

UIElement::UIElement(UI_TYPE type, UIElement * parent)
{
	this->type = type;

	if (parent != nullptr)
		this->parent = parent;
	else
		parent = nullptr;

	if (parent != nullptr)
		parent->AddChildren(this);

	mouseHover = false;
}

UIElement::~UIElement()
{
	delete rect;
}


void UIElement::Draw(float dt)
{
	/*
	if (type == LABEL)
	{
		App->render->Blit(texture, pos.x, pos.y, {}, false, 0.0);
	}
	else
		App->render->Blit(texture, pos.x, pos.y, &(rect), false, 0.0);
		*/
}

void UIElement::Update(float dt)
{
	SDL_Rect* button_rect;	
	button_rect = rect;

	if (type == BUTTON) {

		int mouse_x = App->input->GetMouseX(), mouse_y = App->input->GetMouseY();

		if (state == MOUSE_ENTER) {
			state = FOCUSED;
		}

		if ((mouse_x > this->pos.x && mouse_x < this->pos.x + button_rect->w) && (mouse_y > this->pos.y && mouse_y < this->pos.y + button_rect->h) && state != L_MOUSE_PRESSED && state != FOCUSED) {
			state = MOUSE_ENTER;
			//this->callback->UIEvent(this, state);
			LOG("Mouse Enter");
		}
		else if (state == FOCUSED && ((mouse_x < this->pos.x || mouse_x > this->pos.x + button_rect->w) || (mouse_y < this->pos.y || mouse_y > this->pos.y + button_rect->h))) {
			state = MOUSE_LEAVE;
			//this->callback->UIEvent(this, state);
			LOG("Mouse Leave");
		}
		else if (state == MOUSE_LEAVE)
			state = NO_STATE;

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state == FOCUSED) {
			state = L_MOUSE_PRESSED;
			//this->callback->UIEvent(this, state);
			LOG("Mouse Left Click");
		}
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && state == L_MOUSE_PRESSED) {
			state = NO_STATE;
			//this->callback->UIEvent(this, state);
			LOG("Mouse Stop Left Click");
		}

	}

}



UI_TYPE UIElement::GetType() const
{
	return type;
}

void UIElement::AddChildren(UIElement * children)
{
	if (children != nullptr)
	{
		children->parent = this;
		this->children.push_back(children);
	}
}


