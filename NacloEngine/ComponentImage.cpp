#include "OpenGL.h"
#include "Component.h"
#include "ComponentImage.h"
#include "GameObject.h"
#include "Globals.h"


ComponentImage::ComponentImage(GameObject* container) : Component(container)
{
	this->type = COMPONENT_IMAGE;
}

void ComponentImage::ShowInspector()
{	
	if (ImGui::CollapsingHeader("Image"))
	{
	}
}

void ComponentImage::AssignTexture(Texture * texture)
{
	this->tex = texture;
}

void ComponentImage::SaveComponent(Config &conf)
{
}

void ComponentImage::LoadComponent(Config & conf)
{
}
