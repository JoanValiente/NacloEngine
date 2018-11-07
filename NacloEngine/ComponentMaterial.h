#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"

struct Texture;

class ComponentMaterial : public Component {

public:
	ComponentMaterial(GameObject* container);
	~ComponentMaterial();

	void AssignTexture(Texture* texture);
	void ShowInspector();

public:
	Texture* texture = nullptr;
};

#endif
