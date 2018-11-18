#include "PanelInspector.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"

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
		int width;
		int height;
		SDL_GetWindowSize(App->window->window, &width, &height);

		ImGui::SetNextWindowPos(ImVec2(width / 2 + width / 4 + 43, 19));
		ImGui::SetNextWindowSize(ImVec2(width / 4 - 40, height / 2 + 210));
		

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
