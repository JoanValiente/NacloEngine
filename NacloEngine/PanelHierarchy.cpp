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

		for (int iterator = 0; iterator < root->GetNumChildren(); ++iterator) {
			if (root->children.at(iterator)->active) 
			{
				ShowGameObjectHierarchy(root->children.at(iterator));
			}
		}

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}

	if (move)
	{
		target->ChangeHierarchy(toMove);
		move = false;
	}

}

bool PanelHierarchy::ShowGameObjectHierarchy(GameObject * go)
{
	bool ret = true;
	if (go != nullptr)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		bool node_open = false;

		if (go == App->scene->GetSelected())
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (go->GetNumChildren() == 0)
		{
			node_flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (ImGui::TreeNodeEx(go->name.c_str(), node_flags))
		{
			node_open = true;
		}
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("Hierarchy_Nodes", go, sizeof(GameObject));
			ImGui::EndDragDropSource();
		}
		

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Hierarchy_Nodes"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject));
				ret = false;
				move = true;
				target = go;
				toMove = (GameObject*)payload->Data;
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
		{
			App->scene->SetSelected(go);
		}

		if (node_open)
		{
			if (go->GetNumChildren() != 0)
			{
				for (std::vector<GameObject*>::const_iterator it = go->children.begin(); it < go->children.end(); ++it)
				{
					ImGui::PushID(*it);
					ShowGameObjectHierarchy(*it);
					ImGui::PopID();
				}
			}
			ImGui::TreePop();
		}
	}
	return ret;
}
