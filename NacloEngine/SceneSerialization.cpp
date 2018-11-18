#include "SceneSerialization.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Config.h"

SceneSerialization::SceneSerialization()
{
}

SceneSerialization::~SceneSerialization()
{
	aux_go.clear();
}

bool SceneSerialization::SaveScene(const char * file_name)
{
	bool ret = true;

	Config *file = new Config(file_name);

	file->SetArray("Game Objects");

	for (std::vector<GameObject*>::const_iterator it = App->scene->gameObjects.begin() + 1; it != App->scene->gameObjects.end(); ++it)
	{
		(*it)->SaveGO(file);
	}

	ret = file->Save();

	delete file;
	return ret;
}

bool SceneSerialization::LoadScene(const char * file_name)
{
	bool ret = true;


	App->scene->DestroyAllGameObjects();
	App->scene->root = new GameObject(nullptr, "root");
	ComponentTransform* root_transform = (ComponentTransform*)App->scene->root->NewComponent(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);

	//TODO Destroy the scene if file is correct
	//TODO Revise Loading Scenes, we can't load scenes inside Assets Directory :/
	//TODO LOAD .json using physfs

	std::string final_path = file_name;
	final_path.append(".json");


	Config *file = new Config(final_path.c_str());
	
	int size = file->GetArraySize("Game Objects");

	for (int i = 0; i < size; i++)
	{
		GameObject* go = new GameObject(nullptr, "");

		go->LoadGO(file->GetArray("Game Objects", i));
	}

	for (std::vector<GameObject*>::const_iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		SetGameObjectHierarchy((*it));
	}

	delete file;
	return ret;
}

bool SceneSerialization::ShowSavingOption(TypeSave type)
{
	bool ret = true;

	if (saved_once == false || type == SAVE_AS)
	{
		ImGui::OpenPopup("Save");
		if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Folder:"); ImGui::SameLine();
			ImGui::Text("/Game/Scenes/");
			ImGui::Separator();


			static char file_name[64] = "file_name";
			ImGui::InputText("##name", file_name, 64);

			if (ImGui::Button("SAVE", ImVec2(120, 0)))
			{
				saved_once = true;
				SaveScene(file_name);
				ret = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ret = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	return ret;
}

bool SceneSerialization::ShowLoadingOption()
{
	bool ret = true; 

	ImGui::OpenPopup("Load");

	if (ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Folder:"); ImGui::SameLine();
		ImGui::Text("/Game/Scenes/");
		ImGui::Separator();


		static char file_name[64] = "file_name";
		ImGui::InputText("##name", file_name, 64);

		if (ImGui::Button("LOAD", ImVec2(120, 0)))
		{
			ret = false;
			LoadScene(file_name);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ret = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return ret;
}


void SceneSerialization::SetGameObjectHierarchy(GameObject* go)
{
	for (std::vector<GameObject*>::const_iterator it = App->scene->gameObjects.begin() + 1; it != App->scene->gameObjects.end(); ++it)
	{
		if ((*it)->parent_UID == go->goUID)
		{
			(*it)->parent = go;
			go->AddChildren((*it));
		}
	}
}
