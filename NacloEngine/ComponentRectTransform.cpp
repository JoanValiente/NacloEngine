#include "Component.h"
#include "ComponentRectTransform.h"
#include "GameObject.h"
#include "Globals.h"


ComponentRectTransform::ComponentRectTransform(GameObject* container) : Component(container)
{
	this->type = COMPONENT_RECT_TRANSFORM;
}

void ComponentRectTransform::ShowInspector()
{

}

void ComponentRectTransform::SaveComponent(Config &conf)
{
}

void ComponentRectTransform::LoadComponent(Config & conf)
{
}