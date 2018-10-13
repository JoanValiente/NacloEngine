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

	if (!App->renderer3D->meshes.empty())
	{
		App->meshes->ShowMeshInformation();
	}

}
