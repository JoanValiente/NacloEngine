#include "PanelHierarchy.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"

PanelHierarchy::PanelHierarchy() : Panel("Hierarchy")
{
	active = true;
}

PanelHierarchy::~PanelHierarchy()
{
}

void PanelHierarchy::Draw()
{
	if(ImGui::Begin("Game Objects", &active)) 
	{
		GameObject* root = App->scene->root;

		for (std::vector<GameObject*>::const_iterator it = root->children.begin(); it < root->children.end(); it++) {
			if ((*it)->active) {
				ShowGameObjectHierarchy(*it);
			}
		}

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}

void PanelHierarchy::ShowGameObjectHierarchy(GameObject * go)
{
	if (ImGui::TreeNodeEx(go->name.c_str()))
	{
		for (std::vector<GameObject*>::const_iterator it = go->children.begin(); it < go->children.end(); it++) {
			ShowGameObjectHierarchy(*it);
		}

		ImGui::TreePop();
	}
}
