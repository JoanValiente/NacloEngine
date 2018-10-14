#include "PanelOpenGL.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "OpenGL.h"



PanelOpenGl::PanelOpenGl() : Panel("OpenGL")
{
	active = false;
}

PanelOpenGl::~PanelOpenGl()
{
}

void PanelOpenGl::Draw()
{
	if (ImGui::Begin("Open Gl Options"), &active)
	{
	
		ImGui::End();
	}
}
