#include "Glew/include/glew.h"
#include "Component.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "ComponentInteractive.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Globals.h"

ComponentCanvas::ComponentCanvas(GameObject* container) : Component(container)
{
	this->type = COMPONENT_CANVAS;

	App->scene->canvas.push_back(container);

	if (container->rectTransform != nullptr)
	{
		container->rectTransform->default_height = 50;
		container->rectTransform->default_width = 50;
	}
}

ComponentCanvas::~ComponentCanvas()
{
	for (std::vector<GameObject*>::iterator it = App->scene->canvas.begin(); it != App->scene->canvas.end();)
	{
		if ((*it)->goUID == container->goUID)
		{
			App->scene->canvas.erase(it);
			return;
		}
	}
}

void ComponentCanvas::Update(float dt)
{
	SetCanvasPosition();
	ChangeCambasState();
	
	if (App->engineState == ENGINE_STATE::EDITOR)
	{
		DebugDraw();
	}

	else
	{
		UpdateInteractive();
	}
}

void ComponentCanvas::ShowInspector()
{
	if (ImGui::CollapsingHeader("Canvas"))
	{
	}
}

void ComponentCanvas::DebugDraw()
{
	if (container->rectTransform != nullptr) 
	{
		glBegin(GL_LINES);
		glLineWidth(8.0f);
		glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

		float canvas_height = container->rectTransform->default_height;
		float canvas_width = container->rectTransform->default_width;

		float3 pos = container->rectTransform->position;
		float3 v1 = float3(pos.x - canvas_width, pos.y - canvas_height, pos.z);
		float3 v2 = float3(pos.x + canvas_width, pos.y - canvas_height, pos.z);
		float3 v3 = float3(pos.x + canvas_width, pos.y + canvas_height, pos.z);
		float3 v4 = float3(pos.x - canvas_width, pos.y + canvas_height, pos.z);;

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

void ComponentCanvas::UpdateInteractive()
{
	for (std::vector<ComponentInteractive*>::iterator it = interactive_components.begin(); it != interactive_components.end(); it++)
	{
		if ((*it)->container->active)
			(*it)->UpdateInteractive();
	}

}

void ComponentCanvas::ChangeCambasState()
{
	static bool canvas_game_mode = false;
	static bool canvas_edit_mode = false;

	if (App->engineState == ENGINE_STATE::GAME)
	{
		if (!canvas_edit_mode)
		{
			SetCanvasGameMode();
			canvas_edit_mode = true;
		}

		canvas_game_mode = false;
	}
	else
	{
		if (!canvas_game_mode)
		{
			SetCanvasEditorMode();
			canvas_game_mode = true;
		}
		canvas_edit_mode = false;
	}
}

void ComponentCanvas::SetCanvasGameMode()
{

	container->rectTransform->SetRotation(float3(container->rectTransform->rotation.x, 180,0));
	container->rectTransform->SetNewSize(App->scene->main_camera->camera->frustum.NearPlaneWidth() / 2, App->scene->main_camera->camera->frustum.NearPlaneHeight() / 2);
}

void ComponentCanvas::SetCanvasEditorMode()
{
	container->rectTransform->SetNewSize(container->rectTransform->default_width, container->rectTransform->default_height);
}

void ComponentCanvas::SetCanvasPosition()
{
	if (App->engineState == ENGINE_STATE::GAME)
	{
		container->rectTransform->SetPosition(App->scene->main_camera->camera->frustum.NearPlanePos(0.0f, 0.0f));
		container->rectTransform->SetQuaternion(App->scene->main_camera->transform->quaternion);
	}
	else
	{
		container->rectTransform->SetPosition(float3::zero);
		container->rectTransform->SetQuaternion(Quat::identity);
	}
}


void ComponentCanvas::SaveComponent(Config &conf)
{
}

void ComponentCanvas::LoadComponent(Config & conf)
{
	App->scene->canvas.push_back(container);
}