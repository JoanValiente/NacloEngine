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
	ImGui::SetNextWindowBgAlpha(0.5f);

	if (!ImGui::Begin(("About"), &active))
	{
		ImGui::End();
	}
	else
	{
	
		if (ImGui::Button(App->engine_name))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/JoanValiente/NacloEngine", NULL, NULL, NULL);
		}
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Videogame Engine developed by 2 students from UPC CITM");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("AUTHORS");
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
		ImGui::Separator();
		ImGui::Text("LICENSE");
		ImGui::Text("Copyright 2018 MIT.");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining\na copy of thissoftware and associated documentation files (the 'Software'),\nto deal in the Software without restriction, including without\nlimitation the rights touse, copy, modify, merge, publish, distribute, sublicense,\nand/or sell copies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions:");
		ImGui::Spacing();
		ImGui::Text("The above copyright notice and this permission notice shall be\nincluded in all copies or substantial portions of the Software.");
		ImGui::Spacing();
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS \nOR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN\nNO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR\nOTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF ORIN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");


		ImGui::End();
	}
}

