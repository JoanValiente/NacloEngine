#include "PanelInspector.h"
#include "Imgui/imgui.h"
#include "Application.h"

PanelInspector::PanelInspector() : Panel ("Inspector")
{
	active = true;
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
	if (ImGui::Begin("patata", &active))
	{
		App->meshes->ShowMeshInformation(&App->meshes->mesh);
		ImGui::End();
	}
}
