#include "PanelTime.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleTimer.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "SceneSerialization.h"
#include "ModuleFileSystem.h"
#include "Imgui/imgui.h"

PanelTime::PanelTime() : Panel("Time")
{
	width = 300;
	height = 28;
	posx = 325;
	posy = 20;

	active = true;
}

PanelTime::~PanelTime()
{
}

void PanelTime::Draw()
{
	int width;
	int height;
	SDL_GetWindowSize(App->window->window, &width, &height);

	ImGui::SetNextWindowPos(ImVec2(width / 4 - width / 42, height / 55));
	ImGui::SetNextWindowSize(ImVec2(width / 2 + width / 18, height / 29));

	ImGui::Begin("Quick Bar", &active,
		ImGuiWindowFlags_NoTitleBar |
		//ImGuiWindowFlags_NoCollapse |
		//ImGuiWindowFlags_NoMove |
		//ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar
	);

	ENGINE_STATE state = App->engineState;


	if (ImGui::Button("PLAY", ImVec2(60, 22)))
	{
		App->gameState = GAME_STATE::PLAY;
		App->engineState = ENGINE_STATE::GAME;
		App->sceneser->SaveScene("tmp");
		App->camera->activeCamera = App->scene->mainCamera;
		App->renderer3D->ChangeCamera();
	}

	ImGui::SameLine();

	if (ImGui::Button("STOP", ImVec2(60, 22)))
	{
		if (App->engineState == ENGINE_STATE::GAME) {
			App->gameState = GAME_STATE::STOP;
			App->engineState = ENGINE_STATE::EDITOR;
			App->sceneser->LoadScene("tmp");
			std::string path = "Assets/Scenes/tmp.json";
			App->fs->Phys_DeleteFile(path.c_str());
			App->camera->activeCamera = App->camera->camera;
			App->renderer3D->ChangeCamera();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("PAUSE", ImVec2(60, 22)))
	{
		if (App->gameState == GAME_STATE::PAUSE) {
			App->gameState = GAME_STATE::PLAY;
		}
		else {
			App->gameState = GAME_STATE::PAUSE;
		}
	}

	ImGui::SameLine();

	char gameTime[20] = "";
	sprintf_s(gameTime, sizeof(gameTime), "%.3f", App->timer->GetTime());
	ImGui::Text(gameTime);

	ImGui::SameLine();

	if (ImGui::SliderFloat("TimeScale", &timeScale, 0.0f, 2.0f))
	{
		App->timer->timeScale = timeScale;
	}

	ImGui::End();
}
