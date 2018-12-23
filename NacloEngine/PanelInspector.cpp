#include "PanelInspector.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
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
	if (App->engineState == ENGINE_STATE::EDITOR) {
		GameObject* go = App->scene->GetSelected();

		if (go != nullptr)
		{
			int width;
			int height;
			SDL_GetWindowSize(App->window->window, &width, &height);

			ImGui::SetNextWindowPos(ImVec2(width / 2 + width / 4 + 53, height / 55));
			ImGui::SetNextWindowSize(ImVec2(width / 4 - 50, height / 2 + height / 5));
			ImGui::SetNextWindowBgAlpha(0.5f);

			if (ImGui::Begin("Inspector", &active))
			{
				if (ImGui::Checkbox("##activate_btn", &go->activated))
				{
					go->SetActive(go, go->activated);
				}
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				ImGui::InputText("##name", (char*)go->name.c_str(), 64); ImGui::SameLine();
				bool staticGO = go->staticGO;
				if (ImGui::Checkbox("Static", &staticGO)) {
					go->staticGO = staticGO;
					App->scene->quadtreeUpdate = true;
					go->ChangeStaticChildren(go, staticGO);
				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("##delete", ImGuiDir_Down))
				{
					ImGui::OpenPopup("Delete");
				}
				go->Inspector();

				ImGui::Separator();

				if (ImGui::Button("Add Component"))
				{
					ImGui::OpenPopup("Component");
				}

				if (ImGui::BeginPopup("Component"))
				{
					if (go->mesh == nullptr)
					{
						if (ImGui::Selectable("Mesh"))
						{
							ComponentMesh* mesh = (ComponentMesh*)go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_MESH);
							mesh->AssignMesh(new Mesh);
						}
					}
					if (go->material == nullptr)
					{
						if (ImGui::Selectable("Material"))
						{
							ComponentMaterial* material = (ComponentMaterial*)go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_MATERIAL);
							material->AssignTexture(new Texture);
						}
					}
					if (ImGui::Selectable("Camera"))
					{
						go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_CAMERA);
					}
					if (ImGui::Selectable("RectTransform"))
					{
						go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
					}
					if (ImGui::Selectable("Canvas"))
					{
						go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
						go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_CANVAS);
					}
					if (ImGui::Selectable("Image"))
					{
						go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);
						go->NewComponent(Component::COMPONENT_TYPE::COMPONENT_IMAGE);
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopup("Delete"))
				{
					if (ImGui::Selectable("Delete GameObject"))
					{
						App->scene->DeleteGameObject(go);
					}
					ImGui::EndPopup();
				}

				ImGui::End();
			}
		}
	}
}
