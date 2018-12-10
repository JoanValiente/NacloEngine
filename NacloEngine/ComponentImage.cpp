#include "OpenGL.h"
#include "Component.h"
#include "ComponentImage.h"
#include "GameObject.h"
#include "Globals.h"
#include "Primitive.h"
#include "ComponentRectTransform.h"


ComponentImage::ComponentImage(GameObject* container) : Component(container)
{
	this->type = COMPONENT_IMAGE;
	if (container->rectTransform != nullptr)
	{
		image_rect = new plane(container->rectTransform->position, float3(5.0f, 5.0f, 0.0f));
		container->rectTransform->UpdateMatrix();
	}

	else
	{
		LOG("Error creating Image Rect, no rect transform component created");
	}
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
