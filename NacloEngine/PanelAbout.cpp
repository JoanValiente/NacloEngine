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


		if (ImGui::Button("David Lozano Sanchez"))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/DavidTheMaaster", NULL, NULL, NULL);
		}
		if (ImGui::Button("Joan Valiente Lorite"))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/JoanValiente", NULL, NULL, NULL);
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("LIBRARIES");
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("SDL"))
		{
			ShellExecuteA(NULL, NULL, "https://www.libsdl.org/download-2.0.php", NULL, NULL, NULL);
		}
		ImGui::SameLine();
		if (ImGui::Button("ImGui"))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/ocornut/imgui", NULL, NULL, NULL);
		}
		ImGui::SameLine();
		if (ImGui::Button("PCG"))
		{
			ShellExecuteA(NULL, NULL, "http://www.pcg-random.org/download.html", NULL, NULL, NULL);
		}
		ImGui::SameLine();
		if (ImGui::Button("MathGeoLib"))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/juj/MathGeoLib", NULL, NULL, NULL);
		}
		ImGui::SameLine();
		if (ImGui::Button("MMGR"))
		{
			ShellExecuteA(NULL, NULL, "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", NULL, NULL, NULL);
		}
		if (ImGui::Button("OpenGl"))
		{
			ShellExecuteA(NULL, NULL, "http://glew.sourceforge.net/", NULL, NULL, NULL);
		}
		ImGui::SameLine();
		if (ImGui::Button("Glew"))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/nigels-com/glew", NULL, NULL, NULL);
		}		
		ImGui::SameLine();
		if (ImGui::Button("Parson"))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/kgabis/parson", NULL, NULL, NULL);
		}
		ImGui::SameLine();
		if (ImGui::Button("Assimp"))
		{
			ShellExecuteA(NULL, NULL, "http://www.assimp.org/", NULL, NULL, NULL);
		}
		ImGui::SameLine();
		if (ImGui::Button("Devil"))
		{
			ShellExecuteA(NULL, NULL, "http://openil.sourceforge.net/", NULL, NULL, NULL);
		}
	}
	ImGui::End();
}

