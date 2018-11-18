#include "PanelTime.h"
#include "Application.h"
#include "ModuleInput.h"
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
	ImGui::Begin("Quick Bar", &active,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar
	);

	ENGINE_STATE state = App->engineState;

	
	if (ImGui::Button("PLAY", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->Play();
	
	ImGui::SameLine();

	if (ImGui::Button("STOP", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->Stop();

	ImGui::SameLine();

	if (ImGui::Button("PAUSE", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->Pause();
	
	ImGui::SameLine();

	if (ImGui::Button("->", ImVec2(60, 22)) || App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->Pause();

	ImGui::End();
}
