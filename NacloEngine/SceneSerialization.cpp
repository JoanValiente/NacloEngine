#include "SceneSerialization.h"
#include "Application.h"
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

	for (std::vector<GameObject*>::const_iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
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

	Config *file = new Config(file_name);
	
	int size = file->GetArraySize("Game Objects");

	for (int i = 0; i < size; i++)
	{
		GameObject* go = new GameObject(nullptr, "");

		go->LoadGO(file->GetArray("Game Objects", i));
	}


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
			ImGui::Text("/Game/Scenes/");
			ImGui::Separator();

			static char file_name[64] = "file_name";
			ImGui::InputText("##name", file_name, 64);

			if (ImGui::Button("SAVE", ImVec2(120, 0)))
			{
				saved_once = true;
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
