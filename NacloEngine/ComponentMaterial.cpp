#include "ComponentMaterial.h"
#include "OpenGL.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

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
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("SIZE");
		//ImGui::Text("Width: %i", texture->texture_width); ImGui::SameLine();
		//ImGui::Text("Height: %i", texture->texture_height);
		ImGui::Image((ImTextureID)texture->texture_id, { 256,256 });
	}
}
