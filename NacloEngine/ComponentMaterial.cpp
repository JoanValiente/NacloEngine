#include "ComponentMaterial.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Config.h"
#include "TextureImporter.h"
#include "Application.h"


ComponentMaterial::ComponentMaterial(GameObject * container) : Component(container)
{
	type = COMPONENT_MATERIAL;
	active = true;
}

ComponentMaterial::~ComponentMaterial()
{
	delete texture;
}

void ComponentMaterial::AssignTexture(Texture* texture)
{
	this->texture = texture;
}

void ComponentMaterial::ShowInspector()
{
	if (texture != nullptr)
	{
		if (ImGui::CollapsingHeader("Texture"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Text("SIZE");
			ImGui::Text("Width: %i", texture->width); ImGui::SameLine();
			ImGui::Text("Height: %i", texture->height);
			ImGui::Image((ImTextureID)texture->texture_id, { 256,256 });
		}
	}
}

void ComponentMaterial::SaveComponent(Config & conf)
{
	conf.SetString("Path", texture->path.c_str());
	conf.SetString("Name", texture->texture_name.c_str());
	conf.SetString("DDS Path", texture->texture_dds.c_str());
	conf.SetInt("Width", texture->width);
	conf.SetInt("Height", texture->height);
}

void ComponentMaterial::LoadComponent(Config & conf)
{
	if (conf.GetString("DDS Path") != NULL)
	{
		texture = App->texture->LoadTexture(conf.GetString("DDS Path"));
		texture->texture_dds = conf.GetString("DDS Path");
		texture->path = conf.GetString("Path");
		texture->texture_name = conf.GetString("Name");
		texture->width = conf.GetInt("Width");
		texture->height = conf.GetInt("Height");
	}
}
