#include "Glew/include/glew.h"
#include "Component.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
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

void ComponentCanvas::Update(float dt)
{
	container->rectTransform->SetPosition(App->scene->main_camera->camera->frustum.NearPlanePos(0.0f,0.0f));
	container->rectTransform->SetQuaternion(App->scene->main_camera->transform->quaternion);
	
	if (App->engineState == ENGINE_STATE::EDITOR) {
		DebugDraw();
	}

	ChangeCambasState();
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

void ComponentCanvas::SaveComponent(Config &conf)
{
}

void ComponentCanvas::LoadComponent(Config & conf)
{
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
	container->rectTransform->SetPosition(App->scene->main_camera->camera->frustum.NearPlanePos(0.0f, 0.0f));
	container->rectTransform->SetQuaternion(App->scene->main_camera->transform->quaternion);

	container->rectTransform->SetHeight(App->scene->main_camera->camera->frustum.NearPlaneHeight() / 2);
	container->rectTransform->SetWidth(App->scene->main_camera->camera->frustum.NearPlaneWidth() / 2);
}

void ComponentCanvas::SetCanvasEditorMode()
{
	container->rectTransform->SetPosition(float3::zero);
	container->rectTransform->SetQuaternion(Quat::identity);

	container->rectTransform->SetHeight(container->rectTransform->default_height);
	container->rectTransform->SetWidth(container->rectTransform->default_width);
}
