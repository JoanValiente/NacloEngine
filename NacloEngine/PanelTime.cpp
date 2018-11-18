#include "PanelTime.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleTimer.h"
#include "GameObject.h"
#include "Imgui/imgui.h"

PanelTime::PanelTime() : Panel("Time")
{
	width = 375;
	height = 40;
	posx = 375;
	posy = 20;

	active = true;
}

PanelTime::~PanelTime()
{
}

void PanelTime::Draw()
{
	ImGui::Begin("Quick Bar", &active,
		ImGuiWindowFlags_NoTitleBar |
		//ImGuiWindowFlags_NoCollapse |
		//ImGuiWindowFlags_NoMove |
		//ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar
	);

	ENGINE_STATE state = App->engineState;

	
	if (ImGui::Button("PLAY", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->gameState = GAME_STATE::PLAY;
		App->engineState = ENGINE_STATE::GAME;
		App->camera->activeCamera = App->scene->mainCamera;
	}

	ImGui::SameLine();

	if (ImGui::Button("STOP", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->gameState = GAME_STATE::STOP;
		App->engineState = ENGINE_STATE::EDITOR;
		App->camera->activeCamera = App->camera->camera;
	}

	ImGui::SameLine();

	if (ImGui::Button("PAUSE", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		if (App->gameState == GAME_STATE::PAUSE) {
			App->gameState = GAME_STATE::PLAY;
		}
		else {
			App->gameState = GAME_STATE::PAUSE;
		}
	} 

	ImGui::SameLine();

	if (ImGui::Button("TICK", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->gameState = GAME_STATE::TICK;
		App->timer->isTick = true;
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
