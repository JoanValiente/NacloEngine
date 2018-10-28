#include "ComponentMaterial.h"
#include "ModuleTextures.h"
#include "GameObject.h"


ComponentMaterial::ComponentMaterial(GameObject * container) : Component(container)
{
	type = COMPONENT_MATERIAL;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::AssignTexture(Texture* texture)
{
	this->texture = texture;
}
