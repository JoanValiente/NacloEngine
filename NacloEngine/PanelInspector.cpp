#include "PanelInspector.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleScene.h"

PanelInspector::PanelInspector() : Panel ("Inspector")
{
	active = true;
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::DrawInspector()
{
	GameObject* go = App->scene->selected;

	if (go != nullptr)
	{
		if (ImGui::Begin("Inspector", &active))
		{
			go->Inspector();
			ImGui::End();
		}
	}
}
