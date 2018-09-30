#include "PanelAbout.h"
#include "Imgui/imgui.h"
#include "Application.h"

// ---------------------------------------------------------
PanelAbout::PanelAbout() : Panel("About")
{
	active = false;
}

// ---------------------------------------------------------
PanelAbout::~PanelAbout()
{}

// ---------------------------------------------------------
void PanelAbout::Draw()
{
	if (ImGui::Begin("About"), &active)
	{
		ImGui::InputText("Engine Name", App->engine_name, 20);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Videogame Engine developed by 2 students from UPC CITM");


		ImGui::Text("AUTHORS");
		ImGui::Spacing();
		ImGui::Spacing();


		ImGui::Text("David Lozano Sanchez");
		ImGui::Text("Joan Valiente Lorite");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("LIBRARIES");
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("SDL2");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "https://www.libsdl.org/download-2.0.php", NULL, NULL, NULL);
		}
		ImGui::Text("ImGui");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "https://github.com/ocornut/imgui", NULL, NULL, NULL);
		}
		ImGui::Text("PCG");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "http://www.pcg-random.org/download.html", NULL, NULL, NULL);
		}
		ImGui::Text("MathGeoLib");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "https://github.com/juj/MathGeoLib", NULL, NULL, NULL);
		}
		ImGui::Text("MMGR");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", NULL, NULL, NULL);
		}
		ImGui::Text("OpenGl");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "http://glew.sourceforge.net/", NULL, NULL, NULL);
		}

		ImGui::Text("Glew");
		if (ImGui::IsItemClicked())
		{
			ShellExecuteA(NULL, NULL, "https://github.com/nigels-com/glew", NULL, NULL, NULL);
		}



		
	}
	ImGui::End();
}

