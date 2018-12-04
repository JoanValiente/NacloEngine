#include "Component.h"
#include "ComponentCanvas.h"
#include "GameObject.h"
#include "Globals.h"


ComponentCanvas::ComponentCanvas(GameObject* container) : Component(container)
{
	this->type = COMPONENT_CANVAS;
}

void ComponentCanvas::ShowInspector()
{
	if (ImGui::CollapsingHeader("Canvas"))
	{
	}
}

void ComponentCanvas::SaveComponent(Config &conf)
{
}

void ComponentCanvas::LoadComponent(Config & conf)
{
}