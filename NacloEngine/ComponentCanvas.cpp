#include "Glew/include/glew.h"
#include "Component.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "GameObject.h"
#include "Globals.h"


ComponentCanvas::ComponentCanvas(GameObject* container) : Component(container)
{
	this->type = COMPONENT_CANVAS;
}

void ComponentCanvas::Update(float dt)
{
	DebugDraw();
}

void ComponentCanvas::ShowInspector()
{
	if (ImGui::CollapsingHeader("Canvas"))
	{
	}
}

void ComponentCanvas::DebugDraw()
{
	if (container->rectTransform != nullptr) {

		glBegin(GL_LINES);
		glLineWidth(8.0f);
		glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

		float w = container->rectTransform->width / 2;
		float h = container->rectTransform->height / 2;
		float3 pos = container->rectTransform->position;
		float3 v1 = float3(pos.x - w, pos.y - h, pos.z);
		float3 v2 = float3(pos.x + w, pos.y - h, pos.z);
		float3 v3 = float3(pos.x + w, pos.y + h, pos.z);
		float3 v4 = float3(pos.x - w, pos.y + h, pos.z);;

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