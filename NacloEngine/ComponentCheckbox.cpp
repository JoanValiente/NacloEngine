#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "Component.h"


ComponentCheckbox::ComponentCheckbox(GameObject * container) : ComponentInteractive(container)
{
	this->type = COMPONENT_CHECKBOX;
	interactive = true;
}

ComponentCheckbox::~ComponentCheckbox()
{
}

void ComponentCheckbox::ShowInspector()
{
}

void ComponentCheckbox::Update(float dt)
{
	if (checked)
	{
		ExecuteFunction();
	}
}

void ComponentCheckbox::DebugDraw()
{
}

void ComponentCheckbox::SaveComponent(Config & conf)
{
}

void ComponentCheckbox::LoadComponent(Config & conf)
{
}

void ComponentCheckbox::Hover()
{
}

void ComponentCheckbox::Enter()
{
}

void ComponentCheckbox::Exit()
{
}

void ComponentCheckbox::OnClick()
{
	checked = !checked;
}

void ComponentCheckbox::ExecuteFunction()
{
}
