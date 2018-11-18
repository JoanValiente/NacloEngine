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
	GameObject* go = App->scene->GetSelected();

	if (go != nullptr)
	{
		if (ImGui::Begin("Inspector", &active))
		{
			ImGui::Text(go->name.c_str());
			bool staticGO = go->staticGO;
			if (ImGui::Checkbox("Static", &staticGO)) {
				go->staticGO = staticGO;				
				App->scene->quadtreeUpdate = true;				
				go->ChangeStaticChildren(go, staticGO);
			}

			go->Inspector();
			ImGui::End();
		}
	}
}
