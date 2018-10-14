#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "mmgr\mmgr.h"


PanelConfiguration::PanelConfiguration() : Panel("Configuration")
{
	active = true;

}

// ---------------------------------------------------------
PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw()
{
	if (!ImGui::Begin("Configuration", &active))
	{
		ImGui::End();
	}
	else
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			App->ShowApplicationInfo();
		}


		if (ImGui::CollapsingHeader("Window"))
		{
			App->window->ShowWindowInfo();
		}

		if (ImGui::CollapsingHeader("Camera"))
		{
			App->camera->ShowCameraInfo();
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			App->input->ShowInputInformation();
		}

		if (ImGui::CollapsingHeader("Render"))
		{
			App->renderer3D->ShowRenderInformation();
		}

		if (ImGui::CollapsingHeader("Textures"))
		{
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL Version: ");
			ImGui::SameLine();

			SDL_version curren_version;
			SDL_VERSION(&curren_version);
			char sdlVer[38];
			sprintf_s(sdlVer, sizeof(sdlVer), "%d.%d.%d", curren_version.major, curren_version.minor, curren_version.patch);
			ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), sdlVer);
			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();

			int cpu = SDL_GetCPUCount();
			char cpuVer[38];
			sprintf_s(cpuVer, sizeof(sdlVer), "%i", cpu);
			ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), cpuVer);


			ImGui::Text("System RAM: ");
			ImGui::SameLine();

			cpu = SDL_GetSystemRAM() / 1000;
			sprintf_s(cpuVer, sizeof(sdlVer), "%i Gb", cpu);
			ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), cpuVer);

			ImGui::Text("Caps: ");
			ImGui::SameLine();

			if (SDL_HasRDTSC())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "RDTSC");
				ImGui::SameLine();
			}
			if (SDL_HasMMX())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "MMX");
				ImGui::SameLine();
			}
			if (SDL_HasAVX())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "AVX");
				ImGui::SameLine();
			}
			if (SDL_HasAVX2())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "AVX2");
				ImGui::SameLine();
			}
			if (SDL_HasSSE())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE");
				ImGui::SameLine();
			}
			if (SDL_HasSSE2())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE2");
				ImGui::SameLine();
			}
			if (SDL_HasSSE3())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE3");
				ImGui::SameLine();
			}
			if (SDL_HasSSE41())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE41");
				ImGui::SameLine();
			}
			if (SDL_HasSSE42())
			{
				ImGui::TextColored(ImVec4(1, 0.7f, 0, 100), "SSE42");
			}
		}
		ImGui::End();
	}
}

