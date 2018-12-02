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

		ImGui::SetNextWindowPos(ImVec2(width / 2 + width / 4 + 53, height/55));
		ImGui::SetNextWindowSize(ImVec2(width / 4 - 50, height / 2 + height/5));		

		if (ImGui::Begin("Inspector", &active))
		{
			ImGui::InputText("Name:", (char*)go->name.c_str(), 64);
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
