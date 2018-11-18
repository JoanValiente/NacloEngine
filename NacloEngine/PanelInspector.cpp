#include "PanelInspector.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
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
	GameObject* go = App->scene->GetSelected();
	static bool add_component = false;

	if (go != nullptr)
	{
		if (ImGui::Begin("Inspector", &active))
		{
			ImGui::Text(go->name.c_str());
			bool staticGO = go->staticGO;
			if (ImGui::Checkbox("Static", &staticGO)) {
				go->staticGO = staticGO;
				if (staticGO) {
					App->scene->quadtreeUpdate = true;
				}
			}

			go->Inspector();
			ImGui::End();
		}
	}
}
