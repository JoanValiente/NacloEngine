#include "ComponentMaterial.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
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
	if (texture != nullptr)
		delete texture;
}

void ComponentMaterial::AssignTexture(Texture* texture)
{
	this->texture = texture;
}

void ComponentMaterial::ShowInspector()
{
	if (ImGui::CollapsingHeader("Texture"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (texture != nullptr)
		{
			ImGui::InputText("##Name", (char*)texture->texture_name.c_str(), 64);
			ImGui::Text("SIZE");
			ImGui::Text("Width: %i", texture->width); ImGui::SameLine();
			ImGui::Text("Height: %i", texture->height);

			if (ImGui::Button("Change Texture"))
			{
				ImGui::OpenPopup("Change_Texture");
			}
			ImGui::Image((ImTextureID)texture->texture_id, { 256,256 });

			if (ImGui::BeginPopup("Change_Texture"))
			{
				list<Texture*>::const_iterator item = App->texture->textures_loaded.begin();

				while (item != App->texture->textures_loaded.end())
				{
					if ((*item) != nullptr)
					{
						ImGui::Image((ImTextureID)(*item)->texture_id, { 25,25 });
						if (ImGui::IsItemClicked())
						{
							texture->texture_id = (*item)->texture_id;
						}
					}
					item++;
				}
				ImGui::EndPopup();
			}
		}
		else
		{
			ImGui::Text("No texture assigned");

			if (ImGui::Button("Add Texture"))
			{
				ImGui::OpenPopup("Change_Texture");
			}
			ImGui::Image(0, { 256,256 });

			if (ImGui::BeginPopup("Change_Texture"))
			{
				list<Texture*>::const_iterator item = App->texture->textures_loaded.begin();

				while (item != App->texture->textures_loaded.end())
				{
					if ((*item) != nullptr)
					{
						ImGui::Image((ImTextureID)(*item)->texture_id, { 25,25 });
						if (ImGui::IsItemClicked())
						{
							texture = (*item);
							texture->texture_id = (*item)->texture_id;
						}
					}
					item++;
				}
				ImGui::EndPopup();
			}
		}
	}
}

void ComponentMaterial::SaveComponent(Config & conf)
{
	if (texture != nullptr)
	{
		conf.SetString("Path", texture->path.c_str());
		conf.SetString("Name", texture->texture_name.c_str());
		conf.SetString("DDS Path", texture->texture_dds.c_str());
		conf.SetInt("Width", texture->width);
		conf.SetInt("Height", texture->height);
	}
}

void ComponentMaterial::LoadComponent(Config & conf)
{
	if (conf.GetString("DDS Path") != NULL)
	{
		texture = App->texture->LoadTexture(conf.GetString("DDS Path"));
		if (texture != nullptr)
		{
			texture->texture_dds = conf.GetString("DDS Path");
			texture->path = conf.GetString("Path");
			texture->texture_name = conf.GetString("Name");
			texture->width = conf.GetInt("Width");
			texture->height = conf.GetInt("Height");
		}

	}
}
