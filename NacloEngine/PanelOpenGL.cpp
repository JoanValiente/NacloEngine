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
		static bool depth_test = glIsEnabled(GL_DEPTH_TEST);
		static bool cull_face = glIsEnabled(GL_CULL_FACE);
		static bool lighting = glIsEnabled(GL_LIGHTING);
		static bool color_material = glIsEnabled(GL_COLOR_MATERIAL);
		static bool texture_2D = glIsEnabled(GL_TEXTURE_2D);
		static bool checkers_mode = false;
		static uint textureid = 0;

		if (ImGui::Checkbox("Depth Test", &depth_test))
		{
			if (depth_test)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}

		if (ImGui::Checkbox("Cull Face",&cull_face))
		{
			if (cull_face)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}


		if (ImGui::Checkbox("Lighting", &lighting))
		{
			if (lighting)
			{
				glEnable(GL_LIGHTING);
			}
			else
			{
				glDisable(GL_LIGHTING);
			}
		}

		if (ImGui::Checkbox("Color Material", &color_material))
		{
			if (color_material)
			{
				glEnable(GL_COLOR_MATERIAL);
			}
			else
			{
				glDisable(GL_COLOR_MATERIAL);
			}
		}

		if (ImGui::Checkbox("Texture 2D", &texture_2D))
		{
			if (texture_2D)
			{
				glEnable(GL_TEXTURE_2D);
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}
		}

		if (ImGui::Checkbox("Checkers Mode", &checkers_mode))
		{
			if (checkers_mode)
			{
				textureid = App->meshes->mesh->texture_path;
				App->meshes->mesh->texture_path = App->texture->LoadCheckersTexture();
			}
			else
			{
				App->meshes->mesh->texture_path = textureid;
			}
		}
	
		ImGui::End();
	}
}
