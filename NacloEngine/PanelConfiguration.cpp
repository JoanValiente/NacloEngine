#include "PanelConfiguration.h"
#include "Application.h"

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
			ImGui::InputText("Engine Name", App->engine_name, 20);
			ImGui::InputText("Organization", App->organization_name, 20);

			if (vector_fps.size() != 100)
			{
				vector_fps.push_back(ImGui::GetIO().Framerate);
				vector_ms.push_back(1000 / ImGui::GetIO().Framerate);
			}

			else
			{
				vector_fps.erase(vector_fps.begin());
				vector_fps.push_back(ImGui::GetIO().Framerate);

				vector_ms.erase(vector_ms.begin());
				vector_ms.push_back(1000 / ImGui::GetIO().Framerate);
			}

			char title[25];
			ImGui::Spacing();

			ImGui::SliderInt("FPS Cap", &App->FPS_cap, 0, 60);
			ImGui::Spacing();


			sprintf_s(title, 25, "Framerate %.1f", vector_fps[vector_fps.size() - 1]);
			ImGui::PlotHistogram("##framerate", &vector_fps[0], vector_fps.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			ImGui::Spacing();


			sprintf_s(title, 25, "Milliseconds %.1f", vector_ms[vector_ms.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &vector_ms[0], vector_ms.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		}


		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
			{
				App->window->SetBrightnes(App->window->window, App->window->brightness);
			}

			if (ImGui::SliderInt("Width", &App->window->width, 1, 1920) || ImGui::SliderInt("Height", &App->window->height, 1, 1080))
			{
				App->window->SetWindowSize(App->window->window, App->window->width, App->window->height);
			}
			ImGui::Spacing();

			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			{
				App->window->SetWindowFullscreen(App->window->window);
			}

			if (ImGui::Checkbox("Borderless", &App->window->borderless))
			{
				App->window->SetWindowBorderless(App->window->window);
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Restart to apply");
			}

			if (ImGui::Checkbox("Resizable", &App->window->resizable))
			{
				App->window->SetWindowResizable(App->window->window);
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Restart to apply");
			}

			if (ImGui::Checkbox("Fullscreen Desktop", &App->window->fullscreen_desktop))
			{
				App->window->SetWindowFullDesktop(App->window->window);
			}

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