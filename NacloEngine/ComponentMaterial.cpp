#include "ComponentMaterial.h"
#include "OpenGL.h"
#include "GameObject.h"


ComponentMaterial::ComponentMaterial(GameObject * container) : Component(container)
{
	type = COMPONENT_MATERIAL;
	active = true;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::AssignTexture(Texture* texture)
{
	this->texture = texture;
}
